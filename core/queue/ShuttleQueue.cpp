#include "ShuttleQueue.h"

ShuttleQueue::ShuttleQueue(int busStopId, const QString& stopName)
    : m_front(0), m_rear(-1), m_size(0),
      m_stopId(busStopId), m_stopName(stopName) {}

bool ShuttleQueue::enqueue(const Student& student) {
    if (isFull()) return false;
    m_rear = (m_rear + 1) % MAX_CAPACITY;
    m_data[m_rear] = student;
    m_size++;
    return true;
}

bool ShuttleQueue::dequeue(Student& out) {
    if (isEmpty()) return false;
    out = m_data[m_front];
    m_front = (m_front + 1) % MAX_CAPACITY;
    m_size--;
    return true;
}

bool ShuttleQueue::peek(Student& out) const {
    if (isEmpty()) return false;
    out = m_data[m_front];
    return true;
}

bool ShuttleQueue::isEmpty() const { return m_size == 0; }
bool ShuttleQueue::isFull()  const { return m_size == MAX_CAPACITY; }
int  ShuttleQueue::size()    const { return m_size; }

QVector<Student> ShuttleQueue::snapshot() const {
    QVector<Student> result;
    for (int i = 0; i < m_size; ++i)
        result.append(m_data[(m_front + i) % MAX_CAPACITY]);
    return result;
}

void ShuttleQueue::clear() {
    m_front = 0;
    m_rear  = -1;
    m_size  = 0;
}
