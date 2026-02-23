#pragma once
#include <QString>

struct Building {
    int id;
    QString name;
    QString code;       //CSE,ADM etc
    QString description;
    double x, y;        // pixel position on campus map
    int visitCount;     // for popularity sorting

    Building() : id(0), x(0), y(0), visitCount(0) {}
    Building(int id, const QString& name, const QString& code,
             double x, double y, const QString& desc = "")
        : id(id), name(name), code(code), description(desc),
          x(x), y(y), visitCount(0) {}

    bool operator==(const Building& o) const { return id == o.id; }
    bool operator<(const Building& o) const  { return id < o.id; }
};
