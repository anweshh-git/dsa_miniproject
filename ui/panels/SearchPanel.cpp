#include "SearchPanel.h"
#include "core/sorting/BuildingSorter.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

SearchPanel::SearchPanel(Graph* graph, HashTable* hashTable, QWidget* parent)
    : QWidget(parent), m_graph(graph), m_hashTable(hashTable)
{
    setupUI();
}

void SearchPanel::setupUI() {
    auto* layout = new QVBoxLayout(this);
    layout->setSpacing(8);

    // ── Search Type ──
    auto* typeLayout = new QHBoxLayout;
    typeLayout->addWidget(new QLabel("Method:"));
    m_searchTypeCombo = new QComboBox;
    m_searchTypeCombo->addItem("Hash Table (by Name/Code)");
    m_searchTypeCombo->addItem("Binary Search (by ID)");
    typeLayout->addWidget(m_searchTypeCombo);
    layout->addLayout(typeLayout);

    // ── Query ──
    auto* queryLayout = new QHBoxLayout;
    m_queryEdit = new QLineEdit;
    m_queryEdit->setPlaceholderText("Building name, code, or ID...");
    m_searchBtn = new QPushButton("Search");
    m_searchBtn->setStyleSheet("background:#2a6aaf; color:white; padding:4px 10px; border-radius:4px;");
    queryLayout->addWidget(m_queryEdit);
    queryLayout->addWidget(m_searchBtn);
    layout->addLayout(queryLayout);

    // ── Hash field selector ──
    auto* hashFieldLayout = new QHBoxLayout;
    hashFieldLayout->addWidget(new QLabel("Search by:"));
    m_hashFieldCombo = new QComboBox;
    m_hashFieldCombo->addItem("Name");
    m_hashFieldCombo->addItem("Code");
    hashFieldLayout->addWidget(m_hashFieldCombo);
    layout->addLayout(hashFieldLayout);

    // ── Hash Group ──
    m_hashGroup = new QGroupBox("Hash Table Result");
    auto* hashLayout = new QVBoxLayout(m_hashGroup);
    m_hashResultLabel = new QLabel("No search yet.");
    m_hashResultLabel->setWordWrap(true);
    m_hashResultLabel->setStyleSheet("color:#aaffaa; font-size:12px;");
    hashLayout->addWidget(m_hashResultLabel);

    m_hashStatsTable = new QTableWidget(0, 2);
    m_hashStatsTable->setHorizontalHeaderLabels({"Slot", "Chain Length"});
    m_hashStatsTable->horizontalHeader()->setStretchLastSection(true);
    m_hashStatsTable->setMaximumHeight(160);
    // Populate with chain lengths
    auto lengths = m_hashTable->chainLengths();
    m_hashStatsTable->setRowCount(lengths.size());
    for (int i = 0; i < lengths.size(); ++i) {
        m_hashStatsTable->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        m_hashStatsTable->setItem(i, 1, new QTableWidgetItem(QString::number(lengths[i])));
    }
    hashLayout->addWidget(new QLabel("Hash Table Distribution:"));
    hashLayout->addWidget(m_hashStatsTable);
    layout->addWidget(m_hashGroup);

    // ── Binary Group ──
    m_binaryGroup = new QGroupBox("Binary Search Steps");
    m_binaryGroup->setVisible(false);
    auto* binLayout = new QVBoxLayout(m_binaryGroup);
    m_binaryResultLabel = new QLabel("No search yet.");
    m_binaryResultLabel->setStyleSheet("color:#aaffaa; font-size:12px;");
    binLayout->addWidget(m_binaryResultLabel);
    m_binaryStepsText = new QTextEdit;
    m_binaryStepsText->setReadOnly(true);
    m_binaryStepsText->setMaximumHeight(180);
    m_binaryStepsText->setStyleSheet("background:#0d1a2a; color:#88ddff; font-family: monospace;");
    binLayout->addWidget(m_binaryStepsText);
    layout->addWidget(m_binaryGroup);

    layout->addStretch();

    connect(m_searchBtn, &QPushButton::clicked, this, [this]() {
        if (m_searchTypeCombo->currentIndex() == 0) onHashSearch();
        else onBinarySearch();
    });
    connect(m_searchTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SearchPanel::onSearchTypeChanged);
    connect(m_queryEdit, &QLineEdit::returnPressed, m_searchBtn, &QPushButton::click);
}

