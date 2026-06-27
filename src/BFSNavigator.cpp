#include "BFSNavigator.h"
#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// ---------------------------------------------------------------------------
// findNearestSlot
//
// Step-by-step walkthrough of the algorithm:
//
//  1. Create a visited[] array of size graph.totalNodes(), all false.
//  2. Create a parent[] array of size graph.totalNodes(), all -1.
//     parent[i] stores which node we came FROM when we first reached node i.
//  3. Push startNode into the queue and mark it visited.
//  4. Loop while the queue is not empty:
//       a. Pop the front node (call it 'current').
//       b. Read its ParkingSlot from the graph.
//       c. Check compatibility:
//            - slotType matches vehicleType
//            - isOccupied is false
//          If both are true, this is our target — reconstruct and return.
//       d. Otherwise, push all unvisited neighbours into the queue,
//          set their parent to 'current', and mark them visited.
//  5. If the queue empties with no match, return found = false.
//
// Why do we check the node AFTER popping (not before pushing)?
// Because the start node itself might be a valid slot. If we only checked
// when pushing neighbours, we would skip the very first node.
// ---------------------------------------------------------------------------
BFSResult BFSNavigator::findNearestSlot(ParkingGraph& graph,
                                         int           startNode,
                                         const string& vehicleType)
{
    BFSResult result;
    result.found  = false;
    result.slotId = -1;

    int total = graph.totalNodes();

    // visited[i] = true once node i has been added to the queue
    vector<bool> visited(total, false);

    // parent[i] = the node we came from when we first reached node i
    // Used to reconstruct the path after finding the target
    vector<int> parent(total, -1);

    queue<int> bfsQueue;

    // Start BFS from the gate's connected node
    bfsQueue.push(startNode);
    visited[startNode] = true;

    while (!bfsQueue.empty())
    {
        int current = bfsQueue.front();
        bfsQueue.pop();

        // Read this node's slot data from the graph
        ParkingSlot slot = graph.getSlot(current);

        // Check if this slot is compatible with the vehicle type
        bool typeMatches = (slot.slotType == vehicleType);
        bool slotIsFree  = !slot.isOccupied;

        if (typeMatches && slotIsFree)
        {
            // Found the nearest compatible free slot
            result.found  = true;
            result.slotId = current;
            result.route  = reconstructPath(parent, startNode, current);
            return result;
        }

        // Not a match — explore this node's neighbours
        for (int neighbour : graph.getNeighbours(current))
        {
            if (!visited[neighbour])
            {
                visited[neighbour]  = true;
                parent[neighbour]   = current;
                bfsQueue.push(neighbour);
            }
        }
    }

    // Queue is empty — no compatible free slot was found
    return result;
}

// ---------------------------------------------------------------------------
// reconstructPath
//
// After BFS finds the target node, we trace backwards through parent[]:
//
//   targetNode -> parent[targetNode] -> parent[parent[targetNode]] -> ...
//                                                           -> startNode
//
// This gives us the path in REVERSE order, so we reverse it at the end.
//
// Example: startNode=0, targetNode=4, parent = [-1, 0, 1, 1, 2]
//   Walk: 4 -> parent[4]=2 -> parent[2]=1 -> parent[1]=0 -> parent[0]=-1 (stop)
//   Raw:  [4, 2, 1, 0]
//   After reverse: [0, 1, 2, 4]  <- correct forward path
// ---------------------------------------------------------------------------
vector<int> BFSNavigator::reconstructPath(const vector<int>& parent,
                                           int                startNode,
                                           int                targetNode)
{
    vector<int> path;
    int current = targetNode;

    // Walk backwards from target to start using the parent array
    while (current != -1)
    {
        path.push_back(current);
        if (current == startNode)
            break;
        current = parent[current];
    }

    // Path is currently [target, ..., start] — reverse to get [start, ..., target]
    reverse(path.begin(), path.end());

    return path;
}