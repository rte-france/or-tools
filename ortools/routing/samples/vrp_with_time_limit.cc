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

// [START program]
// [START import]
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <sstream>

#include "absl/base/log_severity.h"
#include "absl/log/globals.h"
#include "absl/log/log.h"
#include "google/protobuf/duration.pb.h"
#include "ortools/base/init_google.h"
#include "ortools/constraint_solver/constraint_solver.h"
#include "ortools/routing/enums.pb.h"
#include "ortools/routing/index_manager.h"
#include "ortools/routing/parameters.h"
#include "ortools/routing/routing.h"
// [END import]

namespace operations_research::routing {
//! @brief Print the solution.
//! @param[in] manager Index manager used.
//! @param[in] routing Routing solver used.
//! @param[in] solution Solution found by the solver.
// [START solution_printer]
void PrintSolution(const RoutingIndexManager& manager,
                   const RoutingModel& routing, const Assignment& solution) {
  int64_t max_route_distance = 0;
  for (int vehicle_id = 0; vehicle_id < manager.num_vehicles(); ++vehicle_id) {
    if (!routing.IsVehicleUsed(solution, vehicle_id)) {
      continue;
    }
    int64_t index = routing.Start(vehicle_id);
    LOG(INFO) << "Route for Vehicle " << vehicle_id << ":";
    int64_t route_distance = 0;
    std::stringstream route;
    while (!routing.IsEnd(index)) {
      route << manager.IndexToNode(index).value() << " -> ";
      const int64_t previous_index = index;
      index = solution.Value(routing.NextVar(index));
      route_distance += const_cast<RoutingModel&>(routing).GetArcCostForVehicle(
          previous_index, index, int64_t{vehicle_id});
    }
    LOG(INFO) << route.str() << manager.IndexToNode(index).value();
    LOG(INFO) << "Distance of the route: " << route_distance << "m";
    max_route_distance = std::max(route_distance, max_route_distance);
  }
  LOG(INFO) << "Maximum of the route distances: " << max_route_distance << "m";
  LOG(INFO) << "";
  LOG(INFO) << "Problem solved in " << routing.solver()->wall_time() << "ms";
}
// [END solution_printer]

void VrpGlobalSpan() {
  // Instantiate the data problem.
  // [START data]
  const int num_locations = 20;
  const int num_vehicles = 5;
  const RoutingIndexManager::NodeIndex depot{0};
  // [END data]

  // Create Routing Index Manager
  // [START index_manager]
  RoutingIndexManager manager(num_locations, num_vehicles, depot);
  // [END index_manager]

  // Create Routing Model.
  // [START routing_model]
  RoutingModel routing(manager);
  // [END routing_model]

  // Create and register a transit callback.
  // [START transit_callback]
  const int transit_callback_index = routing.RegisterTransitCallback(
      [](int64_t from_index, int64_t to_index) -> int64_t {
        (void)from_index;
        (void)to_index;
        return 1;
      });
  // [END transit_callback]

  // Define cost of each arc.
  // [START arc_cost]
  routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);
  // [END arc_cost]

  // Add Distance constraint.
  // [START distance_constraint]
  routing.AddDimension(transit_callback_index,
                       /*slack_max=*/0,
                       /*capacity=*/3000,
                       /*fix_start_cumul_to_zero=*/true, "Distance");
  const RoutingDimension& distance_dimension =
      routing.GetDimensionOrDie("Distance");
  const_cast<RoutingDimension&>(distance_dimension)
      .SetGlobalSpanCostCoefficient(100);
  // [END distance_constraint]

  // Setting first solution heuristic.
  // [START parameters]
  RoutingSearchParameters search_parameters = DefaultRoutingSearchParameters();
  search_parameters.set_first_solution_strategy(
      FirstSolutionStrategy::PATH_CHEAPEST_ARC);
  search_parameters.set_local_search_metaheuristic(
      LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH);
  search_parameters.set_log_search(true);
  search_parameters.mutable_time_limit()->set_seconds(5);
  // [END parameters]

  // Solve the problem.
  // [START solve]
  const Assignment* solution = routing.SolveWithParameters(search_parameters);
  // [END solve]

  // Print solution on console.
  // [START print_solution]
  PrintSolution(manager, routing, *solution);
  // [END print_solution]
}
}  // namespace operations_research::routing

int main(int argc, char* argv[]) {
  InitGoogle(argv[0], &argc, &argv, true);
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);
  operations_research::routing::VrpGlobalSpan();
  return EXIT_SUCCESS;
}
// [END program]
