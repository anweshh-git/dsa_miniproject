#pragma once
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTimer>
#include <QProgressBar>

#include "core/queue/ShuttleQueue.h"

class ShuttlePanel : public QWidget {
    Q_OBJECT
public:
    // Takes ownership of 3 ShuttleQueue pointers
    explicit ShuttlePanel(ShuttleQueue* queues[3], QWidget* parent = nullptr);

private slots:
    void onEnqueue();
    void onDequeue();
    void onAutoSimulate();
    void onStopChanged(int index);
    void onSimulationTick();

private:
    void setupUI();
    void refreshQueueDisplay();

    ShuttleQueue* m_queues[3];
    int           m_currentStop;
    int           m_nextStudentId;

    QComboBox*    m_stopCombo;
    QLineEdit*    m_studentNameEdit;
    QLineEdit*    m_destinationEdit;
    QPushButton*  m_enqueueBtn;
    QPushButton*  m_dequeueBtn;
    QPushButton*  m_autoBtn;
    QPushButton*  m_stopAutoBtn;

    QListWidget*  m_queueList;
    QLabel*       m_statsLabel;
    QProgressBar* m_capacityBar;

    QTimer*       m_simTimer;
    bool          m_simRunning;
};
