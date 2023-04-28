#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTime>
#include <QToolBar>  // add this header file

class TimelineWidget : public QGraphicsView
{
    Q_OBJECT

public:
    explicit TimelineWidget(QWidget *parent = nullptr);

    void readEventsFromFil(const QString &fileName);
    void fitInView();

protected:
    // Event handlers for mouse events
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void createTimeline();
    void addRandomEvents(int count);
    void addEvent(const QString eventCode, const QTime &time);

    // new private function

    QGraphicsScene m_scene;
    QPoint m_lastMousePos;

    QList<QPair<QTime, QString>> m_events;
    QTime m_maxTime;

};

#endif // TIMELINEWIDGET_H
