#include "HashTable.h"

HashTable::HashTable() : m_size(0) {}

// Polynomial rolling hash on lowercase name
int HashTable::hashByName(const QString& key) const {
    QString k = key.toLower();
    long long h = 0;
    for (QChar c : k)
        h = (h * 31 + c.unicode()) % TABLE_SIZE;
    return (int)h;
}

// Simple sum hash on uppercase code
int HashTable::hashByCode(const QString& key) const {
    QString k = key.toUpper();
    int h = 0;
    for (QChar c : k)
        h = (h + c.unicode()) % TABLE_SIZE;
    return h;
}

void HashTable::insert(const Building& b) {
    int slot = hashByName(b.name);
    // Avoid duplicates
    for (auto& existing : m_table[slot]) {
        if (existing.id == b.id) { existing = b; return; }
    }
    m_table[slot].append(b);
    m_size++;
}

void HashTable::remove(int buildingId) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (auto it = m_table[i].begin(); it != m_table[i].end(); ++it) {
            if (it->id == buildingId) {
                m_table[i].erase(it);
                m_size--;
                return;
            }
        }
    }
}

void HashTable::clear() {
    for (int i = 0; i < TABLE_SIZE; ++i)
        m_table[i].clear();
    m_size = 0;
}

Building* HashTable::searchByName(const QString& name) {
    int slot = hashByName(name);
    for (auto& b : m_table[slot]) {
        if (b.name.toLower() == name.toLower())
            return &b;
    }
    return nullptr;
}

Building* HashTable::searchByCode(const QString& code) {
    // Code search: linear scan (codes are short, table is indexed by name)
    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (auto& b : m_table[i]) {
            if (b.code.toUpper() == code.toUpper())
                return &b;
        }
    }
    return nullptr;
}

QVector<Building> HashTable::allBuildings() const {
    QVector<Building> result;
    for (int i = 0; i < TABLE_SIZE; ++i)
        for (const auto& b : m_table[i])
            result.append(b);
    return result;
}

QVector<int> HashTable::chainLengths() const {
    QVector<int> lengths(TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; ++i)
        lengths[i] = m_table[i].size();
    return lengths;
}

QList<Building> HashTable::chainAt(int slot) const {
    if (slot < 0 || slot >= TABLE_SIZE) return {};
    return m_table[slot];
}
