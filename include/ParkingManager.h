#ifndef PARKINGMANAGER_H
#define PARKINGMANAGER_H

#include <string>
#include <unordered_map>
#include "Models.h"
#include "ParkingGraph.h"
#include "BFSNavigator.h"
#include "FeeCalculator.h"
#include "FileHandler.h"

using namespace std;

// ---------------------------------------------------------------------------
// ParkingManager
// Orchestrates the complete parking workflow from the driver's perspective.
// Coordinates ParkingGraph, BFSNavigator, FeeCalculator, and FileHandler.
//
// Active sessions are stored in an unordered_map keyed by license plate so
// that looking up a vehicle on exit is O(1).
//
// STL used:
//   unordered_map<string, ParkingTicket> — active session lookup by plate
// ---------------------------------------------------------------------------
class ParkingManager
{
public:

    // -----------------------------------------------------------------------
    // registerVehicle
    // Prompts the driver for their name, license plate, and vehicle type.
    // Validates that vehicleType is one of: Car, Bike, EV.
    // Returns a populated Vehicle struct.
    // -----------------------------------------------------------------------
    Vehicle registerVehicle();

    // -----------------------------------------------------------------------
    // parkVehicle
    // Full entry flow:
    //   1. Runs BFS from the gate's start node to find nearest free slot.
    //   2. Prints the route (gate -> ... -> slot).
    //   3. Marks the slot occupied in the graph.
    //   4. Creates a ParkingTicket with the current time as entry time.
    //   5. Stores the ticket in activeSessions.
    //   6. Saves updated slot state to file.
    // Returns false if no compatible slot is available.
    // -----------------------------------------------------------------------
    bool parkVehicle(const Vehicle&  vehicle,
                     const string&   gateName,
                     int             startNode,
                     ParkingGraph&   graph,
                     BFSNavigator&   navigator,
                     FileHandler&    fileHandler);

    // -----------------------------------------------------------------------
    // exitVehicle
    // Full exit flow:
    //   1. Looks up the license plate in activeSessions.
    //   2. Records exit time.
    //   3. Calculates duration in minutes.
    //   4. Calls FeeCalculator to get the fee.
    //   5. Prints the final ticket with fee.
    //   6. Marks the slot free in the graph.
    //   7. Saves updated slot state and appends record to parking history.
    //   8. Removes the session from activeSessions.
    // Returns false if the license plate is not found in active sessions.
    // -----------------------------------------------------------------------
    bool exitVehicle(const string&  licensePlate,
                     ParkingGraph&  graph,
                     FeeCalculator& feeCalculator,
                     FileHandler&   fileHandler);

    // -----------------------------------------------------------------------
    // displayParkingStatus
    // Prints a table of all slots showing:
    //   Slot ID | Slot Type | Status (Free / Occupied)
    // -----------------------------------------------------------------------
    void displayParkingStatus(ParkingGraph& graph);

    // -----------------------------------------------------------------------
    // displayTicket
    // Prints a formatted parking ticket to the console.
    // Called after parking (entry ticket) and after exit (final receipt).
    // -----------------------------------------------------------------------
    void displayTicket(const ParkingTicket& ticket);

    // -----------------------------------------------------------------------
    // hasActiveSession
    // Returns true if the given license plate currently has a parked session.
    // Used in main.cpp to prevent double-parking the same vehicle.
    // -----------------------------------------------------------------------
    bool hasActiveSession(const string& licensePlate);

private:

    // Active sessions: license plate -> ParkingTicket
    unordered_map<string, ParkingTicket> activeSessions;

    // -----------------------------------------------------------------------
    // getCurrentTime
    // Returns the current system time as a formatted string.
    // Format: "YYYY-MM-DD HH:MM:SS"
    // Used to stamp entry and exit times on tickets.
    // -----------------------------------------------------------------------
    string getCurrentTime();

    // -----------------------------------------------------------------------
    // calcDurationMinutes
    // Parses two "YYYY-MM-DD HH:MM:SS" strings and returns the difference
    // in whole minutes. Used by exitVehicle to compute the fee duration.
    // -----------------------------------------------------------------------
    int calcDurationMinutes(const string& entryTime, const string& exitTime);
};

#endif // PARKINGMANAGER_H