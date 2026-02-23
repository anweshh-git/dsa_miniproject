#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QGroupBox>

#include "core/graph/Graph.h"
#include "core/sorting/BuildingSorter.h"

class SortPanel : public QWidget {
    Q_OBJECT
public:
    explicit SortPanel(Graph* graph, QWidget* parent = nullptr);

signals:
    void buildingSelected(int buildingId);

private slots:
    void onSort();

private:
    void setupUI();
    void displayBuildings(const QVector<Building>& buildings, double refX, double refY);
    void displayPaths(const QVector<Path>& paths);

    Graph* m_graph;

    QComboBox*      m_sortTypeCombo;
    QDoubleSpinBox* m_refXSpin;
    QDoubleSpinBox* m_refYSpin;
    QPushButton*    m_sortBtn;
    QLabel*         m_algorithmLabel;

    QGroupBox*    m_buildingGroup;
    QTableWidget* m_buildingTable;

    QGroupBox*    m_pathGroup;
    QTableWidget* m_pathTable;
};
