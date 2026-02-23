#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QTableWidget>
#include <QGroupBox>

#include "core/graph/Graph.h"
#include "core/searching/HashTable.h"
#include "core/searching/BinarySearch.h"
#include "core/sorting/BuildingSorter.h"

class SearchPanel : public QWidget {
    Q_OBJECT
public:
    explicit SearchPanel(Graph* graph, HashTable* hashTable, QWidget* parent = nullptr);

signals:
    void buildingFound(int buildingId);

private slots:
    void onHashSearch();
    void onBinarySearch();
    void onSearchTypeChanged(int index);

private:
    void setupUI();
    void displayHashResult(Building* b, const QString& query);
    void displayBinaryResult(const BinarySearchResult& res,
                             const QVector<Building>& sorted, int targetId);

    Graph*      m_graph;
    HashTable*  m_hashTable;

    QComboBox*  m_searchTypeCombo;  // Hash / Binary
    QLineEdit*  m_queryEdit;
    QPushButton* m_searchBtn;

    QGroupBox*  m_hashGroup;
    QLabel*     m_hashResultLabel;
    QTableWidget* m_hashStatsTable;  // show chain lengths

    QGroupBox*  m_binaryGroup;
    QTextEdit*  m_binaryStepsText;
    QLabel*     m_binaryResultLabel;

    QComboBox*  m_hashFieldCombo;   // search by name / code
};
