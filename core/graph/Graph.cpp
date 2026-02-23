#include "Graph.h"
#include <QDebug>

Graph::Graph() {}

void Graph::addBuilding(const Building& b) {
    m_buildings.append(b);
    if (!m_adj.contains(b.id))
        m_adj[b.id] = {};
}

void Graph::addEdge(int fromId, int toId, double distance) {
    Path p(fromId, toId, distance);
    m_edges.append(p);
    m_adj[fromId].append({toId, distance});
    m_adj[toId].append({fromId, distance}); // undirected
}

QVector<QPair<int, double>> Graph::neighbours(int buildingId) const {
    return m_adj.value(buildingId);
}

Building* Graph::findBuilding(int id) {
    for (auto& b : m_buildings)
        if (b.id == id) return &b;
    return nullptr;
}

const Building* Graph::findBuilding(int id) const {
    for (const auto& b : m_buildings)
        if (b.id == id) return &b;
    return nullptr;
}

void Graph::incrementUsage(const QVector<int>& route) {
    for (int i = 0; i + 1 < route.size(); ++i) {
        int a = route[i], b = route[i+1];
        for (auto& e : m_edges) {
            if ((e.from == a && e.to == b) || (e.from == b && e.to == a))
                e.usageCount++;
        }
    }
}

void Graph::loadDefaults() {
    // 12 sample campus buildings
    addBuilding({1,  "Main Gate",          "GATE", 100, 500, "Main entrance to campus"});
    addBuilding({2,  "Admin Block",        "ADM",  250, 480, "Administrative offices"});
    addBuilding({3,  "Library",            "LIB",  400, 350, "Central campus library"});
    addBuilding({4,  "CSE Department",     "CSE",  550, 280, "Computer Science & Engineering"});
    addBuilding({5,  "ECE Department",     "ECE",  550, 450, "Electronics & Communication"});
    addBuilding({6,  "Mechanical Dept",    "MECH", 400, 550, "Mechanical Engineering"});
    addBuilding({7,  "Cafeteria",          "CAFE", 300, 480, "Main cafeteria & food court"});
    addBuilding({8,  "Hostel A",           "HOSA", 700, 200, "Boys hostel block A"});
    addBuilding({9,  "Hostel B",           "HOSB", 700, 400, "Girls hostel block B"});
    addBuilding({10, "Sports Complex",     "SPORT",600, 600, "Indoor & outdoor sports"});
    addBuilding({11, "Auditorium",         "AUD",  350, 200, "Main auditorium"});
    addBuilding({12, "Medical Centre",     "MED",  200, 300, "Campus health centre"});

    // Edges (fromId, toId, distanceMetres)
    addEdge(1,  2,  150);
    addEdge(1,  7,  200);
    addEdge(2,  3,  180);
    addEdge(2,  7,  100);
    addEdge(2,  12, 170);
    addEdge(3,  4,  160);
    addEdge(3,  5,  200);
    addEdge(3,  11, 190);
    addEdge(3,  7,  120);
    addEdge(4,  8,  200);
    addEdge(4,  5,  180);
    addEdge(5,  6,  150);
    addEdge(5,  9,  210);
    addEdge(6,  10, 160);
    addEdge(6,  7,  170);
    addEdge(8,  9,  200);
    addEdge(9,  10, 230);
    addEdge(10, 6,  160);
    addEdge(11, 4,  220);
    addEdge(12, 11, 200);
}
