#include "HistoryPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

HistoryPanel::HistoryPanel(PathHistory* pathHistory, VisitLog* visitLog, QWidget* parent)
    : QWidget(parent), m_pathHistory(pathHistory), m_visitLog(visitLog)
{
    setupUI();
}

void HistoryPanel::setupUI() {
    auto* layout = new QVBoxLayout(this);

    m_tabs = new QTabWidget;

    //history tab
    auto* histWidget = new QWidget;
    auto* histLayout = new QVBoxLayout(histWidget);

    auto* histInfoLabel = new QLabel("📌 Singly Linked List — most recent route at head");
    histInfoLabel->setStyleSheet("color:#88aaff; font-style:italic; font-size:10px;");
    histLayout->addWidget(histInfoLabel);

    m_historyTable = new QTableWidget(0, 4);
    m_historyTable->setHorizontalHeaderLabels({"From", "To", "Distance (m)", "Time"});
    m_historyTable->horizontalHeader()->setStretchLastSection(true);
    m_historyTable->setStyleSheet("background:#0d1a2a; color:#aaddff;");
    m_historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    histLayout->addWidget(m_historyTable);

    m_historyStatsLabel = new QLabel("No routes searched yet.");
    m_historyStatsLabel->setStyleSheet("color:#88ddff;");
    histLayout->addWidget(m_historyStatsLabel);

    m_clearHistoryBtn = new QPushButton("🗑 Clear History");
    m_clearHistoryBtn->setStyleSheet("background:#5a2a2a; color:white; padding:4px; border-radius:4px;");
    histLayout->addWidget(m_clearHistoryBtn);

    m_tabs->addTab(histWidget, "Route History");

    // ── Visit Log Tab (Doubly Linked List) ──
    auto* visitWidget = new QWidget;
    auto* visitLayout = new QVBoxLayout(visitWidget);

    auto* visitInfoLabel = new QLabel("📌 Doubly Linked List — append at tail, remove from head or tail");
    visitInfoLabel->setStyleSheet("color:#88aaff; font-style:italic; font-size:10px;");
    visitInfoLabel->setWordWrap(true);
    visitLayout->addWidget(visitInfoLabel);

    m_visitList = new QListWidget;
    m_visitList->setStyleSheet("background:#0d1a2a; color:#aaffcc;");
    visitLayout->addWidget(m_visitList);

    m_visitStatsLabel = new QLabel("No buildings visited yet.");
    m_visitStatsLabel->setStyleSheet("color:#88ddff;");
    visitLayout->addWidget(m_visitStatsLabel);

    auto* visitBtnLayout = new QHBoxLayout;
    m_removeLastBtn = new QPushButton("Remove Last");
    m_removeLastBtn->setStyleSheet("background:#3a5a2a; color:white; padding:4px; border-radius:4px;");
    m_clearVisitBtn = new QPushButton("Clear All");
    m_clearVisitBtn->setStyleSheet("background:#5a2a2a; color:white; padding:4px; border-radius:4px;");
    visitBtnLayout->addWidget(m_removeLastBtn);
    visitBtnLayout->addWidget(m_clearVisitBtn);
    visitLayout->addLayout(visitBtnLayout);

    m_tabs->addTab(visitWidget, "Visit Log");

    layout->addWidget(m_tabs);

    connect(m_clearHistoryBtn, &QPushButton::clicked, this, &HistoryPanel::onClearHistory);
    connect(m_clearVisitBtn,   &QPushButton::clicked, this, &HistoryPanel::onClearVisitLog);
    connect(m_removeLastBtn,   &QPushButton::clicked, this, &HistoryPanel::onRemoveLastVisit);
}

void HistoryPanel::refresh() {
    refreshPathHistory();
    refreshVisitLog();
}

void HistoryPanel::refreshPathHistory() {
    auto records = m_pathHistory->toVector();
    m_historyTable->setRowCount(records.size());

    for (int i = 0; i < records.size(); ++i) {
        const auto& r = records[i];
        m_historyTable->setItem(i, 0, new QTableWidgetItem(r.fromName));
        m_historyTable->setItem(i, 1, new QTableWidgetItem(r.toName));
        m_historyTable->setItem(i, 2, new QTableWidgetItem(QString::number(r.distance)));
        m_historyTable->setItem(i, 3, new QTableWidgetItem(r.timestamp));
    }

    m_historyStatsLabel->setText(
        QString("Total routes: %1 | Head: %2")
            .arg(m_pathHistory->size())
            .arg(m_pathHistory->isEmpty() ? "none" :
                 m_pathHistory->head()->data.fromName + " → " +
                 m_pathHistory->head()->data.toName));
}

void HistoryPanel::refreshVisitLog() {
    m_visitList->clear();
    auto buildings = m_visitLog->toVector();
    for (int i = 0; i < buildings.size(); ++i) {
        QString prefix = (i == 0) ? " [HEAD] " : (i == buildings.size()-1) ? "[TAIL] " : "-";
        m_visitList->addItem(prefix + buildings[i].name + " (" + buildings[i].code + ")");
    }

    QString headName = m_visitLog->isEmpty() ? "none" :
                       m_visitLog->head()->data.name;
    QString tailName = m_visitLog->isEmpty() ? "none" :
                       m_visitLog->tail()->data.name;

    m_visitStatsLabel->setText(
        QString("Total: %1 | Head: %2 | Tail: %3")
            .arg(m_visitLog->size())
            .arg(headName)
            .arg(tailName));
}

void HistoryPanel::onClearHistory() {
    m_pathHistory->clear();
    refresh();
}

void HistoryPanel::onClearVisitLog() {
    m_visitLog->clear();
    refresh();
}

void HistoryPanel::onRemoveLastVisit() {
    m_visitLog->removeLast();
    refresh();
}
