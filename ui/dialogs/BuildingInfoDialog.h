#pragma once
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "core/models/Building.h"

class BuildingInfoDialog : public QDialog {
    Q_OBJECT
public:
    explicit BuildingInfoDialog(const Building& building, QWidget* parent = nullptr);

signals:
    void routeFromHereRequested(int buildingId);
    void routeToHereRequested(int buildingId);
};
