#pragma once
#include "core/models/Building.h"
#include <QVector>
#include <QString>

// Chapter 8 - Binary Search
// Search buildings by ID (array must be sorted by ID first)

struct BinarySearchResult {
    int         index;          // -1 if not found
    int         comparisons;    // number of comparisons made
    QVector<int> stepsLow;      // low pointer at each step (for visualisation)
    QVector<int> stepsHigh;
    QVector<int> stepsMid;
    bool        found;
};

class BinarySearch {
public:
    // Search sorted buildings array by building ID
    static BinarySearchResult searchById(
        const QVector<Building>& sortedBuildings, int targetId);

    // Search sorted buildings array by name (must be sorted alphabetically)
    static BinarySearchResult searchByName(
        const QVector<Building>& sortedBuildings, const QString& name);
};
