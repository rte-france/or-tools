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

// Wrapper for RoutingIndexManager.

%include "ortools/base/base.i"
%include "ortools/routing/java/types.i"

%{
#include "ortools/routing/index_manager.h"
%}

DEFINE_INDEX_TYPE_TYPEDEF(operations_research::routing::RoutingNodeIndex,
                          operations_research::routing::RoutingIndexManager::NodeIndex);

%ignoreall

%unignore operations_research::routing;
namespace operations_research::routing {

%unignore RoutingIndexManager;
%unignore RoutingIndexManager::~RoutingIndexManager;
%unignore RoutingIndexManager::RoutingIndexManager(
    int, int,
    NodeIndex);
%unignore RoutingIndexManager::RoutingIndexManager(
    int, int,
    const std::vector<NodeIndex>&,
    const std::vector<NodeIndex>&);
%rename (getStartIndex) RoutingIndexManager::GetStartIndex;
%rename (getEndIndex) RoutingIndexManager::GetEndIndex;
%rename (getNumberOfNodes) RoutingIndexManager::num_nodes;
%rename (getNumberOfVehicles) RoutingIndexManager::num_vehicles;
%rename (getNumberOfIndices) RoutingIndexManager::num_indices;
%rename (indexToNode) RoutingIndexManager::IndexToNode;
%rename (nodeToIndex) RoutingIndexManager::NodeToIndex;
%rename (nodesToIndices) RoutingIndexManager::NodesToIndices;

}  // namespace operations_research::routing

%include "ortools/routing/index_manager.h"

%unignoreall
