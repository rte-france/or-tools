// Copyright 2010-2025 Google LLC
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OR_TOOLS_SAT_SOLUTION_CRUSH_H_
#define OR_TOOLS_SAT_SOLUTION_CRUSH_H_

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/log/check.h"
#include "absl/types/span.h"
#include "ortools/algorithms/sparse_permutation.h"
#include "ortools/sat/cp_model_utils.h"
#include "ortools/util/sorted_interval_list.h"

namespace operations_research {
namespace sat {

class SolutionCrush {
 public:
  SolutionCrush() = default;

  // SolutionCrush is neither copyable nor movable.
  SolutionCrush(const SolutionCrush&) = delete;
  SolutionCrush(SolutionCrush&&) = delete;
  SolutionCrush& operator=(const SolutionCrush&) = delete;
  SolutionCrush& operator=(SolutionCrush&&) = delete;

  // Resizes the solution to contain `new_size` variables. Does not change the
  // value of existing variables, and does not set any value for the new
  // variables.
  // WARNING: the methods below do not automatically resize the solution. To set
  // the value of a new variable with one of them, call this method first.
  void Resize(int new_size);

  // Sets the given values in the solution. `solution` must be a map from
  // variable indices to variable values. This must be called only once, before
  // any other method (besides `Resize`).
  // TODO(user): revisit this near the end of the refactoring.
  void LoadSolution(const absl::flat_hash_map<int, int64_t>& solution);

  // Solution hint accessors.
  bool VarHasSolutionHint(int var) const { return hint_has_value_[var]; }
  int64_t SolutionHint(int var) const { return hint_[var]; }
  bool HintIsLoaded() const { return hint_is_loaded_; }
  absl::Span<const int64_t> SolutionHint() const { return hint_; }

  // Similar to SolutionHint() but make sure the value is within the given
  // domain.
  int64_t ClampedSolutionHint(int var, const Domain& domain) {
    int64_t value = hint_[var];
    if (value > domain.Max()) {
      value = domain.Max();
    } else if (value < domain.Min()) {
      value = domain.Min();
    }
    return value;
  }

  bool LiteralSolutionHint(int lit) const {
    const int var = PositiveRef(lit);
    return RefIsPositive(lit) ? hint_[var] : !hint_[var];
  }

  bool LiteralSolutionHintIs(int lit, bool value) const {
    const int var = PositiveRef(lit);
    return hint_is_loaded_ && hint_has_value_[var] &&
           hint_[var] == (RefIsPositive(lit) ? value : !value);
  }

  // If the given literal is already hinted, updates its hint.
  // Otherwise do nothing.
  void UpdateLiteralSolutionHint(int lit, bool value) {
    UpdateVarSolutionHint(PositiveRef(lit),
                          RefIsPositive(lit) == value ? 1 : 0);
  }

  std::optional<int64_t> GetRefSolutionHint(int ref) {
    const int var = PositiveRef(ref);
    if (!VarHasSolutionHint(var)) return std::nullopt;
    const int64_t var_hint = SolutionHint(var);
    return RefIsPositive(ref) ? var_hint : -var_hint;
  }

  std::optional<int64_t> GetExpressionSolutionHint(
      const LinearExpressionProto& expr) {
    int64_t result = expr.offset();
    for (int i = 0; i < expr.vars().size(); ++i) {
      if (expr.coeffs(i) == 0) continue;
      if (!VarHasSolutionHint(expr.vars(i))) return std::nullopt;
      result += expr.coeffs(i) * SolutionHint(expr.vars(i));
    }
    return result;
  }

  void UpdateRefSolutionHint(int ref, int hint) {
    UpdateVarSolutionHint(PositiveRef(ref), RefIsPositive(ref) ? hint : -hint);
  }

  // If the given variable is already hinted, updates its hint value.
  // Otherwise, do nothing.
  void UpdateVarSolutionHint(int var, int64_t value) {
    DCHECK(RefIsPositive(var));
    if (!hint_is_loaded_) return;
    if (!hint_has_value_[var]) return;
    hint_[var] = value;
  }

  // Allows to set the hint of a newly created variable.
  void SetNewVariableHint(int var, int64_t value) {
    CHECK(hint_is_loaded_);
    CHECK(!hint_has_value_[var]);
    SetVarHint(var, value);
  }

  // Sets the value of `literal` to "`var`'s value == `value`". Does nothing if
  // `literal` already has a value.
  void MaybeSetLiteralToValueEncoding(int literal, int var, int64_t value);

  // Sets the value of `var` to the value of the given linear expression.
  // `linear` must be a list of (variable index, coefficient) pairs.
  void SetVarToLinearExpression(
      int var, absl::Span<const std::pair<int, int64_t>> linear);

  // Sets the value of `var` to 1 if the value of at least one literal in
  // `clause` is equal to 1 (or to 0 otherwise). `clause` must be a list of
  // literal indices.
  void SetVarToClause(int var, absl::Span<const int> clause);

  // Sets the value of `var` to 1 if the value of all the literals in
  // `conjunction` is 1 (or to 0 otherwise). `conjunction` must be a list of
  // literal indices.
  void SetVarToConjunction(int var, absl::Span<const int> conjunction);

  // Sets the value of `var` to `value` if the value of the given linear
  // expression is not in `domain` (or does nothing otherwise). `linear` must be
  // a list of (variable index, coefficient) pairs.
  void SetVarToValueIfLinearConstraintViolated(
      int var, int64_t value, absl::Span<const std::pair<int, int64_t>> linear,
      const Domain& domain);

