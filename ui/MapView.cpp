#include "MapView.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QDebug>

MapView::MapView(Graph* graph, QWidget* parent)
    : QGraphicsView(parent), m_graph(graph),
      m_highlightRing(nullptr), m_animStep(0)
{
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);

    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Background colour
    m_scene->setBackgroundBrush(QBrush(QColor("#1e2a3a")));

    m_animTimer = new QTimer(this);
    connect(m_animTimer, &QTimer::timeout, this, &MapView::animateRoute);

    drawGraph();
}

void MapView::drawGraph() {
    m_scene->clear();
    m_nodeItems.clear();
    m_labelItems.clear();
    m_routeLines.clear();
    m_highlightRing = nullptr;

    drawEdges();
    drawBuildings();

    m_scene->setSceneRect(m_scene->itemsBoundingRect().adjusted(-40,-40,40,40));
}

void MapView::drawEdges() {
    QPen edgePen(QColor("#3a5a7a"), 2, Qt::SolidLine);
    for (const auto& path : m_graph->edges()) {
        const Building* from = m_graph->findBuilding(path.from);
        const Building* to   = m_graph->findBuilding(path.to);
        if (!from || !to) continue;

        auto* line = m_scene->addLine(from->x, from->y, to->x, to->y, edgePen);
        line->setZValue(0);

        // Distance label on edge midpoint
        double mx = (from->x + to->x) / 2;
        double my = (from->y + to->y) / 2;
        auto* lbl = m_scene->addText(QString("%1m").arg((int)path.distance));
        lbl->setDefaultTextColor(QColor("#6a9abf"));
        lbl->setFont(QFont("Arial", 7));
        lbl->setPos(mx - 10, my - 8);
        lbl->setZValue(1);
    }
}

void MapView::drawBuildings() {
    QFont labelFont("Arial", 8, QFont::Bold);

    for (const auto& b : m_graph->buildings()) {
        double cx = b.x - NODE_RADIUS;
        double cy = b.y - NODE_RADIUS;
        double sz = NODE_RADIUS * 2;

        // Node circle
        auto* ellipse = m_scene->addEllipse(cx, cy, sz, sz,
            QPen(QColor("#5aaaf0"), 2),
            QBrush(QColor("#2a4a6a")));
        ellipse->setZValue(2);
        ellipse->setData(0, b.id);  // store ID
        ellipse->setToolTip(QString("%1\n%2\n%3").arg(b.name, b.code, b.description));
        m_nodeItems[b.id] = ellipse;

        // Code text inside node
        auto* codeText = m_scene->addText(b.code);
        codeText->setDefaultTextColor(Qt::white);
        codeText->setFont(QFont("Arial", 6, QFont::Bold));
        QRectF cr = codeText->boundingRect();
        codeText->setPos(b.x - cr.width()/2, b.y - cr.height()/2);
        codeText->setZValue(3);

        // Name label below node
        auto* nameText = m_scene->addText(b.name);
        nameText->setDefaultTextColor(QColor("#aadaff"));
        nameText->setFont(labelFont);
        QRectF nr = nameText->boundingRect();
        nameText->setPos(b.x - nr.width()/2, b.y + NODE_RADIUS + 2);
        nameText->setZValue(3);
        m_labelItems[b.id] = nameText;
    }
}

void MapView::showRoute(const RouteResult& route) {
    clearRoute();
    if (!route.valid || route.buildingIds.size() < 2) return;

    m_currentRoute = route.buildingIds;
    m_animStep = 0;
    m_animTimer->start(120);
}

void MapView::animateRoute() {
    if (m_animStep >= m_currentRoute.size() - 1) {
        m_animTimer->stop();
        return;
    }

    int fromId = m_currentRoute[m_animStep];
    int toId   = m_currentRoute[m_animStep + 1];
    const Building* from = m_graph->findBuilding(fromId);
    const Building* to   = m_graph->findBuilding(toId);

    if (from && to) {
        QPen routePen(QColor("#ff6a30"), 4, Qt::SolidLine);
        routePen.setCapStyle(Qt::RoundCap);
        auto* line = m_scene->addLine(from->x, from->y, to->x, to->y, routePen);
        line->setZValue(5);
        m_routeLines.append(line);

        // Highlight nodes on route
        if (m_nodeItems.contains(fromId))
            m_nodeItems[fromId]->setBrush(QBrush(QColor("#ff6a30")));
        if (m_nodeItems.contains(toId))
            m_nodeItems[toId]->setBrush(QBrush(QColor("#ff6a30")));
    }

    m_animStep++;
}

void MapView::clearRoute() {
    m_animTimer->stop();
    for (auto* line : m_routeLines)
        m_scene->removeItem(line);
    m_routeLines.clear();

    // Reset node colours
    for (auto& b : m_graph->buildings()) {
        if (m_nodeItems.contains(b.id))
            m_nodeItems[b.id]->setBrush(QBrush(QColor("#2a4a6a")));
    }
    m_currentRoute.clear();
}

void MapView::highlightBuilding(int buildingId) {
    if (m_highlightRing) {
        m_scene->removeItem(m_highlightRing);
        m_highlightRing = nullptr;
    }

    const Building* b = m_graph->findBuilding(buildingId);
    if (!b) return;

    int r = NODE_RADIUS + 6;
    m_highlightRing = m_scene->addEllipse(
        b->x - r, b->y - r, r*2, r*2,
        QPen(QColor("#ffee00"), 3),
        QBrush(Qt::transparent));
    m_highlightRing->setZValue(4);

    // Centre view on building
    centerOn(b->x, b->y);
}

void MapView::refresh() {
    drawGraph();
}

void MapView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPointF scenePos = mapToScene(event->pos());
        QList<QGraphicsItem*> items = m_scene->items(
            scenePos, Qt::IntersectsItemShape, Qt::DescendingOrder);

        for (auto* item : items) {
            QVariant data = item->data(0);
            if (data.isValid()) {
                emit buildingClicked(data.toInt());
                return;
            }
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void MapView::wheelEvent(QWheelEvent* event) {
    double factor = (event->angleDelta().y() > 0) ? 1.15 : 0.87;
    scale(factor, factor);
}

void MapView::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);
    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}
