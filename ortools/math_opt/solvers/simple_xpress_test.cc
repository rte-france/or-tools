#include <iostream>
#include <ostream>

#include "absl/log/check.h"
#include "absl/status/statusor.h"
#include "ortools/base/init_google.h"
#include "ortools/math_opt/cpp/math_opt.h"
#include "gtest/gtest.h"

using namespace operations_research::math_opt;

TEST(GScipSolver, XpressTest) {
  // Build the model.
  namespace math_opt = ::operations_research::math_opt;
  math_opt::Model lp_model("getting_started_lp");
  const math_opt::Variable x = lp_model.AddContinuousVariable(-1.0, 1.5, "x");
  const math_opt::Variable y = lp_model.AddContinuousVariable(0.0, 1.0, "y");
  lp_model.AddLinearConstraint(x + y <= 1.5, "c");
  lp_model.Maximize(x + 2 * y);

  // Set parameters, e.g. turn on logging.
  math_opt::SolveArguments args;
  args.parameters.enable_output = true;

  // Solve and ensure an optimal solution was found with no errors.
      const absl::StatusOr<math_opt::SolveResult> result =
      math_opt::Solve(lp_model, math_opt::SolverType::kGscip, args);
  CHECK_OK(result.status());
  CHECK_OK(result->termination.EnsureIsOptimal());

  // Print some information from the result.
  std::cout << "MathOpt solve succeeded" << std::endl;
  std::cout << "Objective value: " << result->objective_value() << std::endl;
  std::cout << "x: " << result->variable_values().at(x) << std::endl;
  std::cout << "y: " << result->variable_values().at(y) << std::endl;
}

int main(int argc, char** argv) {
  absl::SetFlag(&FLAGS_stderrthreshold, 0);
  testing::InitGoogleTest(&argc, argv);
}