#include "BuildingInfoDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

BuildingInfoDialog::BuildingInfoDialog(const Building& building, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(building.name);
    setMinimumWidth(300);

    auto* layout = new QVBoxLayout(this);

    auto makeRow = [](const QString& key, const QString& val) {
        auto* lbl = new QLabel(QString("<b>%1:</b> %2").arg(key, val));
        lbl->setWordWrap(true);
        return lbl;
    };

    layout->addWidget(makeRow("Name",        building.name));
    layout->addWidget(makeRow("Code",        building.code));
    layout->addWidget(makeRow("ID",          QString::number(building.id)));
    layout->addWidget(makeRow("Description", building.description));
    layout->addWidget(makeRow("Position",    QString("(%1, %2)").arg(building.x).arg(building.y)));
    layout->addWidget(makeRow("Visit Count", QString::number(building.visitCount)));

    auto* sep = new QFrame;
    sep->setFrameShape(QFrame::HLine);
    layout->addWidget(sep);

    auto* btnLayout = new QHBoxLayout;
    auto* fromBtn = new QPushButton("Route FROM here");
    auto* toBtn   = new QPushButton("Route TO here");
    auto* closeBtn = new QPushButton("Close");

    btnLayout->addWidget(fromBtn);
    btnLayout->addWidget(toBtn);
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);

    connect(fromBtn,  &QPushButton::clicked, this, [this, &building]() {
        emit routeFromHereRequested(building.id); accept();
    });
    connect(toBtn,    &QPushButton::clicked, this, [this, &building]() {
        emit routeToHereRequested(building.id); accept();
    });
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
}