void SearchPanel::onSearchTypeChanged(int index) {
    bool isHash = (index == 0);
    m_hashGroup->setVisible(isHash);
    m_hashFieldCombo->setVisible(isHash);
    m_binaryGroup->setVisible(!isHash);
    m_queryEdit->setPlaceholderText(isHash ? "Building name or code..." : "Building ID (number)...");
}

void SearchPanel::onHashSearch() {
    QString query = m_queryEdit->text().trimmed();
    if (query.isEmpty()) return;

    Building* result = nullptr;
    if (m_hashFieldCombo->currentText() == "Name")
        result = m_hashTable->searchByName(query);
    else
        result = m_hashTable->searchByCode(query);

    displayHashResult(result, query);
}

void SearchPanel::displayHashResult(Building* b, const QString& query) {
    if (!b) {
        m_hashResultLabel->setText(QString("❌ '%1' not found in hash table.").arg(query));
        return;
    }
    m_hashResultLabel->setText(
        QString("✅ Found!\nName: %1\nCode: %2\nID: %3\nDescription: %4")
            .arg(b->name, b->code)
            .arg(b->id)
            .arg(b->description));
    emit buildingFound(b->id);

    // Highlight slot in table
    int slot = -1;
    auto lengths = m_hashTable->chainLengths();
    for (int i = 0; i < lengths.size(); ++i) {
        auto chain = m_hashTable->chainAt(i);
        for (const auto& cb : chain) {
            if (cb.id == b->id) { slot = i; break; }
        }
        if (slot != -1) break;
    }
    if (slot != -1)
        m_hashStatsTable->selectRow(slot);
}

void SearchPanel::onBinarySearch() {
    bool ok;
    int targetId = m_queryEdit->text().trimmed().toInt(&ok);
    if (!ok) {
        m_binaryResultLabel->setText("❌ Please enter a numeric building ID.");
        return;
    }

    QVector<Building> sorted = BuildingSorter::sortById(m_graph->buildings());
    BinarySearchResult res   = BinarySearch::searchById(sorted, targetId);
    displayBinaryResult(res, sorted, targetId);
}

void SearchPanel::displayBinaryResult(const BinarySearchResult& res,
                                      const QVector<Building>& sorted, int targetId)
{
    m_binaryStepsText->clear();
    QString steps;
    steps += QString("Target ID: %1\n").arg(targetId);
    steps += QString("Array size: %1\n\n").arg(sorted.size());
    steps += "Step | Low | Mid | High | Building at Mid\n";
    steps += QString("-").repeated(50) + "\n";

    for (int i = 0; i < res.stepsMid.size(); ++i) {
        int mid = res.stepsMid[i];
        steps += QString("%1    | %2   | %3   | %4    | %5 (ID=%6)\n")
            .arg(i+1)
            .arg(res.stepsLow[i])
            .arg(mid)
            .arg(res.stepsHigh[i])
            .arg(sorted[mid].name)
            .arg(sorted[mid].id);
    }

    steps += QString("\nTotal comparisons: %1").arg(res.comparisons);
    m_binaryStepsText->setPlainText(steps);

    if (res.found) {
        const Building& b = sorted[res.index];
        m_binaryResultLabel->setText(
            QString(" Found at index %1!\nName: %2  Code: %3")
                .arg(res.index).arg(b.name).arg(b.code));
        emit buildingFound(b.id);
    } else {
        m_binaryResultLabel->setText(
            QString("❌ ID %1 not found. (%2 comparisons)").arg(targetId).arg(res.comparisons));
    }
}
