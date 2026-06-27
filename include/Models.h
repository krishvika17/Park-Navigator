#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Vehicle
// Represents the driver's vehicle entering the parking lot.
// vehicleType must be exactly "Car", "Bike", or "EV" — used to match slots.
// ---------------------------------------------------------------------------
struct Vehicle
{
    std::string licensePlate;   // e.g. "DL01AB1234"
    std::string ownerName;      // e.g. "Rahul Singh"
    std::string vehicleType;    // "Car", "Bike", or "EV"
};

// ---------------------------------------------------------------------------
// ParkingSlot
// Represents one node in the parking graph.
// slotType must match vehicleType for a slot to be compatible:
//   Car  -> "Car"  slot
//   Bike -> "Bike" slot
//   EV   -> "EV"   slot
// ---------------------------------------------------------------------------
struct ParkingSlot
{
    int         slotId;         // unique node ID in the graph (0-indexed)
    std::string slotType;       // "Car", "Bike", or "EV"
    bool        isOccupied;     // true = taken, false = free
};

// ---------------------------------------------------------------------------
// ParkingTicket
// Generated when a vehicle is parked. Holds the full session record.
// route stores the node IDs from the gate's start node to the allocated slot.
// ---------------------------------------------------------------------------
struct ParkingTicket
{
    Vehicle          vehicle;       // who parked
    int              slotId;        // which slot was allocated
    std::string      entryTime;     // recorded when parked
    std::string      exitTime;      // recorded when leaving (empty until exit)
    double           fee;           // calculated on exit (0.0 until exit)
    std::vector<int> route;         // path: [startNode, ..., slotId]
};

// ---------------------------------------------------------------------------
// BFSResult
// Returned by BFSNavigator after a search.
// found = false means no compatible free slot exists (lot is full for that type).
// When found = true, slotId holds the allocated slot and route holds the path.
// ---------------------------------------------------------------------------
struct BFSResult
{
    bool             found;     // was a compatible free slot found?
    int              slotId;    // the allocated slot node ID (-1 if not found)
    std::vector<int> route;     // node IDs from start to slotId (inclusive)
};

#endif // MODELS_H