#include "ShuttlePanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRandomGenerator>

ShuttlePanel::ShuttlePanel(ShuttleQueue* queues[3], QWidget* parent)
    : QWidget(parent), m_currentStop(0), m_nextStudentId(1), m_simRunning(false)
{
    for (int i = 0; i < 3; ++i) m_queues[i] = queues[i];
    m_simTimer = new QTimer(this);
    connect(m_simTimer, &QTimer::timeout, this, &ShuttlePanel::onSimulationTick);
    setupUI();
}

void ShuttlePanel::setupUI() {
    auto* layout = new QVBoxLayout(this);

    // ── Stop selector ──
    auto* stopLayout = new QHBoxLayout;
    stopLayout->addWidget(new QLabel("Bus Stop:"));
    m_stopCombo = new QComboBox;
    for (int i = 0; i < 3; ++i)
        m_stopCombo->addItem(m_queues[i]->stopName());
    stopLayout->addWidget(m_stopCombo);
    layout->addLayout(stopLayout);

    // ── Add student ──
    auto* addGroup = new QGroupBox("Add Student to Queue");
    auto* addLayout = new QVBoxLayout(addGroup);
    m_studentNameEdit = new QLineEdit;
    m_studentNameEdit->setPlaceholderText("Student name...");
    m_destinationEdit = new QLineEdit;
    m_destinationEdit->setPlaceholderText("Destination building...");
    m_enqueueBtn = new QPushButton("➕ Enqueue");
    m_enqueueBtn->setStyleSheet("background:#2a7a3a; color:white; padding:4px; border-radius:4px;");
    addLayout->addWidget(m_studentNameEdit);
    addLayout->addWidget(m_destinationEdit);
    addLayout->addWidget(m_enqueueBtn);
    layout->addWidget(addGroup);

    // ── Queue display ──
    auto* queueGroup = new QGroupBox("Queue (Front → Back)");
    auto* queueLayout = new QVBoxLayout(queueGroup);
    m_capacityBar = new QProgressBar;
    m_capacityBar->setRange(0, ShuttleQueue::MAX_CAPACITY);
    m_capacityBar->setFormat("Capacity: %v / %m");
    queueLayout->addWidget(m_capacityBar);
    m_queueList = new QListWidget;
    m_queueList->setStyleSheet("background:#0d1a2a; color:#aaffcc;");
    queueLayout->addWidget(m_queueList);
    m_statsLabel = new QLabel;
    m_statsLabel->setStyleSheet("color:#88ddff;");
    queueLayout->addWidget(m_statsLabel);
    layout->addWidget(queueGroup);

    // ── Controls ──
    auto* ctrlLayout = new QHBoxLayout;
    m_dequeueBtn = new QPushButton("🚌 Board (Dequeue)");
    m_dequeueBtn->setStyleSheet("background:#7a3a2a; color:white; padding:4px; border-radius:4px;");
    m_autoBtn    = new QPushButton("▶ Auto Simulate");
    m_autoBtn->setStyleSheet("background:#2a4a8a; color:white; padding:4px; border-radius:4px;");
    m_stopAutoBtn = new QPushButton("⏹ Stop");
    ctrlLayout->addWidget(m_dequeueBtn);
    ctrlLayout->addWidget(m_autoBtn);
    ctrlLayout->addWidget(m_stopAutoBtn);
    layout->addLayout(ctrlLayout);
    layout->addStretch();

    connect(m_enqueueBtn, &QPushButton::clicked, this, &ShuttlePanel::onEnqueue);
    connect(m_dequeueBtn, &QPushButton::clicked, this, &ShuttlePanel::onDequeue);
    connect(m_autoBtn,    &QPushButton::clicked, this, &ShuttlePanel::onAutoSimulate);
    connect(m_stopAutoBtn,&QPushButton::clicked, this, [this]() {
        m_simTimer->stop(); m_simRunning = false;
    });
    connect(m_stopCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ShuttlePanel::onStopChanged);

    refreshQueueDisplay();
}

void ShuttlePanel::onEnqueue() {
    QString name = m_studentNameEdit->text().trimmed();
    QString dest = m_destinationEdit->text().trimmed();
    if (name.isEmpty()) name = QString("Student%1").arg(m_nextStudentId);
    if (dest.isEmpty()) dest = "Main Gate";

    Student s;
    s.id         = m_nextStudentId++;
    s.name       = name;
    s.busStopId  = m_queues[m_currentStop]->busStopId();
    s.destination= dest;

    if (!m_queues[m_currentStop]->enqueue(s))
        m_statsLabel->setText("❌ Queue is full!");
    else {
        m_studentNameEdit->clear();
        m_destinationEdit->clear();
    }
    refreshQueueDisplay();
}

void ShuttlePanel::onDequeue() {
    Student s;
    if (m_queues[m_currentStop]->dequeue(s))
        m_statsLabel->setText(QString("🚌 %1 boarded → %2").arg(s.name, s.destination));
    else
        m_statsLabel->setText("Queue is empty.");
    refreshQueueDisplay();
}

void ShuttlePanel::onStopChanged(int index) {
    m_currentStop = index;
    refreshQueueDisplay();
}

void ShuttlePanel::onAutoSimulate() {
    m_simRunning = true;
    m_simTimer->start(800);
}

void ShuttlePanel::onSimulationTick() {
    // Randomly enqueue or dequeue
    int action = QRandomGenerator::global()->bounded(3); // 0,1,2

    if (action <= 1) { // enqueue
        Student s;
        s.id         = m_nextStudentId++;
        s.name       = QString("Student%1").arg(s.id);
        s.busStopId  = m_queues[m_currentStop]->busStopId();
        s.destination= "Campus";
        m_queues[m_currentStop]->enqueue(s);
    } else { // dequeue
        Student out;
        m_queues[m_currentStop]->dequeue(out);
    }
    refreshQueueDisplay();
}

void ShuttlePanel::refreshQueueDisplay() {
    ShuttleQueue* q = m_queues[m_currentStop];
    m_queueList->clear();
    m_capacityBar->setValue(q->size());

    auto snap = q->snapshot();
    for (int i = 0; i < snap.size(); ++i) {
        QString prefix = (i == 0) ? "👤 [FRONT] " : "👤 ";
        m_queueList->addItem(prefix + snap[i].name + " → " + snap[i].destination);
    }

    m_statsLabel->setText(QString("Size: %1 / %2 | %3")
        .arg(q->size())
        .arg(q->capacity())
        .arg(q->isEmpty() ? "Empty" : q->isFull() ? "FULL" : "Active"));
}
