#include "BreatheEffect.h"
#include <QDebug>

BreatheEffect::BreatheEffect(QWidget *parent) :QWidget(parent)
{
	m_FontSize = 32;
	m_Interval = 1000;
	m_Name = QStringLiteral("NULL");
	m_IsEnable = false;
	m_TextColor = QColor(255, 255, 255);
	m_BackGroundColor = QColor(195, 195, 195);

	m_Step = 10;
	m_Interval = 100;
	m_Offset = 0;
	m_Add = false;
	m_TestLabel = new QLabel;
	m_TextTotalLength = 0;

	m_UpdateTimer = new QTimer(this);
	connect(m_UpdateTimer, SIGNAL(timeout()), this, SLOT(UpdateTimerSlot()));
}
BreatheEffect::~BreatheEffect()
{
	delete m_UpdateTimer;
}
void BreatheEffect::SetUpdateInterval(int& Interval)
{
	m_Interval = Interval;
	if (m_Interval <= 0)
	{
		return;
	}
	m_UpdateTimer->setInterval(m_Interval);
}
void BreatheEffect::SetShowTextName(QString& Name,int& FontSize,QColor& Color)
{
	m_Name = Name;
	m_FontSize = FontSize;
	m_TextColor = Color;

	m_Font.setFamily(QStringLiteral("楷体"));
	m_Font.setPointSize(FontSize);
	m_TestLabel->setFont(m_Font);
	m_FontWidth = m_TestLabel->fontMetrics().width(QChar('A'));
	qDebug() << "[MainWindow] Font Width:" << m_FontWidth;
	m_FontHeight = m_TestLabel->fontMetrics().leading();
	qDebug() << "[MainWindow] Font height:" << m_FontHeight;

    m_Pen.setWidth(1);
    m_Pen.setColor(Qt::blue);
    m_Pen.setStyle(Qt::SolidLine);
    m_Pen.setCapStyle(Qt::RoundCap);
    m_Pen.setJoinStyle(Qt::RoundJoin);

	m_TextTotalLength = m_Name.length();
}
void BreatheEffect::SetEnableUpdate(bool& IsEnable)
{
	m_IsEnable = IsEnable;
	if (m_IsEnable)
	{
		if (!m_UpdateTimer->isActive())
		{
			m_UpdateTimer->start();
		}
	}
	else
	{
		if (m_UpdateTimer->isActive())
		{
			m_UpdateTimer->stop();
		}
	}
}
void BreatheEffect::SetBackGroudColor(QColor& Color)
{
	m_BackGroundColor = Color;
}
void BreatheEffect::UpdateTimerSlot()
{
	update();
}
void BreatheEffect::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	// 绘制准备工作 启用反锯齿
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	//绘制背景
	// 半径为当前 宽 或者 高 的一半
	int radius = qMin(this->width(), this->height()) / 2;
	// 以点为中心的渐变色
    QRadialGradient radialGradient(QPoint(radius / 2, radius / 2), radius);
	// 循环加减
	(m_Offset < 100 && m_Add) ? (m_Offset += m_Step) : (m_Add = false);
	(m_Offset > 0 && !m_Add) ? (m_Offset -= m_Step) : (m_Add = true);
	// 按照 点范围[0.0,1.0] 对于 每点的颜色
	m_BackGroundColor.setAlpha(200 + m_Offset > 255 ? 255 : 200 + m_Offset);
	radialGradient.setColorAt(0.0, m_BackGroundColor);
	m_BackGroundColor.setAlpha(140 + m_Offset);
	radialGradient.setColorAt(0.2, m_BackGroundColor);
	m_BackGroundColor.setAlpha(80 + m_Offset);
	radialGradient.setColorAt(0.4, m_BackGroundColor);
	m_BackGroundColor.setAlpha(20 + m_Offset >= 0 ? 20 + m_Offset : 0);
	radialGradient.setColorAt(0.6, m_BackGroundColor);
	m_BackGroundColor.setAlpha(-60 + m_Offset >= 0 ? -50 + m_Offset : 0);
	radialGradient.setColorAt(0.8, m_BackGroundColor);
	m_BackGroundColor.setAlpha(0);
	radialGradient.setColorAt(1.0, m_BackGroundColor);
	// 设置 画笔 图形的边界线

	painter.setPen(Qt::NoPen);
	// 设置 画刷 画刷为 点向外辐射的渐变色
	painter.setBrush(radialGradient);
	// 画椭圆，长=宽 为原型
	painter.drawEllipse(0, 0, radius, radius);

	painter.setFont(m_Font);
	painter.setPen(m_Pen);
    painter.drawText(radius / 2 - (m_FontWidth * m_TextTotalLength) / 2, radius / 2 + m_FontHeight / 2, m_Name);
}
