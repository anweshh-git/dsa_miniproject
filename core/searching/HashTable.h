#pragma once
#include "core/models/Building.h"
#include <QVector>
#include <QString>
#include <QList>

// Chapter 8 - Hashing
// Hash table with chaining for building lookup by name or code

class HashTable {
public:
    static const int TABLE_SIZE = 31; // prime number

    HashTable();

    void insert(const Building& b);
    void remove(int buildingId);
    void clear();

    //search by name
    Building* searchByName(const QString& name);

   //search by code
    Building* searchByCode(const QString& code);

    QVector<Building> allBuildings() const;

    int tableSize() const { return TABLE_SIZE; }
    int size()      const { return m_size; }

    QVector<int> chainLengths() const;

    // For visualisation: get chain at slot
    QList<Building> chainAt(int slot) const;

private:
    QList<Building> m_table[TABLE_SIZE];
    int             m_size;

    int hashByName(const QString& key) const;
    int hashByCode(const QString& key) const;
};
