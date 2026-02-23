#pragma once
#include "core/models/Building.h"
#include <QVector>

//chapter 4 (doubly linked list implemented)
//log of builidng visited today

struct VisitNode {
    Building    data;
    QString     visitTime;
    VisitNode*  prev;
    VisitNode*  next;

    VisitNode(const Building& b, const QString& t)
        : data(b), visitTime(t), prev(nullptr), next(nullptr) {}
};

class VisitLog {
public:
    VisitLog();
    ~VisitLog();

    void appendVisit(const Building& b, const QString& time);
    bool removeFirst();     // remove head
    bool removeLast();      // remove tail
    void clear();

    bool isEmpty() const;
    int  size()    const;

    const VisitNode* head() const { return m_head; }
    const VisitNode* tail() const { return m_tail; }

    // Traverse forward
    QVector<Building> toVector()        const;
    // Traverse backward
    QVector<Building> toReverseVector() const;

private:
    VisitNode* m_head;
    VisitNode* m_tail;
    int        m_size;
};
