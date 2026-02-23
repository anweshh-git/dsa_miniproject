#include "BuildingSorter.h"
#include <cmath>

double BuildingSorter::distanceTo(const Building& b, double x, double y) {
    double dx = b.x - x, dy = b.y - y;
    return std::sqrt(dx*dx + dy*dy);
}

void BuildingSorter::swap(Building& a, Building& b) {
    Building tmp = a; a = b; b = tmp;
}

void BuildingSorter::swap(Path& a, Path& b) {
    Path tmp = a; a = b; b = tmp;
}

// Bubble Sort — by distance from point
QVector<Building> BuildingSorter::sortByDistance(
    QVector<Building> buildings, double fromX, double fromY)
{
    int n = buildings.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            double da = distanceTo(buildings[j],   fromX, fromY);
            double db = distanceTo(buildings[j+1], fromX, fromY);
            if (da > db)
                swap(buildings[j], buildings[j+1]);
        }
    }
    return buildings;
}

// Insertion Sort — by usage count (descending)
QVector<Path> BuildingSorter::sortByPopularity(QVector<Path> paths) {
    int n = paths.size();
    for (int i = 1; i < n; ++i) {
        Path key = paths[i];
        int j = i - 1;
        while (j >= 0 && paths[j].usageCount < key.usageCount) {
            paths[j+1] = paths[j];
            j--;
        }
        paths[j+1] = key;
    }
    return paths;
}

// Selection Sort — by name alphabetically
QVector<Building> BuildingSorter::sortByName(QVector<Building> buildings) {
    int n = buildings.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i+1; j < n; ++j) {
            if (buildings[j].name < buildings[minIdx].name)
                minIdx = j;
        }
        if (minIdx != i)
            swap(buildings[i], buildings[minIdx]);
    }
    return buildings;
}

// Selection Sort — by ID ascending
QVector<Building> BuildingSorter::sortById(QVector<Building> buildings) {
    int n = buildings.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i+1; j < n; ++j) {
            if (buildings[j].id < buildings[minIdx].id)
                minIdx = j;
        }
        if (minIdx != i)
            swap(buildings[i], buildings[minIdx]);
    }
    return buildings;
}
