#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

// ---------------------------------------------------------------------------
// saveSlotState
// Opens slot_state.txt in overwrite mode (not append) so the file always
// reflects the current state, not a growing history.
// Each line: "slotId isOccupied"  where isOccupied is 1 (true) or 0 (false).
// ---------------------------------------------------------------------------
void FileHandler::saveSlotState(ParkingGraph& graph)
{
    ofstream file(SLOT_STATE_FILE);

    if (!file.is_open())
    {
        cerr << "Error: could not open " << SLOT_STATE_FILE << " for writing.\n";
        return;
    }

    vector<ParkingSlot> slots = graph.getAllSlots();

    for (const ParkingSlot& slot : slots)
    {
        // Write slotId and isOccupied (1 or 0) separated by a space
        file << slot.slotId << " " << slot.isOccupied << "\n";
    }

    file.close();
}

// ---------------------------------------------------------------------------
// loadSlotState
// Reads slot_state.txt line by line.
// Each line contains a slotId and a 0/1 flag.
// We call graph.markOccupied or graph.markFree on each node to restore state.
// If the file doesn't exist (first run), ifstream simply fails to open and
// we skip loading — all slots start as free, which is the correct default.
// ---------------------------------------------------------------------------
void FileHandler::loadSlotState(ParkingGraph& graph)
{
    ifstream file(SLOT_STATE_FILE);

    if (!file.is_open())
    {
        // First run — file doesn't exist yet, nothing to load
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        istringstream stream(line);
        int slotId     = 0;
        int isOccupied = 0;

        stream >> slotId >> isOccupied;

        // Bounds check: only update nodes that exist in the graph
        if (slotId < 0 || slotId >= graph.totalNodes())
            continue;

        if (isOccupied == 1)
            graph.markOccupied(slotId);
        else
            graph.markFree(slotId);
    }

    file.close();
}

// ---------------------------------------------------------------------------
// saveToHistory
// Opens parking_history.txt in APPEND mode so previous records are preserved.
// Writes a clearly formatted block for each completed session.
// ---------------------------------------------------------------------------
void FileHandler::saveToHistory(const ParkingTicket& ticket)
{
    ofstream file(HISTORY_FILE, ios::app);

    if (!file.is_open())
    {
        cerr << "Error: could not open " << HISTORY_FILE << " for writing.\n";
        return;
    }

    file << "--- Parking Record ---\n";
    file << "License Plate : " << ticket.vehicle.licensePlate << "\n";
    file << "Owner         : " << ticket.vehicle.ownerName    << "\n";
    file << "Vehicle Type  : " << ticket.vehicle.vehicleType  << "\n";
    file << "Slot ID       : " << ticket.slotId               << "\n";
    file << "Entry Time    : " << ticket.entryTime            << "\n";
    file << "Exit Time     : " << ticket.exitTime             << "\n";
    file << "Fee           : Rs. " << fixed << setprecision(2) << ticket.fee << "\n";
    file << "----------------------\n\n";

    file.close();
}
