#include "ParkingManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <limits>

using namespace std;

// ---------------------------------------------------------------------------
// getCurrentTime
// Reads the system clock and formats it as "YYYY-MM-DD HH:MM:SS".
// localtime() fills a tm struct from the current time_t value.
// put_time() formats the tm struct into a stream using the given pattern.
// ---------------------------------------------------------------------------
string ParkingManager::getCurrentTime()
{
    time_t now     = time(nullptr);
    tm*    timeinfo = localtime(&now);

    ostringstream oss;
    oss << put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// ---------------------------------------------------------------------------
// calcDurationMinutes
// Parses two timestamps of the form "YYYY-MM-DD HH:MM:SS".
// Converts both to time_t using mktime(), subtracts, converts to minutes.
// Returns at least 1 minute so a zero-duration exit still has a valid fee.
// ---------------------------------------------------------------------------
int ParkingManager::calcDurationMinutes(const string& entryTime,
                                         const string& exitTime)
{
    auto parseTime = [](const string& s) -> time_t
    {
        tm t = {};
        istringstream ss(s);
        ss >> get_time(&t, "%Y-%m-%d %H:%M:%S");
        t.tm_isdst = -1;   // let mktime decide daylight saving
        return mktime(&t);
    };

    time_t entry = parseTime(entryTime);
    time_t exit_ = parseTime(exitTime);

    double seconds = difftime(exit_, entry);
    int    minutes = static_cast<int>(seconds / 60.0);

    return (minutes < 1) ? 1 : minutes;
}

// ---------------------------------------------------------------------------
// registerVehicle
// Reads owner name, license plate, and vehicle type from stdin.
// Keeps asking for vehicle type until the user enters a valid one.
// ---------------------------------------------------------------------------
Vehicle ParkingManager::registerVehicle()
{
    Vehicle v;

    cout << "\n--- Vehicle Registration ---\n";

    cout << "Owner name    : ";
    getline(cin, v.ownerName);

    cout << "License plate : ";
    getline(cin, v.licensePlate);

    // Validate vehicle type — must be exactly Car, Bike, or EV
    while (true)
    {
        cout << "Vehicle type  : (Car / Bike / EV) : ";
        getline(cin, v.vehicleType);

        if (v.vehicleType == "Car" || v.vehicleType == "Bike" || v.vehicleType == "EV")
            break;

        cout << "  Invalid type. Please enter Car, Bike, or EV.\n";
    }

    return v;
}

// ---------------------------------------------------------------------------
// parkVehicle
// Runs the complete entry flow for one vehicle session.
// Returns false early if no compatible slot is found so main() can inform
// the user without crashing.
// ---------------------------------------------------------------------------
bool ParkingManager::parkVehicle(const Vehicle&  vehicle,
                                  const string&   gateName,
                                  int             startNode,
                                  ParkingGraph&   graph,
                                  BFSNavigator&   navigator,
                                  FileHandler&    fileHandler)
{
    // Step 1 — Run BFS to find the nearest compatible free slot
    BFSResult result = navigator.findNearestSlot(graph, startNode, vehicle.vehicleType);

    if (!result.found)
    {
        cout << "\n  No free " << vehicle.vehicleType
             << " slot available. The lot is full for this vehicle type.\n";
        return false;
    }

    // Step 2 — Display the route from the gate to the allocated slot
    cout << "\n  Nearest slot found : Slot " << result.slotId << "\n";
    cout << "  Route from " << gateName << " : ";
    for (int i = 0; i < static_cast<int>(result.route.size()); i++)
    {
        if (i > 0) cout << " -> ";
        cout << "S" << result.route[i];
    }
    cout << "\n";

    // Step 3 — Mark the slot occupied in the graph
    graph.markOccupied(result.slotId);

    // Step 4 — Build the ticket
    ParkingTicket ticket;
    ticket.vehicle   = vehicle;
    ticket.slotId    = result.slotId;
    ticket.entryTime = getCurrentTime();
    ticket.exitTime  = "";       // filled on exit
    ticket.fee       = 0.0;     // calculated on exit
    ticket.route     = result.route;

    // Step 5 — Store in active sessions and display entry ticket
    activeSessions[vehicle.licensePlate] = ticket;
    displayTicket(ticket);

    // Step 6 — Persist updated slot occupancy to file
    fileHandler.saveSlotState(graph);

    return true;
}

// ---------------------------------------------------------------------------
// exitVehicle
// Looks up the license plate, calculates duration, computes fee, frees the
// slot, saves both files, and removes the session from activeSessions.
// ---------------------------------------------------------------------------
bool ParkingManager::exitVehicle(const string&  licensePlate,
                                  ParkingGraph&  graph,
                                  FeeCalculator& feeCalculator,
                                  FileHandler&   fileHandler)
{
    // Look up the active session by license plate
    auto it = activeSessions.find(licensePlate);
    if (it == activeSessions.end())
    {
        cout << "\n  No active session found for plate: " << licensePlate << "\n";
        return false;
    }

    ParkingTicket& ticket = it->second;

    // Record exit time and compute fee
    ticket.exitTime = getCurrentTime();
    int duration    = calcDurationMinutes(ticket.entryTime, ticket.exitTime);
    ticket.fee      = feeCalculator.calculateFee(ticket.vehicle.vehicleType, duration);

    // Print final receipt
    cout << "\n--- Exit Receipt ---\n";
    displayTicket(ticket);
    cout << "  Duration : " << duration << " minute(s)\n";
    cout << "  Fee paid : Rs. " << fixed << setprecision(2) << ticket.fee << "\n";
    cout << "  Thank you for using Smart Parking!\n";

    // Free the slot in the graph
    graph.markFree(ticket.slotId);

    // Save updated slot state and append to history
    fileHandler.saveSlotState(graph);
    fileHandler.saveToHistory(ticket);

    // Remove from active sessions
    activeSessions.erase(it);

    return true;
}

// ---------------------------------------------------------------------------
// displayParkingStatus
// Prints a simple table of all slots with their type and occupancy.
// Column widths are fixed for consistent alignment.
// ---------------------------------------------------------------------------
void ParkingManager::displayParkingStatus(ParkingGraph& graph)
{
    cout << "\n--- Parking Lot Status ---\n";
    cout << left
         << setw(10) << "Slot ID"
         << setw(12) << "Type"
         << setw(10) << "Status"
         << "\n";
    cout << string(32, '-') << "\n";

    vector<ParkingSlot> slots = graph.getAllSlots();

    for (const ParkingSlot& slot : slots)
    {
        string status = slot.isOccupied ? "Occupied" : "Free";

        cout << left
             << setw(10) << slot.slotId
             << setw(12) << slot.slotType
             << setw(10) << status
             << "\n";
    }

    cout << string(32, '-') << "\n";
}

// ---------------------------------------------------------------------------
// displayTicket
// Prints a formatted parking ticket.
// On entry, fee and exitTime are empty/zero — those lines are skipped cleanly.
// ---------------------------------------------------------------------------
void ParkingManager::displayTicket(const ParkingTicket& ticket)
{
    cout << "\n=============================\n";
    cout << "       PARKING TICKET        \n";
    cout << "=============================\n";
    cout << "Owner         : " << ticket.vehicle.ownerName    << "\n";
    cout << "License Plate : " << ticket.vehicle.licensePlate << "\n";
    cout << "Vehicle Type  : " << ticket.vehicle.vehicleType  << "\n";
    cout << "Slot ID       : " << ticket.slotId               << "\n";
    cout << "Entry Time    : " << ticket.entryTime            << "\n";

    if (!ticket.exitTime.empty())
        cout << "Exit Time     : " << ticket.exitTime << "\n";

    if (ticket.fee > 0.0)
        cout << "Fee           : Rs. " << fixed << setprecision(2) << ticket.fee << "\n";

    cout << "=============================\n";
}

// ---------------------------------------------------------------------------
// hasActiveSession
// Returns true if a session exists for the given license plate.
// ---------------------------------------------------------------------------
bool ParkingManager::hasActiveSession(const string& licensePlate)
{
    return activeSessions.find(licensePlate) != activeSessions.end();
}