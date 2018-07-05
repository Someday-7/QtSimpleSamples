#pragma once
#include "qwidget.h"
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPen>
#include <QPainter>
class BreatheEffect :public QWidget
{
	Q_OBJECT
public:
    explicit BreatheEffect(QWidget *parent = 0);
    ~BreatheEffect();
	void SetUpdateInterval(int& Interval);
	void SetShowTextName(QString& Name, int& FontSize, QColor& Color);
	void SetEnableUpdate(bool& IsEnable);
	void SetBackGroudColor(QColor& Color);
protected:
	void paintEvent(QPaintEvent *event);
	private slots:
	void UpdateTimerSlot();
private:
	int m_FontSize;
	QString m_Name;
	bool m_IsEnable;
	QColor m_TextColor;
	QColor m_BackGroundColor;
	QFont m_Font;
	QPen m_Pen;
	QTimer* m_UpdateTimer;

	//update control
	int m_Step;
	int m_Interval;
	int m_Offset;
	bool m_Add;
	QLabel* m_TestLabel;
	int m_FontWidth;
	int m_FontHeight;
	int m_TextTotalLength;
};

