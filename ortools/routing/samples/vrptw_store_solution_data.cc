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
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "absl/base/log_severity.h"
#include "absl/log/globals.h"
#include "absl/log/log.h"
#include "ortools/base/init_google.h"
#include "ortools/constraint_solver/constraint_solver.h"
#include "ortools/routing/enums.pb.h"
#include "ortools/routing/index_manager.h"
#include "ortools/routing/parameters.h"
#include "ortools/routing/routing.h"
// [END import]

// [START program_part1]
namespace operations_research::routing {
// [START data_model]
struct DataModel {
  const std::vector<std::vector<int64_t>> time_matrix{
      {0, 6, 9, 8, 7, 3, 6, 2, 3, 2, 6, 6, 4, 4, 5, 9, 7},
      {6, 0, 8, 3, 2, 6, 8, 4, 8, 8, 13, 7, 5, 8, 12, 10, 14},
      {9, 8, 0, 11, 10, 6, 3, 9, 5, 8, 4, 15, 14, 13, 9, 18, 9},
      {8, 3, 11, 0, 1, 7, 10, 6, 10, 10, 14, 6, 7, 9, 14, 6, 16},
      {7, 2, 10, 1, 0, 6, 9, 4, 8, 9, 13, 4, 6, 8, 12, 8, 14},
      {3, 6, 6, 7, 6, 0, 2, 3, 2, 2, 7, 9, 7, 7, 6, 12, 8},
      {6, 8, 3, 10, 9, 2, 0, 6, 2, 5, 4, 12, 10, 10, 6, 15, 5},
      {2, 4, 9, 6, 4, 3, 6, 0, 4, 4, 8, 5, 4, 3, 7, 8, 10},
      {3, 8, 5, 10, 8, 2, 2, 4, 0, 3, 4, 9, 8, 7, 3, 13, 6},
      {2, 8, 8, 10, 9, 2, 5, 4, 3, 0, 4, 6, 5, 4, 3, 9, 5},
      {6, 13, 4, 14, 13, 7, 4, 8, 4, 4, 0, 10, 9, 8, 4, 13, 4},
      {6, 7, 15, 6, 4, 9, 12, 5, 9, 6, 10, 0, 1, 3, 7, 3, 10},
      {4, 5, 14, 7, 6, 7, 10, 4, 8, 5, 9, 1, 0, 2, 6, 4, 8},
      {4, 8, 13, 9, 8, 7, 10, 3, 7, 4, 8, 3, 2, 0, 4, 5, 6},
      {5, 12, 9, 14, 12, 6, 6, 7, 3, 3, 4, 7, 6, 4, 0, 9, 2},
      {9, 10, 18, 6, 8, 12, 15, 8, 13, 9, 13, 3, 4, 5, 9, 0, 9},
      {7, 14, 9, 16, 14, 8, 5, 10, 6, 5, 4, 10, 8, 6, 2, 9, 0},
  };
  const std::vector<std::pair<int64_t, int64_t>> time_windows{
      {0, 5},    // depot
      {7, 12},   // 1
      {10, 15},  // 2
      {16, 18},  // 3
      {10, 13},  // 4
      {0, 5},    // 5
      {5, 10},   // 6
      {0, 4},    // 7
      {5, 10},   // 8
      {0, 3},    // 9
      {10, 16},  // 10
      {10, 15},  // 11
      {0, 5},    // 12
      {5, 10},   // 13
      {7, 8},    // 14
      {10, 15},  // 15
      {11, 15},  // 16
  };
  const int num_vehicles = 4;
  const RoutingIndexManager::NodeIndex depot{0};
};
// [END data_model]

// [START solution_printer]
void PrintSolution(
    const std::vector<std::vector<int>> routes,
    std::vector<std::vector<std::pair<int64_t, int64_t>>> cumul_data) {
  int64_t total_time{0};
  std::ostringstream route;
  for (int vehicle_id = 0; vehicle_id < routes.size(); ++vehicle_id) {
    if (routes[vehicle_id].size() <= 2) {
      continue;
    }
    route << "\nRoute " << vehicle_id << ": \n";

    route << "  " << routes[vehicle_id][0] << " Time("
          << cumul_data[vehicle_id][0].first << ", "
          << cumul_data[vehicle_id][0].second << ") ";
    for (int j = 1; j < routes[vehicle_id].size(); ++j) {
      route << "-> " << routes[vehicle_id][j] << " Time("
            << cumul_data[vehicle_id][j].first << ", "
            << cumul_data[vehicle_id][j].second << ") ";
    }
    route << "\n  Route time: "
          << cumul_data[vehicle_id][routes[vehicle_id].size() - 1].first
          << " minutes\n";

    total_time += cumul_data[vehicle_id][routes[vehicle_id].size() - 1].first;
  }
  route << "\nTotal travel time: " << total_time << " minutes";
  LOG(INFO) << route.str();
}
// [END solution_printer]

// [START get_routes]
std::vector<std::vector<int>> GetRoutes(const Assignment& solution,
                                        const RoutingModel& routing,
                                        const RoutingIndexManager& manager) {
  // Get vehicle routes and store them in a two dimensional array, whose
  // i, j entry is the node for the jth visit of vehicle i.
  std::vector<std::vector<int>> routes(manager.num_vehicles());
  // Get routes.
  for (int vehicle_id = 0; vehicle_id < manager.num_vehicles(); ++vehicle_id) {
    int64_t index = routing.Start(vehicle_id);
    routes[vehicle_id].push_back(manager.IndexToNode(index).value());
    while (!routing.IsEnd(index)) {
      index = solution.Value(routing.NextVar(index));
      routes[vehicle_id].push_back(manager.IndexToNode(index).value());
    }
  }
  return routes;
}
// [END get_routes]

// [START get_cumulative_data]
std::vector<std::vector<std::pair<int64_t, int64_t>>> GetCumulData(
    const Assignment& solution, const RoutingModel& routing,
    const RoutingDimension& dimension) {
  // Returns an array cumul_data, whose i, j entry is a pair containing
  // the minimum and maximum of CumulVar for the dimension.:
  // - cumul_data[i][j].first is the minimum.
  // - cumul_data[i][j].second is the maximum.
  std::vector<std::vector<std::pair<int64_t, int64_t>>> cumul_data(
      routing.vehicles());

  for (int vehicle_id = 0; vehicle_id < routing.vehicles(); ++vehicle_id) {
    int64_t index = routing.Start(vehicle_id);
    IntVar* dim_var = dimension.CumulVar(index);
    cumul_data[vehicle_id].emplace_back(solution.Min(dim_var),
                                        solution.Max(dim_var));
    while (!routing.IsEnd(index)) {
      index = solution.Value(routing.NextVar(index));
      IntVar* dim_var = dimension.CumulVar(index);
      cumul_data[vehicle_id].emplace_back(solution.Min(dim_var),
                                          solution.Max(dim_var));
    }
  }
  return cumul_data;
}
// [END get_cumulative_data]

void VrpTimeWindows() {
  // Instantiate the data problem.
  // [START data]
  DataModel data;
  // [END data]

  // Create Routing Index Manager
  // [START index_manager]
  RoutingIndexManager manager(data.time_matrix.size(), data.num_vehicles,
                              data.depot);
  // [END index_manager]

  // Create Routing Model.
  // [START routing_model]
  RoutingModel routing(manager);
  // [END routing_model]

  // Create and register a transit callback.
  // [START transit_callback]
  const int transit_callback_index = routing.RegisterTransitCallback(
      [&data, &manager](const int64_t from_index,
                        const int64_t to_index) -> int64_t {
        // Convert from routing variable Index to time matrix NodeIndex.
        const int from_node = manager.IndexToNode(from_index).value();
        const int to_node = manager.IndexToNode(to_index).value();
        return data.time_matrix[from_node][to_node];
      });
  // [END transit_callback]

  // Define cost of each arc.
  // [START arc_cost]
  routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);
  // [END arc_cost]

