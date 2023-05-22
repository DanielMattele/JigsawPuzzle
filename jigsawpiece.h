#ifndef JIGSAWPIECE_H
#define JIGSAWPIECE_H

#include "jigsawlabel.h"
#include <QDateTime>
#include <QMouseEvent>
#include <QTimer>

class JigsawPiece : public JigsawLabel
{
    Q_OBJECT
private:
    const unsigned int MINDRAGDISTANCE = 5;
    const unsigned int MINROTATEANGLE = 10;
    const unsigned int FPS = 60;

    int m_id;
    bool m_selected;
    bool m_dragged;
    unsigned int m_draggedDistance;
    QPointF m_cursorOffset;
    int m_angle;
    int m_startingAngle;
    bool m_rotated;
    bool m_rotationEnabled;
    bool m_dragEnabled;

    QRectF m_maxRectForRotation;
    void calculateMaxRectForRotation();

    QPointF m_actualPosition;
    QTimer* m_moveTimer;
    QDateTime m_timerStartTime;

private slots:
    void moveTimerTimeOut();

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    void redraw();

public:
    explicit JigsawPiece(int id, QWidget* parent = nullptr);
    explicit JigsawPiece(int id, const QPixmap &background, QWidget* parent = nullptr);
    explicit JigsawPiece(int id, const QSize &size, const QBrush &background, const QPainterPath &jigsawPath, QWidget* parent = nullptr);
    ~ JigsawPiece();

    void setText(const QString &newText) = delete;
    void setTextArea(const QRect &newTextArea) = delete;
    void setFont(const QFont &newFont) = delete;
    void setTextColor(const QColor &newTextColor) = delete;
    void setAlignment(const QFlags<Qt::AlignmentFlag> &newAlignment) = delete;

    void setPixmap(const QPixmap &newPixmap);
    void setJigsawPath(const QPainterPath &newJigsawPath, const QSize &newSize, const QBrush &newBrush);
    void setJigsawPath(const QPainterPath &newJigsawPath, const QSize &newSize);
    void setJigsawPath(const QPainterPath &newJigsawPath);
    void setBorderPen(const QPen &newBorderPen);

    void setAngle(int newAngle);
    int angle() const;

    void move(const QPoint &pos) = delete;
    void move(int x, int y) = delete;

    void move(const QPointF &pos);
    void move(double x, double y);

    bool rotationIsEnabled() const;
    bool dragIsEnabled() const;

    int id() const;

public slots:
    void setRotationEnabled(bool val = true);
    void setDragEnabled(bool val = true);

    void rotateAroundPoint(int angle, const QPointF &point); // works only for squares!

signals:
    void clicked(int id);
    void rightClicked(int id);
    void leftClicked(int id);
    void released(int id);
    void dragged(int id, const QPointF &draggedBy);
    void dragStarted(int id);
    void dragStopped(int id);
    void rotated(int id, int angle, const QPointF &rotatingPoint);
    void rotateStopped(int id);
    void entered(int id);
    void left(int id);
};

#endif // JIGSAWPIECE_H
