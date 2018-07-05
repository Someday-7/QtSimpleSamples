#ifndef LINEFLOWEFFECT_H
#define LINEFLOWEFFECT_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QWidget>
#include <QHBoxLayout>
#include <math.h>
#include <QDebug>
class LineFlowEffect : public QWidget
{
    Q_OBJECT
public:
    explicit LineFlowEffect(QWidget *parent = nullptr);
    ~LineFlowEffect();
    void SetPosition(QPoint StartPos,QPoint EndPos,int Width,int Angle);
    void StartEffect();
protected:
    void paintEvent(QPaintEvent *event);
public slots:

private slots:
    void UpdateTimerSlot();
signals:

private:
    QTimer* m_UpdateTimer;
    QPoint m_PointAStart;
    QPoint m_PointAEnd;
    QPoint m_PointBStart;
    QPoint m_PointBEnd;
    int m_Angle;
    int m_PipelineWidth;
};

#endif // LINEFLOWEFFECT_H
