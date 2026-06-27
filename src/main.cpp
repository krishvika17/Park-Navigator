#include <iostream>
#include <vector>
#include <limits>

#include "Models.h"
#include "ParkingGraph.h"
#include "BFSNavigator.h"
#include "FeeCalculator.h"
#include "FileHandler.h"
#include "ParkingManager.h"

using namespace std;

int main()
{
    // ---------------------------------------------------------------------
    // Create core objects
    // ---------------------------------------------------------------------
    ParkingGraph graph;
    BFSNavigator navigator;
    FeeCalculator feeCalculator;
    FileHandler fileHandler;
    ParkingManager parkingManager;

    // ---------------------------------------------------------------------
    // Create parking slots
    // Slot IDs : 0 - 11
    //
    // 0-4   -> Car
    // 5-8   -> Bike
    // 9-11  -> EV
    // ---------------------------------------------------------------------

    graph.addSlot({0, "Car", false});
    graph.addSlot({1, "Car", false});
    graph.addSlot({2, "Car", false});
    graph.addSlot({3, "Car", false});
    graph.addSlot({4, "Car", false});

    graph.addSlot({5, "Bike", false});
    graph.addSlot({6, "Bike", false});
    graph.addSlot({7, "Bike", false});
    graph.addSlot({8, "Bike", false});

    graph.addSlot({9, "EV", false});
    graph.addSlot({10, "EV", false});
    graph.addSlot({11, "EV", false});

    // ---------------------------------------------------------------------
    // Parking lot layout (graph)
    //
    // Gate A
    //    |
    //    0 ---- 1 ---- 2 ---- 3 ---- 4
    //           |      |
    //           5      9
    //           |      |
    //           6      10
    //           |      |
    //           7 ---- 11
    //           |
    //           8
    //
    // Gate B -> node 4
    // Gate C -> node 8
    // ---------------------------------------------------------------------

    graph.addEdge(0,1);
    graph.addEdge(1,2);
    graph.addEdge(2,3);
    graph.addEdge(3,4);

    graph.addEdge(1,5);
    graph.addEdge(5,6);
    graph.addEdge(6,7);
    graph.addEdge(7,8);

    graph.addEdge(2,9);
    graph.addEdge(9,10);
    graph.addEdge(10,11);
    graph.addEdge(7,11);

    // ---------------------------------------------------------------------
    // Register gates
    // ---------------------------------------------------------------------

    graph.addGate("Gate A",0);
    graph.addGate("Gate B",4);
    graph.addGate("Gate C",8);

    // ---------------------------------------------------------------------
    // Restore previous parking state
    // ---------------------------------------------------------------------

    fileHandler.loadSlotState(graph);

    cout << "=========================================\n";
    cout << "     SMART PARKING MANAGEMENT SYSTEM\n";
    cout << "=========================================\n";

    int choice;

    while(true)
    {
        cout << "\n";
        cout << "========== MAIN MENU ==========\n";
        cout << "1. Park Vehicle\n";
        cout << "2. Exit Vehicle\n";
        cout << "3. View Parking Status\n";
        cout << "4. Exit Program\n";
        cout << "===============================\n";

        cout << "Enter your choice : ";

        cin >> choice;

        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nInvalid input.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(choice)
        {
            case 1:
            {
                Vehicle vehicle = parkingManager.registerVehicle();

                if(parkingManager.hasActiveSession(vehicle.licensePlate))
                {
                    cout << "\nVehicle is already parked.\n";
                    break;
                }

                cout << "\nChoose Entry Gate\n";
                cout << "1. Gate A\n";
                cout << "2. Gate B\n";
                cout << "3. Gate C\n";

                int gateChoice;

                cout << "Enter choice : ";
                cin >> gateChoice;

                if(cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "\nInvalid gate selection.\n";
                    break;
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                string gateName;

                if(gateChoice == 1)
                    gateName = "Gate A";
                else if(gateChoice == 2)
                    gateName = "Gate B";
                else if(gateChoice == 3)
                    gateName = "Gate C";
                else
                {
                    cout << "\nInvalid gate.\n";
                    break;
                }

                int startNode = graph.getGateStartNode(gateName);

                parkingManager.parkVehicle(
                    vehicle,
                    gateName,
                    startNode,
                    graph,
                    navigator,
                    fileHandler
                );

                break;
            }
                        case 2:
            {
                string licensePlate;

                cout << "\nEnter License Plate : ";
                getline(cin, licensePlate);

                parkingManager.exitVehicle(
                    licensePlate,
                    graph,
                    feeCalculator,
                    fileHandler
                );

                break;
            }

            case 3:
            {
                parkingManager.displayParkingStatus(graph);
                break;
            }

            case 4:
            {
                cout << "\nSaving parking data...\n";
                fileHandler.saveSlotState(graph);

                cout << "Thank you for using Smart Parking Management System!\n";
                return 0;
            }

            default:
            {
                cout << "\nInvalid choice. Please try again.\n";
                break;
            }
        }
    }

    return 0;
}