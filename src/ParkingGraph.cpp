#include "ParkingGraph.h"
#include <iostream>

// ---------------------------------------------------------------------------
// addSlot
// Appends the slot to the slots vector.
// Also pushes an empty vector into adjList so that both stay in sync —
// adjList[i] always corresponds to slots[i].
// ---------------------------------------------------------------------------
void ParkingGraph::addSlot(const ParkingSlot& slot)
{
    slots.push_back(slot);
    adjList.push_back(std::vector<int>());  // empty neighbour list for this node
}

// ---------------------------------------------------------------------------
// addEdge
// Adds an undirected edge between node a and node b.
// We push b into a's neighbour list AND a into b's neighbour list.
// The range check prevents out-of-bounds access if called with bad IDs.
// ---------------------------------------------------------------------------
void ParkingGraph::addEdge(int a, int b)
{
    int total = static_cast<int>(slots.size());

    if (a < 0 || a >= total || b < 0 || b >= total)
    {
        std::cerr << "addEdge: invalid node IDs " << a << " and " << b << "\n";
        return;
    }

    adjList[a].push_back(b);
    adjList[b].push_back(a);
}

// ---------------------------------------------------------------------------
// addGate
// Stores the mapping: gateName -> startNodeId in the gates map.
// ---------------------------------------------------------------------------
void ParkingGraph::addGate(const std::string& gateName, int startNodeId)
{
    gates[gateName] = startNodeId;
}

// ---------------------------------------------------------------------------
// getGateStartNode
// Looks up the gate name in the map.
// Returns -1 if not found (caller should check for -1 before using it).
// ---------------------------------------------------------------------------
int ParkingGraph::getGateStartNode(const std::string& gateName) const
{
    auto it = gates.find(gateName);
    if (it == gates.end())
    {
        return -1;  // gate not found
    }
    return it->second;
}

// ---------------------------------------------------------------------------
// getSlot
// Returns a COPY of the slot at the given nodeId.
// Safe for read-only access.
// ---------------------------------------------------------------------------
ParkingSlot ParkingGraph::getSlot(int nodeId) const
{
    return slots[nodeId];
}

// ---------------------------------------------------------------------------
// getSlotRef
// Returns a REFERENCE to the slot so the caller can modify it directly.
// Used internally by markOccupied and markFree.
// ---------------------------------------------------------------------------
ParkingSlot& ParkingGraph::getSlotRef(int nodeId)
{
    return slots[nodeId];
}

// ---------------------------------------------------------------------------
// markOccupied
// Sets isOccupied = true for the slot at nodeId.
// Called by ParkingManager when a vehicle is parked.
// ---------------------------------------------------------------------------
void ParkingGraph::markOccupied(int nodeId)
{
    slots[nodeId].isOccupied = true;
}

// ---------------------------------------------------------------------------
// markFree
// Sets isOccupied = false for the slot at nodeId.
// Called by ParkingManager when a vehicle exits.
// ---------------------------------------------------------------------------
void ParkingGraph::markFree(int nodeId)
{
    slots[nodeId].isOccupied = false;
}

// ---------------------------------------------------------------------------
// getNeighbours
// Returns a const reference to the adjacency list entry for nodeId.
// BFSNavigator uses this to explore neighbouring nodes during search.
// ---------------------------------------------------------------------------
const std::vector<int>& ParkingGraph::getNeighbours(int nodeId) const
{
    return adjList[nodeId];
}

// ---------------------------------------------------------------------------
// totalNodes
// Returns the number of nodes in the graph.
// BFSNavigator uses this to create a visited[] array of the right size.
// ---------------------------------------------------------------------------
int ParkingGraph::totalNodes() const
{
    return static_cast<int>(slots.size());
}

// ---------------------------------------------------------------------------
// getGateNames
// Iterates over the gates map and collects all keys (gate names).
// main.cpp uses this to build the gate selection menu dynamically.
// ---------------------------------------------------------------------------
std::vector<std::string> ParkingGraph::getGateNames() const
{
    std::vector<std::string> names;
    for (const auto& entry : gates)
    {
        names.push_back(entry.first);
    }
    return names;
}

// ---------------------------------------------------------------------------
// getAllSlots
// Returns a copy of the entire slots vector.
// ParkingManager uses this when printing the parking status table.
// ---------------------------------------------------------------------------
std::vector<ParkingSlot> ParkingGraph::getAllSlots() const
{
    return slots;
}