  // Add Time constraint.
  // [START time_constraint]
  const std::string time = "Time";
  routing.AddDimension(transit_callback_index,     // transit callback index
                       /*slack_max*/ int64_t{30},  // allow waiting time
                       /*capacity*/ int64_t{30},   // maximum time per vehicle
                       /*fix_start_cumul_to_zero*/ false, time);
  const RoutingDimension& time_dimension = routing.GetDimensionOrDie(time);
  // Add time window constraints for each location except depot.
  for (int i = 1; i < data.time_windows.size(); ++i) {
    const int64_t index =
        manager.NodeToIndex(RoutingIndexManager::NodeIndex(i));
    time_dimension.CumulVar(index)->SetRange(data.time_windows[i].first,
                                             data.time_windows[i].second);
  }
  // Add time window constraints for each vehicle start node.
  for (int i = 0; i < data.num_vehicles; ++i) {
    const int64_t index = routing.Start(i);
    time_dimension.CumulVar(index)->SetRange(data.time_windows[0].first,
                                             data.time_windows[0].second);
  }
  // [END time_constraint]

  // Instantiate route start and end times to produce feasible times.
  // [START depot_start_end_times]
  for (int i = 0; i < data.num_vehicles; ++i) {
    routing.AddVariableMinimizedByFinalizer(
        time_dimension.CumulVar(routing.Start(i)));
    routing.AddVariableMinimizedByFinalizer(
        time_dimension.CumulVar(routing.End(i)));
  }
  // [END depot_start_end_times]

  // Setting first solution heuristic.
  // [START parameters]
  RoutingSearchParameters searchParameters = DefaultRoutingSearchParameters();
  searchParameters.set_first_solution_strategy(
      FirstSolutionStrategy::PATH_CHEAPEST_ARC);
  // [END parameters]

  // Solve the problem.
  // [START solve]
  const Assignment* solution = routing.SolveWithParameters(searchParameters);
  // [END solve]

  // Print solution on console.
  // [START print_solution]
  PrintSolution(GetRoutes(*solution, routing, manager),
                GetCumulData(*solution, routing, time_dimension));
  // [END print_solution]
}
}  // namespace operations_research::routing

int main(int argc, char* argv[]) {
  InitGoogle(argv[0], &argc, &argv, true);
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);
  operations_research::routing::VrpTimeWindows();
  return EXIT_SUCCESS;
}
// [END program_part1]
// [END program]
