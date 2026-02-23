#pragma once
#include "core/models/Path.h"
#include <QString>
#include <QVector>

//chapter 4 implementedStor
//stores previously searched routes
struct PathRecord {
    int         fromId;
    int         toId;
    QString     fromName;
    QString     toName;
    double      distance;
    QVector<int> route;       // building IDs in order
    QString     timestamp;
};

struct PathNode {
    PathRecord  data;
    PathNode*   next;
    PathNode(const PathRecord& d) : data(d), next(nullptr) {}
};

class PathHistory {
public:
    PathHistory();
    ~PathHistory();

    void        prepend(const PathRecord& record);  //add to front
    bool        removeFirst();                      //remove front
    void        clear();

    bool        isEmpty() const;
    int         size()    const;

    const PathNode* head() const { return m_head; }

    //convert to  vector for display
    QVector<PathRecord> toVector() const;

private:
    PathNode* m_head;
    int       m_size;
};
