# Enhanced Campus Map — DSA Mini Project

A Qt6 C++ application demonstrating key DSA concepts:
- **Chapter 3 — Queue**: Circular array queue for shuttle bus stop simulation
- **Chapter 4 — Linked List**: Singly linked list (route history) + Doubly linked list (visit log)
- **Chapter 7 — Sorting**: Bubble Sort, Insertion Sort, Selection Sort (all manual, no std::sort)
- **Chapter 8 — Searching & Hashing**: Hash table with chaining + Binary search with step visualisation

---

## Build Requirements
- Qt 6.x (Widgets module)
- CMake 3.16+
- C++17 compiler (GCC / Clang / MSVC)

---

## Build & Run

```bash
mkdir build && cd build
cmake ..
cmake --build .
./CampusMap          # Linux/Mac
CampusMap.exe        # Windows
```

---

## Project Structure

```
CampusMap/
├── CMakeLists.txt
├── main.cpp
├── core/
│   ├── models/
│   │   ├── Building.h          # Building data struct
│   │   └── Path.h              # Path/Edge + RouteResult structs
│   ├── graph/
│   │   ├── Graph.h/.cpp        # Adjacency list graph with 12 buildings
│   │   └── Dijkstra.h/.cpp     # Shortest path (manual, no STL priority_queue)
│   ├── queue/
│   │   └── ShuttleQueue.h/.cpp # Ch3: Circular array queue
│   ├── linkedlist/
│   │   ├── PathHistory.h/.cpp  # Ch4: Singly linked list
│   │   └── VisitLog.h/.cpp     # Ch4: Doubly linked list
│   ├── sorting/
│   │   └── BuildingSorter.h/.cpp # Ch7: Bubble, Insertion, Selection sort
│   └── searching/
│       ├── HashTable.h/.cpp    # Ch8: Hash table with chaining (size=31)
│       └── BinarySearch.h/.cpp # Ch8: Binary search with step trace
└── ui/
    ├── MainWindow.h/.cpp       # Main window, wires everything together
    ├── MapView.h/.cpp          # QGraphicsView campus map with route animation
    └── panels/
        ├── SearchPanel.h/.cpp  # Hash search + Binary search UI
        ├── ShuttlePanel.h/.cpp # Queue simulation UI (auto-sim with QTimer)
        ├── HistoryPanel.h/.cpp # Linked list display UI
        └── SortPanel.h/.cpp    # Sorting results UI
```

---

## How to Use

### Finding a Route (Graph + Dijkstra)
1. Click any building on the map to select it (it highlights orange)
2. Click another building — shortest path animates in red

### Search Tab
- **Hash Table**: Type a building name (e.g. "Library") or code (e.g. "CSE") → instant O(1) lookup
- **Binary Search**: Enter a building ID (1–12) → see step-by-step comparisons table

### Shuttle Tab (Queue)
- Select a bus stop from the dropdown
- Type a student name and destination, click **Enqueue**
- Click **Board (Dequeue)** to remove the front student
- Click **Auto Simulate** to watch random enqueue/dequeue operations live

### History Tab (Linked List)
- **Route History**: Every route searched is prepended to a singly linked list
- **Visit Log**: Every building clicked is appended to a doubly linked list
- You can remove the last visit (tail removal) or clear all

### Sort Tab
- **Distance sort**: Enter X,Y of your current location → buildings sorted by pixel distance (Bubble Sort)
- **Popularity sort**: Paths sorted by how many times each route used them (Insertion Sort)
- **Name sort**: Alphabetical A–Z (Selection Sort)

---

## Sample Campus Buildings

| ID | Name | Code |
|----|------|------|
| 1  | Main Gate | GATE |
| 2  | Admin Block | ADM |
| 3  | Library | LIB |
| 4  | CSE Department | CSE |
| 5  | ECE Department | ECE |
| 6  | Mechanical Dept | MECH |
| 7  | Cafeteria | CAFE |
| 8  | Hostel A | HOSA |
| 9  | Hostel B | HOSB |
| 10 | Sports Complex | SPORT |
| 11 | Auditorium | AUD |
| 12 | Medical Centre | MED |
