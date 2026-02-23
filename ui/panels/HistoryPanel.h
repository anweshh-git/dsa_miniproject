#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>

#include "core/linkedlist/PathHistory.h"
#include "core/linkedlist/VisitLog.h"

class HistoryPanel : public QWidget {
    Q_OBJECT
public:
    explicit HistoryPanel(PathHistory* pathHistory, VisitLog* visitLog,
                          QWidget* parent = nullptr);

    void refresh();

private slots:
    void onClearHistory();
    void onClearVisitLog();
    void onRemoveLastVisit();

private:
    void setupUI();
    void refreshPathHistory();
    void refreshVisitLog();

    PathHistory* m_pathHistory;
    VisitLog*    m_visitLog;

    QTabWidget*   m_tabs;

    // Path history tab
    QTableWidget* m_historyTable;
    QLabel*       m_historyStatsLabel;
    QPushButton*  m_clearHistoryBtn;

    // Visit log tab
    QListWidget*  m_visitList;
    QLabel*       m_visitStatsLabel;
    QPushButton*  m_clearVisitBtn;
    QPushButton*  m_removeLastBtn;
};
