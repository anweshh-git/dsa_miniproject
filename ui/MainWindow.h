#pragma once
#include <QMainWindow>
#include <QSplitter>
#include <QTabWidget>
#include <QStatusBar>
#include <QLabel>

#include "core/graph/Graph.h"
#include "core/graph/Dijkstra.h"
#include "core/queue/ShuttleQueue.h"
#include "core/linkedlist/PathHistory.h"
#include "core/linkedlist/VisitLog.h"
#include "core/sorting/BuildingSorter.h"
#include "core/searching/HashTable.h"
#include "core/searching/BinarySearch.h"

#include "MapView.h"
#include "panels/SearchPanel.h"
#include "panels/ShuttlePanel.h"
#include "panels/HistoryPanel.h"
#include "panels/SortPanel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onBuildingClicked(int buildingId);
    void onRouteRequested(int fromId, int toId);
    void onBuildingSearched(int buildingId);

private:
    void setupUI();
    void setupDSA();
    void connectSignals();
    void updateStatusBar(const QString& msg);

    // DSA Core
    Graph        m_graph;
    Dijkstra*    m_dijkstra;
    ShuttleQueue* m_shuttleQueues[3];   // 3 bus stops
    PathHistory  m_pathHistory;
    VisitLog     m_visitLog;
    HashTable    m_hashTable;

    // UI
    QSplitter*    m_splitter;
    MapView*      m_mapView;
    QTabWidget*   m_tabWidget;
    SearchPanel*  m_searchPanel;
    ShuttlePanel* m_shuttlePanel;
    HistoryPanel* m_historyPanel;
    SortPanel*    m_sortPanel;

    QLabel* m_statusLabel;
    int     m_selectedBuildingId;
};
