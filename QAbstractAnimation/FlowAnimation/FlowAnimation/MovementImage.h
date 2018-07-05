#ifndef MOVEMENTIMAGE_H
#define MOVEMENTIMAGE_H
#include "interface.h"
#include <QLabel>
#include <QPropertyAnimation>
class MovementImage : public QLabel
{
    Q_OBJECT
public:
    explicit MovementImage(QWidget* parent);
    ~MovementImage();
    void StartAnimation(QRect ShapeRect);
    void SetAnimationDirection(AnimationDirection Direction,int RotateValue);
private:
    QPropertyAnimation* m_Animation;
    AnimationDirection m_CurrentDirection;
    int m_CurrentRotate;
};

#endif // MOVEMENTIMAGE_H
