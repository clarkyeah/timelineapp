#include "timelinewidget.h"
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QRandomGenerator>
#include <QTime>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <string>

TimelineWidget::TimelineWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(&m_scene);
//    createTimeline();
//    addRandomEvents(10);

    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);
    setOptimizationFlag(QGraphicsView::DontSavePainterState, true);
    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);


}

void TimelineWidget::fitInView()
{
    QRectF rect = m_scene.itemsBoundingRect();
    if (rect.isNull()) {
        return;
    }
    rect.adjust(-20, -20, 20, 20);
    QGraphicsView::fitInView(rect, Qt::KeepAspectRatio);
    centerOn(0, 0);
}


void TimelineWidget::wheelEvent(QWheelEvent *event)
{
    const double zoomFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(zoomFactor, zoomFactor);
    } else {
        scale(1 / zoomFactor, 1 / zoomFactor);
    }
}

void TimelineWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_lastMousePos = event->pos();
    }
    QGraphicsView::mousePressEvent(event);
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPointF delta = mapFromScene(event->pos()) - mapFromScene(m_lastMousePos);
        translate(delta.x(), delta.y());
        m_lastMousePos = event->pos();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

void TimelineWidget::createTimeline()
{
     int maxMinutes = m_maxTime.hour() * 60 + m_maxTime.minute();
     int timelineWidth = maxMinutes * 4;
    
    // Draw the timeline
    auto timeline = new QGraphicsLineItem(0, 0, timelineWidth, 0);
    m_scene.addItem(timeline);

    // Add time step labels
    int stepCount = maxMinutes / 10;
    for (int i = 0; i <= stepCount; ++i) {
        auto label = new QGraphicsTextItem(QString::number(i * 10));
        label->setPos(i * 40 - label->boundingRect().width() / 2, -30);
        m_scene.addItem(label);
    }
}


void TimelineWidget::addRandomEvents(int count)
{
    for (int i = 0; i < count; ++i) {
        // Generate a random time step and position
        int timeStep = QRandomGenerator::global()->bounded(0, 1000);
        int yPos = QRandomGenerator::global()->bounded(20, 100);

        // Create an event item and add it to the scene
        auto eventItem = new QGraphicsEllipseItem(timeStep - 5, yPos - 5, 10, 10);
        eventItem->setBrush(Qt::blue);
        m_scene.addItem(eventItem);
    }
}


void TimelineWidget::readEventsFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Cannot open file."));
        return;
    }

    m_events.clear();
    m_maxTime = QTime(0, 0);

    QTextStream in(&file);
    in.readLine(); // Skip the header line

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (fields.size() != 2) {
            continue;
        }

        QTime timestamp = QTime::fromString(fields[0].trimmed(), "hh:mm");
        if (!timestamp.isValid()) {
            continue;
        }

        if (timestamp > m_maxTime) {
            m_maxTime = timestamp;
        }

        m_events.append({timestamp, fields[1].trimmed()});
    }

    file.close();

    // Clear the existing items and create a new timeline
    m_scene.clear();
    createTimeline();
    // Add events to the timeline
    for (const auto &event : m_events) {
        addEvent(event.second, event.first);
    }

    update(); // Update the timeline widget
}

void TimelineWidget::addEvent(const QString eventCode, const QTime &time)
{
    const qreal labelSpacing = 15.0; // Adjust this value based on your desired spacing
    qreal x = (time.hour() * 60 + time.minute()) * 4;

    QGraphicsEllipseItem *dot = new QGraphicsEllipseItem(x - 2.5, -2.5, 5, 5);
    dot->setBrush(Qt::red);
    m_scene.addItem(dot);

    QGraphicsTextItem *label = new QGraphicsTextItem(eventCode);
    label->setDefaultTextColor(Qt::black);

    qreal yPos = 5;
    bool positionFound = false;
    while (!positionFound) {
        positionFound = true;
        QRectF searchRect(x - label->boundingRect().width() / 2, yPos, label->boundingRect().width(), label->boundingRect().height());
        for (const auto &otherLabel : m_scene.items(searchRect)) {
            if (otherLabel->type() == QGraphicsTextItem::Type) {
                yPos += labelSpacing;
                positionFound = false;
                break;
            }
        }
    }

    label->setPos(x - label->boundingRect().width() / 2, yPos);
    m_scene.addItem(label);
}


//void TimelineWidget::addEvent(const QString eventCode, const QTime &time)
//{
//    qreal x = (time.hour() * 60 + time.minute()) * 4;
//
//    QGraphicsEllipseItem *dot = new QGraphicsEllipseItem(x - 2.5, -2.5, 5, 5);
//    dot->setBrush(Qt::red);
//    m_scene.addItem(dot);
//
//    QGraphicsTextItem *label = new QGraphicsTextItem((eventCode));
//    label->setDefaultTextColor(Qt::black);
//    label->setPos(x - label->boundingRect().width() / 2, 5);
//    m_scene.addItem(label);
//}