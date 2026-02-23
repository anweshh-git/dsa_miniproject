#pragma once
#include <QString>
#include <QVector>

// Chapter 3 - Queue
// Circular array-based queue for shuttle bus stop simulation

struct Student {
    int id;
    QString name;
    int busStopId;      // which stop they're waiting at
    QString destination;
};

class ShuttleQueue {
public:
    static const int MAX_CAPACITY = 20;

    explicit ShuttleQueue(int busStopId, const QString& stopName);

    bool enqueue(const Student& student);   // student arrives at stop
    bool dequeue(Student& out);             // student boards shuttle
    bool peek(Student& out) const;          // who's next

    bool isEmpty() const;
    bool isFull() const;
    int  size() const;
    int  capacity() const { return MAX_CAPACITY; }

    int         busStopId() const { return m_stopId; }
    QString     stopName()  const { return m_stopName; }

    // Returns snapshot of queue contents (front to back)
    QVector<Student> snapshot() const;

    void clear();

private:
    Student m_data[MAX_CAPACITY];
    int     m_front;
    int     m_rear;
    int     m_size;
    int     m_stopId;
    QString m_stopName;
};
