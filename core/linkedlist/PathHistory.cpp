#include "PathHistory.h"

PathHistory::PathHistory() : m_head(nullptr), m_size(0) {}

PathHistory::~PathHistory() { clear(); }

void PathHistory::prepend(const PathRecord& record) {
    PathNode* node = new PathNode(record);
    node->next = m_head;
    m_head = node;
    m_size++;
}

bool PathHistory::removeFirst() {
    if (!m_head) return false;
    PathNode* tmp = m_head;
    m_head = m_head->next;
    delete tmp;
    m_size--;
    return true;
}

void PathHistory::clear() {
    while (m_head) removeFirst();
}

bool PathHistory::isEmpty() const { return m_size == 0; }
int  PathHistory::size()    const { return m_size; }

QVector<PathRecord> PathHistory::toVector() const {
    QVector<PathRecord> result;
    PathNode* cur = m_head;
    while (cur) {
        result.append(cur->data);
        cur = cur->next;
    }
    return result;
}
