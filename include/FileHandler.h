#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include "Models.h"
#include "ParkingGraph.h"
using namespace std;

// File paths — defined here so both .h and .cpp agree on the same names
const string HISTORY_FILE   = "data/parking_history.txt";
const string SLOT_STATE_FILE = "data/slot_state.txt";

// ---------------------------------------------------------------------------
// FileHandler
// Single responsibility: all disk I/O for the parking system.
//
// Two files managed:
//   slot_state.txt      — records which slots are occupied (overwritten each
//                         time a vehicle parks or exits, read once on startup)
//   parking_history.txt — append-only log of every completed parking session
//
// STL / library used:
//   ifstream  — reading files
//   ofstream  — writing files
//   string    — line-by-line file data
// ---------------------------------------------------------------------------
class FileHandler
{
public:

    // -----------------------------------------------------------------------
    // saveSlotState
    // Writes the current occupied/free status of every slot to slot_state.txt.
    // Format per line:  slotId isOccupied
    // Example:          0 0
    //                   1 1
    //                   2 0
    // Called after every park and exit event so restarts see correct state.
    // -----------------------------------------------------------------------
    void saveSlotState(ParkingGraph& graph);

    // -----------------------------------------------------------------------
    // loadSlotState
    // Reads slot_state.txt and restores each slot's isOccupied flag.
    // Called once in main() before the menu loop starts.
    // If the file does not exist yet (first run), does nothing safely.
    // -----------------------------------------------------------------------
    void loadSlotState(ParkingGraph& graph);

    // -----------------------------------------------------------------------
    // saveToHistory
    // Appends one completed session record to parking_history.txt.
    // Called by ParkingManager when a vehicle exits.
    // Format:
    //   --- Parking Record ---
    //   License Plate : DL01AB1234
    //   Owner         : Rahul Singh
    //   Vehicle Type  : Car
    //   Slot ID       : 3
    //   Entry Time    : 2025-08-01 10:30:00
    //   Exit Time     : 2025-08-01 12:45:00
    //   Fee           : Rs. 15.00
    //   ----------------------
    // -----------------------------------------------------------------------
    void saveToHistory(const ParkingTicket& ticket);
};

#endif // FILEHANDLER_H