#include "MainWindow.h"
#include <QVBoxLayout>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_dijkstra(nullptr), m_selectedBuildingId(-1)
{
    setWindowTitle("Enhanced Campus Map - DSA Project");
    resize(1280, 800);

    setupDSA();
    setupUI();
    connectSignals();

    updateStatusBar("Campus map loaded. Click a building to select it.");
}

MainWindow::~MainWindow() {
    delete m_dijkstra;
    for (int i = 0; i < 3; ++i)
        delete m_shuttleQueues[i];
}

void MainWindow::setupDSA() {
    // Graph
    m_graph.loadDefaults();
    m_dijkstra = new Dijkstra(m_graph);

    // Hash table — populate from graph
    for (const auto& b : m_graph.buildings())
        m_hashTable.insert(b);

    // Shuttle queues for 3 bus stops
    m_shuttleQueues[0] = new ShuttleQueue(1, "Main Gate Stop");
    m_shuttleQueues[1] = new ShuttleQueue(4, "CSE / ECE Stop");
    m_shuttleQueues[2] = new ShuttleQueue(7, "Cafeteria Stop");
}

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(4);

    m_splitter = new QSplitter(Qt::Horizontal, central);

    // Left: map
    m_mapView = new MapView(&m_graph, m_splitter);
    m_splitter->addWidget(m_mapView);

    // Right: tabs
    m_tabWidget = new QTabWidget(m_splitter);
    m_tabWidget->setMinimumWidth(360);

    m_searchPanel  = new SearchPanel(&m_graph, &m_hashTable);
    m_shuttlePanel = new ShuttlePanel(m_shuttleQueues);
    m_historyPanel = new HistoryPanel(&m_pathHistory, &m_visitLog);
    m_sortPanel    = new SortPanel(&m_graph);

    m_tabWidget->addTab(m_searchPanel,  "Search");
    m_tabWidget->addTab(m_shuttlePanel, "Shuttle");
    m_tabWidget->addTab(m_historyPanel, "History");
    m_tabWidget->addTab(m_sortPanel,    "Sort");

    m_splitter->addWidget(m_tabWidget);
    m_splitter->setStretchFactor(0, 3);
    m_splitter->setStretchFactor(1, 1);

    mainLayout->addWidget(m_splitter);

    // Status bar
    m_statusLabel = new QLabel("Ready");
    statusBar()->addWidget(m_statusLabel);
}

void MainWindow::connectSignals() {
    connect(m_mapView,     &MapView::buildingClicked,  this, &MainWindow::onBuildingClicked);
    connect(m_mapView,     &MapView::routeRequested,   this, &MainWindow::onRouteRequested);
    connect(m_searchPanel, &SearchPanel::buildingFound,this, &MainWindow::onBuildingSearched);
}

void MainWindow::onBuildingClicked(int buildingId) {
    const Building* b = m_graph.findBuilding(buildingId);
    if (!b) return;

    // Add to visit log
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_visitLog.appendVisit(*b, time);
    m_historyPanel->refresh();

    // Increment visit count for sorting
    Building* mb = m_graph.findBuilding(buildingId);
    if (mb) mb->visitCount++;

    if (m_selectedBuildingId == -1) {
        m_selectedBuildingId = buildingId;
        updateStatusBar(QString("Selected: %1. Click another building to find route.").arg(b->name));
    } else if (m_selectedBuildingId == buildingId) {
        m_selectedBuildingId = -1;
        m_mapView->clearRoute();
        updateStatusBar("Selection cleared.");
    } else {
        onRouteRequested(m_selectedBuildingId, buildingId);
        m_selectedBuildingId = -1;
    }
}

void MainWindow::onRouteRequested(int fromId, int toId) {
    RouteResult result = m_dijkstra->shortestPath(fromId, toId);
    m_mapView->showRoute(result);

    if (!result.valid) {
        updateStatusBar("No path found between selected buildings.");
        return;
    }

    // Update edge usage
    m_graph.incrementUsage(result.buildingIds);

    // Add to path history (linked list)
    const Building* from = m_graph.findBuilding(fromId);
    const Building* to   = m_graph.findBuilding(toId);
    if (from && to) {
        PathRecord rec;
        rec.fromId    = fromId;
        rec.toId      = toId;
        rec.fromName  = from->name;
        rec.toName    = to->name;
        rec.distance  = result.totalDistance;
        rec.route     = result.buildingIds;
        rec.timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
        m_pathHistory.prepend(rec);
        m_historyPanel->refresh();
    }

    updateStatusBar(QString("Route: %1 → %2 | Distance: %3 m | Hops: %4")
        .arg(from ? from->name : "?")
        .arg(to   ? to->name   : "?")
        .arg(result.totalDistance)
        .arg(result.buildingIds.size() - 1));
}

void MainWindow::onBuildingSearched(int buildingId) {
    m_mapView->highlightBuilding(buildingId);
    const Building* b = m_graph.findBuilding(buildingId);
    if (b)
        updateStatusBar(QString("Found: %1 (%2)").arg(b->name, b->code));
}

void MainWindow::updateStatusBar(const QString& msg) {
    m_statusLabel->setText(msg);
}