  // Sets the value of `literal` to `value` if the value of the given linear
  // expression is not in `domain` (or does nothing otherwise). `linear` must be
  // a list of (variable index, coefficient) pairs.
  void SetLiteralToValueIfLinearConstraintViolated(
      int literal, bool value, absl::Span<const std::pair<int, int64_t>> linear,
      const Domain& domain);

  // Sets the value of `var` to `value` if the value of `condition_lit` is true.
  void SetVarToValueIf(int var, int64_t value, int condition_lit);

  // Sets the value of `literal` to `value` if the value of `condition_lit` is
  // true.
  void SetLiteralToValueIf(int literal, bool value, int condition_lit);

  // If one literal does not have a value, and the other one does, sets the
  // value of the latter to the value of the former. If both literals have a
  // value, sets the value of `lit1` to the value of `lit2`.
  void MakeLiteralsEqual(int lit1, int lit2);

  // Updates the value of the given variable to be within the given domain. The
  // variable is updated to the closest value within the domain. `var` must
  // already have a value.
  void UpdateVarToDomain(int var, const Domain& domain);

  // Updates the value of the given literals to false if their current values
  // are different (or does nothing otherwise).
  void UpdateLiteralsToFalseIfDifferent(int lit1, int lit2);

  // Decrements the value of `lit` and increments the value of `dominating_lit`
  // if their values are equal to 1 and 0, respectively.
  void UpdateLiteralsWithDominance(int lit, int dominating_lit);

  // Decrements the value of `ref` by the minimum amount necessary to be in
  // [`min_value`, `max_value`], and increments the value of one or more
  // `dominating_refs` by the same total amount (or less if it is not possible
  // to exactly match this amount), while staying within their respective
  // domains. The value of a negative reference index r is the opposite of the
  // value of the variable PositiveRef(r).
  //
  // `min_value` must be the minimum value of `ref`'s current domain D, and
  // `max_value` must be in D.
  void UpdateRefsWithDominance(
      int ref, int64_t min_value, int64_t max_value,
      absl::Span<const std::pair<int, Domain>> dominating_refs);

  // Sets the value of `var_y` so that "`var_x`'s value = `var_y`'s value
  // * `coeff` + `offset`". Does nothing if `var_y` already has a value.
  // Returns whether the update was successful.
  bool MaybeSetVarToAffineEquationSolution(int var_x, int var_y, int64_t coeff,
                                           int64_t offset);

  // Sets the value of the variables in `level_vars` and in `circuit` if all the
  // variables in `reservoir` have a value. This assumes that there is one level
  // variable and one circuit node per element in `reservoir` (in the same
  // order) -- plus one last node representing the start and end of the circuit.
  void SetReservoirCircuitVars(const ReservoirConstraintProto& reservoir,
                               int64_t min_level, int64_t max_level,
                               absl::Span<const int> level_vars,
                               const CircuitConstraintProto& circuit);

  // Sets the value of `var` to "`time_i`'s value <= `time_j`'s value &&
  // `active_i`'s value == true && `active_j`'s value == true".
  void SetVarToReifiedPrecedenceLiteral(int var,
                                        const LinearExpressionProto& time_i,
                                        const LinearExpressionProto& time_j,
                                        int active_i, int active_j);

  // Sets the value of `div_var` and `prod_var` if all the variables in the
  // IntMod `ct` constraint have a value, assuming that this "target = x % m"
  // constraint is expanded into "div_var = x / m", "prod_var = div_var * m",
  // and "target = x - prod_var" constraints. If `ct` is not enforced, sets the
  // values of `div_var` and `prod_var` to `default_div_value` and
  // `default_prod_value`, respectively.
  void SetIntModExpandedVars(const ConstraintProto& ct, int div_var,
                             int prod_var, int64_t default_div_value,
                             int64_t default_prod_value);

  // Sets the value of as many variables in `prod_vars` as possible (depending
  // on how many expressions in `int_prod` have a value), assuming that the
  // `int_prod` constraint "target = x_0 * x_1 * ... * x_n" is expanded into
  // "prod_var_1 = x_0 * x1",
  // "prod_var_2 = prod_var_1 * x_2",
  //  ...,
  // "prod_var_(n-1) = prod_var_(n-2) * x_(n-1)",
  // and "target = prod_var_(n-1) * x_n" constraints.
  void SetIntProdExpandedVars(const LinearArgumentProto& int_prod,
                              absl::Span<const int> prod_vars);

  void PermuteVariables(const SparsePermutation& permutation);

 private:
  void SetVarHint(int var, int64_t value) {
    hint_has_value_[var] = true;
    hint_[var] = value;
  }

  void SetLiteralHint(int lit, bool value) {
    SetVarHint(PositiveRef(lit), RefIsPositive(lit) == value ? 1 : 0);
  }

  // This contains all the hinted value or zero if the hint wasn't specified.
  // We try to maintain this as we create new variable.
  bool model_has_hint_ = false;
  bool hint_is_loaded_ = false;
  std::vector<bool> hint_has_value_;
  std::vector<int64_t> hint_;
};

}  // namespace sat
}  // namespace operations_research

#endif  // OR_TOOLS_SAT_SOLUTION_CRUSH_H_
