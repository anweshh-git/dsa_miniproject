#include "Dijkstra.h"
#include <limits>
#include <QDebug>

static const double INF = std::numeric_limits<double>::infinity();

Dijkstra::Dijkstra(const Graph& graph) : m_graph(graph) {}

void Dijkstra::run(int sourceId) {
    m_dist.clear();
    m_prev.clear();

    // Initialise all distances to INF
    for (const auto& b : m_graph.buildings()) {
        m_dist[b.id] = INF;
        m_prev[b.id] = -1;
    }
    m_dist[sourceId] = 0.0;

    // visited set
    QMap<int, bool> visited;
    for (const auto& b : m_graph.buildings())
        visited[b.id] = false;

    int n = m_graph.buildingCount();

    for (int iter = 0; iter < n; ++iter) {
        // Pick unvisited node with minimum distance (manual min-find)
        int u = -1;
        double minDist = INF;
        for (const auto& b : m_graph.buildings()) {
            if (!visited[b.id] && m_dist[b.id] < minDist) {
                minDist = m_dist[b.id];
                u = b.id;
            }
        }

        if (u == -1) break;
        visited[u] = true;

        // Relax neighbours
        for (const auto& [v, w] : m_graph.neighbours(u)) {
            if (!visited[v] && m_dist[u] + w < m_dist[v]) {
                m_dist[v] = m_dist[u] + w;
                m_prev[v] = u;
            }
        }
    }
}

QVector<int> Dijkstra::reconstructPath(int destId) {
    QVector<int> path;
    for (int at = destId; at != -1; at = m_prev.value(at, -1))
        path.prepend(at);
    return path;
}

RouteResult Dijkstra::shortestPath(int sourceId, int destId) {
    run(sourceId);

    RouteResult result;
    result.totalDistance = m_dist.value(destId, INF);
    result.valid = (result.totalDistance < INF);

    if (result.valid)
        result.buildingIds = reconstructPath(destId);

    return result;
}
