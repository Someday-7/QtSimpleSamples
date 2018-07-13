/****************************************************************************
**Author: jingyu
**contact: 42839579@qq.com
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef LINEFLOWEFFECT_H
#define LINEFLOWEFFECT_H

#include <QWidget>
#include <QPainter>
#include <QWidget>
#include <QHBoxLayout>
#include <math.h>
#include <QDebug>
#include <QVector>
#include <QHash>
#define Pi 3.1415926
#define ENABLE_EFFECT_DEBUG_OUTPUT 0
typedef enum LineStatus{ARROW_IN = 0,ARROW_OUT}LineStatus;
typedef struct LinesCombinedParameter
{
    QPoint m_PointAStart;//第一根线的起点
    QPoint m_PointAEnd;//第一根线的终点
    bool m_DoubleLine;//是否要绘制双线平行
    int m_Angle;//线相对于视图坐标系偏移角度
    int m_MovementAngle;//移动的三角形组合，带方向的角度
    int m_DoubleLineDistance;//两根线之间距离
    int m_PolygonCount;//线上面总共有多少三角形
    double m_HypotenuseLength;//线段长度
    LinesCombinedParameter()
    {
        m_PointAStart = QPoint(0,0);//第一根线的起点
        m_PointAEnd = QPoint(0,0);//第一根线的终点
        m_DoubleLine = 0;//是否要绘制双线平行
        m_Angle = 0;//线相对于视图坐标系偏移角度
        m_MovementAngle = 0;//移动的三角形组合，带方向的角度
        m_DoubleLineDistance = 0;//两根线之间距离
        m_PolygonCount = 0;//线上面总共有多少三角形
        m_HypotenuseLength = 0.0;//线段长度
    }
}LinesCombinedParameter;
class LineFlowEffect : public QWidget
{
    Q_OBJECT
public:
    explicit LineFlowEffect(QWidget *parent = nullptr);
    ~LineFlowEffect();
    void SetPosition(QPoint StartPos,QPoint EndPos,int Width);//暂时只支持直角
    void SetPosition(QVector<QPoint>LinesVector,int Width);//暂时只支持直角
    void SetArrowStyle(Qt::PenStyle Style,QColor Color);
    void SetArrowDirection(LineStatus Line);
    void EnbaleActiveDisplay(bool Status);
    void paintEffect(QPainter& painter);
    bool GetLineStatus();
private:
    QPen m_Pen;//enable
    QPen m_GrayPen;//disabled
    QBrush m_Brush;//enable
    QBrush m_GrayBrush;//disabled
    QPoint m_PointAStart;//第一根线的起点
    QPoint m_PointAEnd;//第一根线的终点
    QPoint m_PointBStart;//第二根线的起点
    QPoint m_PointBEnd;//第二根线的终点
    bool m_DoubleLine;//是否要绘制双线平行
    int m_Angle;//线相对于视图坐标系偏移角度
    int m_MovementAngle;//移动的三角形组合，带方向的角度
    int m_DoubleLineDistance;//两根线之间距离
    int m_PolygonCount;//线上面总共有多少三角形
    int m_CurrentForwardPolygonIndex;//当前领头滚动的三角形下标正向
    int m_CurrentReversePolygonIndex;//当前领头滚动的三角形下标反向
    double m_HypotenuseLength;//线段长度
    bool m_IsMoreLine;//判断是绘制双线还是单线
    int m_LineDirection;//箭头效果的移动方向，以起始点作为输出或者输入的判定
    QHash<int,LinesCombinedParameter> m_LineNumberParaHash;//绑定一个排序下标
    bool CurrentDisplayStatus[2];//当前线段是否被使能，否的话就是灰色
    int* m_MultiLinesPolygonIndex;//做三个三角形移动效果的下标控制，根据所在线段的比例位置进行移动位置控制
    int m_DisplayCurrentLineEffect;//当前需要移动箭头效果的线段
};

#endif // LINEFLOWEFFECT_H
