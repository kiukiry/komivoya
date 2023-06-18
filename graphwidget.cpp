#include "graph.h"
#include "graphwidget.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QRectF>
#include <QtMath>
#include <cmath>
void GraphWidget::reshGraph(const Graph& graph, const PathInfo& optimalPath) {


    // Очистить сцену
    scene()->clear();

    // Получить количество вершин в графе
    int numVertices = graph.getNumVertices();

    // Вычислить положения вершин в круге для визуализации
    const int sceneWidth = 1000;
    const int sceneHeight = 1000;
    const QPointF center(sceneWidth / 2, sceneHeight / 2);
    const qreal radius = qMin(sceneWidth, sceneHeight) * 0.4;
    const qreal angleIncrement = 2 * M_PI / numVertices;

    std::vector<QPointF> vertexPositions;
    for (int i = 0; i < numVertices; i++) {
        qreal angle = i * angleIncrement;
        qreal x = center.x() + radius * qCos(angle);
        qreal y = center.y() + radius * qSin(angle);
        vertexPositions.emplace_back(x, y);
    }

    // Нарисовать рёбра в графе
    drawEdges(graph, vertexPositions);

    // Нарисовать вершины в графе
    drawVertices(vertexPositions);

    // Нарисовать оптимальный путь
    drawPath(optimalPath.path, vertexPositions);
}

void GraphWidget::visGraph(const Graph& graph)
{
    // Очистить сцену
    scene()->clear();

    // Получить количество вершин в графе
    int numVertices = graph.getNumVertices();

    // Вычислить положения вершин в круге для визуализации
    const int sceneWidth = 1000;
    const int sceneHeight = 1000;
    const QPointF center(sceneWidth / 2, sceneHeight / 2);
    const qreal radius = qMin(sceneWidth, sceneHeight) * 0.4;
    const qreal angleIncrement = 2 * M_PI / numVertices;

    std::vector<QPointF> vertexPositions;
    for (int i = 0; i < numVertices; i++) {
        qreal angle = i * angleIncrement;
        qreal x = center.x() + radius * qCos(angle);
        qreal y = center.y() + radius * qSin(angle);
        vertexPositions.emplace_back(x, y);
    }

    // Нарисовать рёбра в графе
    drawEdges(graph, vertexPositions);

    // Нарисовать вершины в графе
    drawVertices(vertexPositions);
}

void GraphWidget::drawEdges(const Graph& graph, const std::vector<QPointF>& vertexPositions) {
    const int numVertices = graph.getNumVertices();
    const QPen edgePen(Qt::black);
    QFont font("Arial", 10, QFont::Bold);
    QColor fontColor(255, 255, 0);

    for (int v1 = 0; v1 < numVertices; v1++) {
        for (int v2 = 0; v2 < numVertices; v2++) {
            int weight = graph.getEdgeWeight(v1, v2);
            if (weight > 0) {
                QPointF p1 = vertexPositions[v1];
                QPointF p2 = vertexPositions[v2];
                QGraphicsLineItem* line = scene()->addLine(p1.x(), p1.y(), p2.x(), p2.y(), edgePen);
                QGraphicsSimpleTextItem* text = scene()->addSimpleText(QString::number(weight));
                text->setFont(font);
                text->setBrush(fontColor);
                text->setPos((p1 + p2) / 2);
            }
        }
    }
}
void GraphWidget::drawVertices(const std::vector<QPointF>& vertexPositions) {
    const int numVertices = vertexPositions.size();
    const int vertexRadius = 30;
    const QPen vertexPen(Qt::black);
    const QBrush vertexBrush(Qt::yellow);

    for (int v = 0; v < numVertices; v++) {
        QPointF position = vertexPositions[v];
        QRectF rect(position.x() - vertexRadius, position.y() - vertexRadius,
                    2 * vertexRadius, 2 * vertexRadius);
        QGraphicsEllipseItem* ellipse = scene()->addEllipse(rect, vertexPen, vertexBrush);

        // Добавьте метки к вершинам
        QGraphicsTextItem* label = scene()->addText(QString::number(v+1));
        QFont labelFont("Arial", 14, QFont::Bold); // Установка шрифта и размера метки
        label->setFont(labelFont);
        label->setDefaultTextColor(Qt::black); // Установка цвета текста метки
        QRectF textRect = label->boundingRect(); // Получение прямоугольника, описывающего текст метки
        label->setPos(position - QPointF(textRect.width()/2, textRect.height()/2)); // Установка позиции метки в центре вершины
        ellipse->setToolTip(QString("Вершина %1").arg(v+1)); // Установка всплывающей подсказки с номером вершины
    }
}

void GraphWidget::drawPath(const std::vector<int>& path, const std::vector<QPointF>& vertexPositions) {
    if (path.empty())
        return;

    const int pathPenWidth = 2;
    const QColor pathColor = Qt::red;
    const QPen pathPen(pathColor, pathPenWidth);

    const int numVertices = path.size();
    for (int i = 0; i < numVertices - 1; i++) {
        int v1 = path[i];
        int v2 = path[i + 1];
        QPointF p1 = vertexPositions[v1];
        QPointF p2 = vertexPositions[v2];
        scene()->addLine(p1.x(), p1.y(), p2.x(), p2.y(), pathPen);
    }

    // Соедините последнюю и первую вершины пути
    int v1 = path.back();
    int v2 = path.front();
    QPointF p1 = vertexPositions[v1];
    QPointF p2 = vertexPositions[v2];
    scene()->addLine(p1.x(), p1.y(), p2.x(), p2.y(), pathPen);
}
