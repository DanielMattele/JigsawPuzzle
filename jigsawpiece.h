#ifndef JIGSAWPIECE_H
#define JIGSAWPIECE_H

#include "jigsawlabel.h"
#include <QDateTime>
#include <QMouseEvent>
#include <QTimer>

/*
 * The JigsawPiece class inherits JigsawLabel. You can drag and rotate a JigsawPiece by clicking on it. For the rotation
 * it is necessary to change the size of the pixmap, otherwise it would be cut off at the edges. To accomplish that,
 * a rectangle is calculated in the calculateMaxRectForRotation() function. The geometry is set to this rectangle, but
 * the original position is stored in a member variable. If you use the (overridden) move function, the JigsawPiece is
 * moved, as if the extended geometry wouldn't exist. Other functions of QLabel on the other hand (like pos()) might give
 * you not the values you expected, so be carefull with them.
 *
 * There are two ways to drag a JigsawPiece: First, you can just drag it with the left mouse button pressed down. The
 * JigsawPiece is dropped when the button is released. Second, you can click on a piece and mark it as selected. It is
 * then moved along with the cursor. If you click it again, it is unselected and dropped. In rare cases it is possible a
 * JigsawPiece isn't dropped if you click it again, because the cursor is not inside its borders anymore. Therefore a
 * JigsawPiece is always dropped after a certain time (10 secs by default).
 * 
 * There are two important signals in this class:
 *
 * dragged(int id, const QPointF &draggedBy)
 * This signal is emitted every time the JigsawPiece is dragged by one of the two ways to drag a JigsawPiece described above.
 * It can be used to move other JigsawPieces which are already merged in a jigsaw puzzle game.
 *
 * rotated(int id, int angle, const QPointF &rotatingPoint)
 * This signal is emitted every time the JigsawPiece is rotated. It doesn't work right now, but later it should be used to
 * rotate merged pieces with the rotateAroundPoint function.
 *
 * It is not allowed to draw text onto a JigsawPiece, so some of the functions implemented in JigsawLabel are deleted.
 */

class JigsawPiece : public JigsawLabel
{
    Q_OBJECT
private:
    static constexpr unsigned int MINDRAGDISTANCE = 5;
    static constexpr unsigned int MINROTATEANGLE = 10;
    static constexpr unsigned int FPS = 60;
    static constexpr unsigned int DROPPEDAFTERMSECS = 10000;

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
