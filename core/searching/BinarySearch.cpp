#include "BinarySearch.h"

BinarySearchResult BinarySearch::searchById(
    const QVector<Building>& sortedBuildings, int targetId)
{
    BinarySearchResult result;
    result.found = false;
    result.index = -1;
    result.comparisons = 0;

    int low = 0, high = sortedBuildings.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        result.comparisons++;
        result.stepsLow.append(low);
        result.stepsHigh.append(high);
        result.stepsMid.append(mid);

        if (sortedBuildings[mid].id == targetId) {
            result.index = mid;
            result.found = true;
            break;
        } else if (sortedBuildings[mid].id < targetId) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return result;
}

BinarySearchResult BinarySearch::searchByName(
    const QVector<Building>& sortedBuildings, const QString& name)
{
    BinarySearchResult result;
    result.found = false;
    result.index = -1;
    result.comparisons = 0;

    int low = 0, high = sortedBuildings.size() - 1;
    QString target = name.toLower();

    while (low <= high) {
        int mid = low + (high - low) / 2;
        result.comparisons++;
        result.stepsLow.append(low);
        result.stepsHigh.append(high);
        result.stepsMid.append(mid);

        int cmp = sortedBuildings[mid].name.toLower().compare(target);
        if (cmp == 0) {
            result.index = mid;
            result.found = true;
            break;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return result;
}
