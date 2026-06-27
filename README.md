# Smart Parking Management System

## Overview

Smart Parking Management System is a console-based C++ application that simulates the operation of a parking facility. The system allocates the nearest compatible parking slot using the Breadth-First Search (BFS) algorithm on a graph-based parking layout. It also manages vehicle entry and exit, parking fee calculation, parking status, and persistent storage of parking records.

---

## Features

- Register vehicles (Car, Bike, EV)
- Multiple entry gates
- Nearest slot allocation using BFS
- Vehicle type-based slot allocation
- Parking ticket generation
- Parking fee calculation
- Parking status display
- Vehicle exit and slot release
- File-based data persistence

---

## Technologies Used

- C++17
- Object-Oriented Programming
- Graph (Adjacency List)
- Breadth-First Search (BFS)
- STL (`vector`, `queue`, `unordered_map`)
- File Handling (`fstream`)

---

## Project Structure

```
Smart-Parking-Management-System
│
├── include/
├── src/
├── data/
├── docs/
├── README.md
└── .gitignore
```

---

## Working

1. Register a vehicle and select an entry gate.
2. BFS searches the graph to locate the nearest compatible free parking slot.
3. The selected slot is allocated and a parking ticket is generated.
4. On exit, the parking duration and fee are calculated.
5. The parking slot is released and the parking history is updated.

---

## Sample Output

The `docs/screenshots` directory contains sample outputs for:

- Vehicle registration
- Nearest slot allocation
- Parking ticket
- Parking status
- Exit receipt

---

## Build and Run

Compile:

```bash
g++ -std=c++17 -Iinclude src/*.cpp -o build/parking
```

Run:

```bash
./build/parking
```

---

## Concepts Demonstrated

- Object-Oriented Programming
- Graph Representation
- Breadth-First Search
- Queue-based Traversal
- Hash Map (`unordered_map`)
- File Handling
- Modular Software Design

---

## Future Scope

- Graphical user interface
- Database integration
- Online parking reservation
- Configurable parking layout

---

## Author

**Krishvika**
