#include "VisitLog.h"

VisitLog::VisitLog() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

VisitLog::~VisitLog() { clear(); }

void VisitLog::appendVisit(const Building& b, const QString& time) {
    VisitNode* node = new VisitNode(b, time);
    if (!m_tail) {
        m_head = m_tail = node;
    } else {
        node->prev = m_tail;
        m_tail->next = node;
        m_tail = node;
    }
    m_size++;
}

bool VisitLog::removeFirst() {
    if (!m_head) return false;
    VisitNode* tmp = m_head;
    m_head = m_head->next;
    if (m_head) m_head->prev = nullptr;
    else        m_tail = nullptr;
    delete tmp;
    m_size--;
    return true;
}

bool VisitLog::removeLast() {
    if (!m_tail) return false;
    VisitNode* tmp = m_tail;
    m_tail = m_tail->prev;
    if (m_tail) m_tail->next = nullptr;
    else        m_head = nullptr;
    delete tmp;
    m_size--;
    return true;
}

void VisitLog::clear() {
    while (m_head) removeFirst();
}

bool VisitLog::isEmpty() const { return m_size == 0; }
int  VisitLog::size()    const { return m_size; }

QVector<Building> VisitLog::toVector() const {
    QVector<Building> result;
    VisitNode* cur = m_head;
    while (cur) { result.append(cur->data); cur = cur->next; }
    return result;
}

QVector<Building> VisitLog::toReverseVector() const {
    QVector<Building> result;
    VisitNode* cur = m_tail;
    while (cur) { result.append(cur->data); cur = cur->prev; }
    return result;
}
