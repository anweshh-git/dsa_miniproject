#pragma once
#include <QVector>
#include <QMap>
#include <QPair>
#include "core/models/Building.h"
#include "core/models/Path.h"

class Graph {
public:
    Graph();

    void addBuilding(const Building& b);
    void addEdge(int fromId, int toId, double distance);

    const QVector<Building>& buildings() const { return m_buildings; }
    const QVector<Path>& edges() const { return m_edges; }

    // Returns adjacency list: list of (neighbourId, distance)
    QVector<QPair<int, double>> neighbours(int buildingId) const;

    Building* findBuilding(int id);
    const Building* findBuilding(int id) const;

    int buildingCount() const { return m_buildings.size(); }

    // Increment usage count on edges forming the route
    void incrementUsage(const QVector<int>& route);

    void loadDefaults();   // loads sample campus data

private:
    QVector<Building> m_buildings;
    QVector<Path>     m_edges;
    // adjacency: buildingId -> list of (toId, distance)
    QMap<int, QVector<QPair<int, double>>> m_adj;
};
