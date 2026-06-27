#ifndef PARKINGGRAPH_H
#define PARKINGGRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Models.h"

// ---------------------------------------------------------------------------
// ParkingGraph
// Stores the parking lot as an undirected unweighted graph.
//
// Nodes  : each ParkingSlot is one node (identified by its slotId / index)
// Edges  : two slots are neighbours if a driver can walk directly between them
// Gates  : named entry points (e.g. "Gate A") mapped to the node ID they
//          connect to. Gates are NOT nodes — they are just starting positions
//          for BFS.
//
// STL used:
//   vector<ParkingSlot>       — stores all slot data indexed by node ID
//   vector<vector<int>>       — adjacency list (adjList[i] = neighbours of i)
//   unordered_map<string,int> — maps gate name -> starting node ID
// ---------------------------------------------------------------------------
class ParkingGraph
{
public:

    // -----------------------------------------------------------------------
    // addSlot
    // Adds one parking slot as a node in the graph.
    // The slot's slotId must equal the current size of the slots vector
    // (i.e. slots are added in order: 0, 1, 2, ...).
    // A matching empty neighbour list is also added to adjList.
    // -----------------------------------------------------------------------
    void addSlot(const ParkingSlot& slot);

    // -----------------------------------------------------------------------
    // addEdge
    // Connects node 'a' and node 'b' bidirectionally.
    // Call this after all slots have been added.
    // Does nothing if either ID is out of range.
    // -----------------------------------------------------------------------
    void addEdge(int a, int b);

    // -----------------------------------------------------------------------
    // addGate
    // Registers a named entry gate connected to a specific node.
    // e.g. addGate("Gate A", 0) means BFS for Gate A starts at node 0.
    // -----------------------------------------------------------------------
    void addGate(const std::string& gateName, int startNodeId);

    // -----------------------------------------------------------------------
    // getGateStartNode
    // Returns the node ID that the given gate connects to.
    // Returns -1 if the gate name is not found.
    // -----------------------------------------------------------------------
    int getGateStartNode(const std::string& gateName) const;

    // -----------------------------------------------------------------------
    // getSlot / getSlotRef
    // getSlot    : returns a copy of the slot at the given node ID.
    // getSlotRef : returns a modifiable reference (used by markOccupied/Free).
    // -----------------------------------------------------------------------
    ParkingSlot  getSlot(int nodeId) const;
    ParkingSlot& getSlotRef(int nodeId);

    // -----------------------------------------------------------------------
    // markOccupied / markFree
    // Updates the isOccupied flag for the slot at the given node ID.
    // Called by ParkingManager when a vehicle parks or leaves.
    // -----------------------------------------------------------------------
    void markOccupied(int nodeId);
    void markFree(int nodeId);

    // -----------------------------------------------------------------------
    // getNeighbours
    // Returns the list of node IDs adjacent to the given node.
    // Used by BFSNavigator to explore the graph.
    // -----------------------------------------------------------------------
    const std::vector<int>& getNeighbours(int nodeId) const;

    // -----------------------------------------------------------------------
    // totalNodes
    // Returns the total number of nodes (slots) in the graph.
    // Used by BFSNavigator to size its visited[] array.
    // -----------------------------------------------------------------------
    int totalNodes() const;

    // -----------------------------------------------------------------------
    // getGateNames
    // Returns all registered gate names.
    // Used by main.cpp to display the gate selection menu.
    // -----------------------------------------------------------------------
    std::vector<std::string> getGateNames() const;

    // -----------------------------------------------------------------------
    // getAllSlots
    // Returns a copy of all slots.
    // Used by DisplayManager (inside ParkingManager) to print status table.
    // -----------------------------------------------------------------------
    std::vector<ParkingSlot> getAllSlots() const;

private:
    std::vector<ParkingSlot>       slots;      // slot data, indexed by node ID
    std::vector<std::vector<int>>  adjList;    // adjList[i] = neighbour IDs of node i
    std::unordered_map<std::string, int> gates; // gate name -> starting node ID
};

#endif // PARKINGGRAPH_H