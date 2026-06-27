#ifndef BFSNAVIGATOR_H
#define BFSNAVIGATOR_H

#include <string>
#include <vector>
#include "Models.h"
#include "ParkingGraph.h"

using namespace std;

// ---------------------------------------------------------------------------
// BFSNavigator
// Searches the parking graph using Breadth-First Search to find the nearest
// compatible free slot for a given vehicle type.
//
// "Nearest" means fewest hops from the starting node — BFS guarantees this
// on an unweighted graph because it explores layer by layer.
//
// A slot is "compatible" if:
//   1. Its slotType matches the vehicleType (Car->Car, Bike->Bike, EV->EV)
//   2. Its isOccupied flag is false
//
// STL used:
//   queue<int>   — BFS frontier (FIFO order ensures nearest-first traversal)
//   vector<bool> — visited flags, one per node, to avoid revisiting
//   vector<int>  — parent array for path reconstruction
// ---------------------------------------------------------------------------
class BFSNavigator
{
public:

    // -----------------------------------------------------------------------
    // findNearestSlot
    // Runs BFS from 'startNode' across the given graph.
    // Skips any node whose slot is occupied OR whose type doesn't match
    // 'vehicleType'. Returns a BFSResult with:
    //   found  = true  and the slotId + route if a compatible slot was found
    //   found  = false and slotId = -1        if the lot is full for that type
    //
    // The route vector contains node IDs in order from startNode to the
    // allocated slot (both endpoints inclusive).
    // -----------------------------------------------------------------------
    BFSResult findNearestSlot(ParkingGraph& graph,
                              int           startNode,
                              const string& vehicleType);

private:

    // -----------------------------------------------------------------------
    // reconstructPath
    // Walks backwards through the parent[] array from 'targetNode' to
    // 'startNode', then reverses the result so the path reads start -> target.
    // Called internally by findNearestSlot once the target is located.
    // -----------------------------------------------------------------------
    vector<int> reconstructPath(const vector<int>& parent,
                                int                startNode,
                                int                targetNode);
};

#endif // BFSNAVIGATOR_H