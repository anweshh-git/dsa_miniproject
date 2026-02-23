#pragma once
#include "core/models/Building.h"
#include "core/models/Path.h"
#include <QVector>
#include <QMap>

// Chapter 7 - Sorting
// Manual implementations (no std::sort)

class BuildingSorter {
public:
    // Sort buildings by distance from a given position (bubble sort)
    static QVector<Building> sortByDistance(
        QVector<Building> buildings,
        double fromX, double fromY);

    // Sort paths by usage count descending (insertion sort)
    static QVector<Path> sortByPopularity(QVector<Path> paths);

    // Sort buildings by name alphabetically (selection sort)
    static QVector<Building> sortByName(QVector<Building> buildings);

    // Sort buildings by ID ascending (used before binary search)
    static QVector<Building> sortById(QVector<Building> buildings);

    // Helper: euclidean distance from building to a point
    static double distanceTo(const Building& b, double x, double y);

private:
    // Swap helpers
    static void swap(Building& a, Building& b);
    static void swap(Path& a, Path& b);
};
