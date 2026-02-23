#pragma once
#include <QString>
#include <QVector>

struct Path {
    int from;
    int to;
    double distance;    // in metres
    int usageCount;     // for popularity sorting

    Path() : from(0), to(0), distance(0), usageCount(0) {}
    Path(int from, int to, double dist)
        : from(from), to(to), distance(dist), usageCount(0) {}
};

// Stores a full route result from Dijkstra
struct RouteResult {
    QVector<int> buildingIds;   // ordered list of building IDs in route
    double totalDistance;
    bool valid;

    RouteResult() : totalDistance(0), valid(false) {}
};
