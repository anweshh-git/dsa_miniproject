#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QMap>
#include <QTimer>

#include "core/graph/Graph.h"
#include "core/models/Path.h"

class MapView : public QGraphicsView {
    Q_OBJECT
public:
    explicit MapView(Graph* graph, QWidget* parent = nullptr);

    void showRoute(const RouteResult& route);
    void clearRoute();
    void highlightBuilding(int buildingId);
    void refresh();

signals:
    void buildingClicked(int buildingId);
    void routeRequested(int fromId, int toId);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void drawGraph();
    void drawBuildings();
    void drawEdges();

    void highlightEdges(const QVector<int>& route);
    void animateRoute();

    Graph*          m_graph;
    QGraphicsScene* m_scene;

    // Stored graphics items
    QMap<int, QGraphicsEllipseItem*> m_nodeItems;
    QMap<int, QGraphicsTextItem*>    m_labelItems;
    QVector<QGraphicsLineItem*>      m_routeLines;
    QGraphicsEllipseItem*            m_highlightRing;

    QVector<int>   m_currentRoute;
    int            m_animStep;
    QTimer*        m_animTimer;

    static const int NODE_RADIUS = 18;
};
