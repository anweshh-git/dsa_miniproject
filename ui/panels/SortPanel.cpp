#include "SortPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <cmath>

SortPanel::SortPanel(Graph* graph, QWidget* parent)
    : QWidget(parent), m_graph(graph)
{
    setupUI();
}

void SortPanel::setupUI() {
    auto* layout = new QVBoxLayout(this);

    // ── Sort type ──
    auto* typeLayout = new QHBoxLayout;
    typeLayout->addWidget(new QLabel("Sort by:"));
    m_sortTypeCombo = new QComboBox;
    m_sortTypeCombo->addItem("Distance from point (Bubble Sort)");
    m_sortTypeCombo->addItem("Path Popularity (Insertion Sort)");
    m_sortTypeCombo->addItem("Building Name A–Z (Selection Sort)");
    typeLayout->addWidget(m_sortTypeCombo);
    layout->addLayout(typeLayout);

    m_algorithmLabel = new QLabel;
    m_algorithmLabel->setStyleSheet("color:#88aaff; font-style:italic; font-size:10px;");
    layout->addWidget(m_algorithmLabel);

    // ── Reference point (for distance sort) ──
    auto* refGroup = new QGroupBox("Reference Point (for Distance Sort)");
    auto* refLayout = new QHBoxLayout(refGroup);
    refLayout->addWidget(new QLabel("X:"));
    m_refXSpin = new QDoubleSpinBox;
    m_refXSpin->setRange(0, 1000);
    m_refXSpin->setValue(100);
    refLayout->addWidget(m_refXSpin);
    refLayout->addWidget(new QLabel("Y:"));
    m_refYSpin = new QDoubleSpinBox;
    m_refYSpin->setRange(0, 1000);
    m_refYSpin->setValue(500);
    refLayout->addWidget(m_refYSpin);
    layout->addWidget(refGroup);

    m_sortBtn = new QPushButton("▶ Sort");
    m_sortBtn->setStyleSheet("background:#2a5a8a; color:white; padding:6px; border-radius:4px; font-weight:bold;");
    layout->addWidget(m_sortBtn);

    // ── Building results ──
    m_buildingGroup = new QGroupBox("Sorted Buildings");
    auto* bgLayout = new QVBoxLayout(m_buildingGroup);
    m_buildingTable = new QTableWidget(0, 4);
    m_buildingTable->setHorizontalHeaderLabels({"Rank", "Name", "Code", "Distance/Value"});
    m_buildingTable->horizontalHeader()->setStretchLastSection(true);
    m_buildingTable->setStyleSheet("background:#0d1a2a; color:#aaddff;");
    m_buildingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bgLayout->addWidget(m_buildingTable);
    layout->addWidget(m_buildingGroup);

    // ── Path popularity results ──
    m_pathGroup = new QGroupBox("Sorted Paths (Popularity)");
    auto* pgLayout = new QVBoxLayout(m_pathGroup);
    m_pathTable = new QTableWidget(0, 3);
    m_pathTable->setHorizontalHeaderLabels({"From ID", "To ID", "Usage Count"});
    m_pathTable->horizontalHeader()->setStretchLastSection(true);
    m_pathTable->setStyleSheet("background:#0d1a2a; color:#aaddff;");
    m_pathTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pgLayout->addWidget(m_pathTable);
    layout->addWidget(m_pathGroup);
    m_pathGroup->setVisible(false);

    connect(m_sortBtn, &QPushButton::clicked, this, &SortPanel::onSort);
    connect(m_sortTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int idx) {
        bool isPath = (idx == 1);
        m_pathGroup->setVisible(isPath);
        m_buildingGroup->setVisible(!isPath);

        if (idx == 0) m_algorithmLabel->setText("Algorithm: Bubble Sort O(n²) — compares adjacent elements");
        else if (idx == 1) m_algorithmLabel->setText("Algorithm: Insertion Sort O(n²) — builds sorted portion incrementally");
        else m_algorithmLabel->setText("Algorithm: Selection Sort O(n²) — finds minimum in unsorted portion");
    });

    m_algorithmLabel->setText("Algorithm: Bubble Sort O(n²) — compares adjacent elements");
}

void SortPanel::onSort() {
    int idx = m_sortTypeCombo->currentIndex();
    double refX = m_refXSpin->value();
    double refY = m_refYSpin->value();

    if (idx == 0) {
        auto sorted = BuildingSorter::sortByDistance(m_graph->buildings(), refX, refY);
        displayBuildings(sorted, refX, refY);
        m_buildingGroup->setVisible(true);
        m_pathGroup->setVisible(false);
    }
    else if (idx == 1) {
        auto sorted = BuildingSorter::sortByPopularity(m_graph->edges());
        displayPaths(sorted);
        m_buildingGroup->setVisible(false);
        m_pathGroup->setVisible(true);
    }
    else {
        auto sorted = BuildingSorter::sortByName(m_graph->buildings());
        displayBuildings(sorted, refX, refY);
        m_buildingGroup->setVisible(true);
        m_pathGroup->setVisible(false);
    }
}

void SortPanel::displayBuildings(const QVector<Building>& buildings,
                                  double refX, double refY) {
    m_buildingTable->setRowCount(buildings.size());
    for (int i = 0; i < buildings.size(); ++i) {
        const auto& b = buildings[i];
        double dist = BuildingSorter::distanceTo(b, refX, refY);
        m_buildingTable->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
        m_buildingTable->setItem(i, 1, new QTableWidgetItem(b.name));
        m_buildingTable->setItem(i, 2, new QTableWidgetItem(b.code));
        m_buildingTable->setItem(i, 3, new QTableWidgetItem(
            m_sortTypeCombo->currentIndex() == 2 ?
            b.name.left(1) :
            QString::number(dist, 'f', 1) + " px"));
    }
}

void SortPanel::displayPaths(const QVector<Path>& paths) {
    m_pathTable->setRowCount(paths.size());
    for (int i = 0; i < paths.size(); ++i) {
        const auto& p = paths[i];
        m_pathTable->setItem(i, 0, new QTableWidgetItem(QString::number(p.from)));
        m_pathTable->setItem(i, 1, new QTableWidgetItem(QString::number(p.to)));
        m_pathTable->setItem(i, 2, new QTableWidgetItem(QString::number(p.usageCount)));
    }
}
