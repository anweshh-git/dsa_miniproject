#pragma once
#include "Graph.h"
#include "core/models/Path.h"
#include <QVector>
#include <QMap>

class Dijkstra {
public:
    explicit Dijkstra(const Graph& graph);

    RouteResult shortestPath(int sourceId, int destId);

    QMap<int, double> allDistances() const { return m_dist; }

private:
    const Graph& m_graph;
    QMap<int, double> m_dist;
    QMap<int, int>    m_prev;

    void run(int sourceId);
    QVector<int> reconstructPath(int destId);
};
