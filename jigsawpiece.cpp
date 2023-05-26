#include "jigsawpiece.h"

JigsawPiece::JigsawPiece(int id, QWidget* parent)
    : JigsawLabel{parent}
    , m_id(id)
    , m_selected(false)
    , m_draggedDistance(0)
    , m_dragged(false)
    , m_cursorOffset(QPointF(0.0, 0.0))
    , m_angle(0)
    , m_startingAngle(0)
    , m_rotated(false)
    , m_rotationEnabled(true)
    , m_dragEnabled(true)
    , m_moveTimer(new QTimer(this))
    , m_timerStartTime(QDateTime::currentDateTime())
{
    expandGeometryForRotation();
    QObject::connect(m_moveTimer, &QTimer::timeout, this, &JigsawPiece::moveTimerTimeOut);
}

JigsawPiece::JigsawPiece(int id, const QPixmap &background, QWidget *parent)
    : JigsawLabel{background, parent}
    , m_id(id)
    , m_selected(false)
    , m_draggedDistance(0)
    , m_dragged(false)
    , m_cursorOffset(QPointF(0.0, 0.0))
    , m_angle(0)
    , m_startingAngle(0)
    , m_rotated(false)
    , m_rotationEnabled(true)
    , m_dragEnabled(true)
    , m_moveTimer(new QTimer(this))
    , m_timerStartTime(QDateTime::currentDateTime())
{
    expandGeometryForRotation();
    QObject::connect(m_moveTimer, &QTimer::timeout, this, &JigsawPiece::moveTimerTimeOut);
}

JigsawPiece::JigsawPiece(int id, const QSize &size, const QBrush &background, const QPainterPath &jigsawPath, QWidget *parent)
    : JigsawLabel{size, background, jigsawPath, parent}
    , m_id(id)
    , m_selected(false)
    , m_draggedDistance(0)
    , m_dragged(false)
    , m_cursorOffset(QPointF(0.0, 0.0))
    , m_angle(0)
    , m_startingAngle(0)
    , m_rotated(false)
    , m_rotationEnabled(true)
    , m_dragEnabled(true)
    , m_moveTimer(new QTimer(this))
    , m_timerStartTime(QDateTime::currentDateTime())
{
    expandGeometryForRotation();
    QObject::connect(m_moveTimer, &QTimer::timeout, this, &JigsawPiece::moveTimerTimeOut);
}

JigsawPiece::~JigsawPiece()
{
    
}

void JigsawPiece::setPixmap(const QPixmap &newPixmap)
{
    JigsawLabel::setPixmap(newPixmap);
    redraw();
}

void JigsawPiece::setJigsawPath(const QPainterPath &newJigsawPath, const QSize &newSize, const QBrush &newBrush)
{
    JigsawLabel::setJigsawPath(newJigsawPath, newSize, newBrush);
    redraw();
}

void JigsawPiece::setJigsawPath(const QPainterPath &newJigsawPath, const QSize &newSize)
{
    JigsawLabel::setJigsawPath(newJigsawPath, newSize);
    redraw();
}

void JigsawPiece::setJigsawPath(const QPainterPath &newJigsawPath)
{
    JigsawLabel::setJigsawPath(newJigsawPath);
    redraw();
}

void JigsawPiece::setBorderPen(const QPen &newBorderPen)
{
    JigsawLabel::setBorderPen(newBorderPen);
    redraw();
}

void JigsawPiece::setAngle(int newAngle)
{
    if (newAngle == m_angle) return;
    m_angle = newAngle;
    redraw();
    emit rotated(m_id, m_angle, mapToParent(m_maxRectForRotation.center() - m_maxRectForRotation.topLeft()));
}

/*
 * If you use this function without the constDistance and constOriginalAngle values, it gets very unprecise. The
 * JigsawPiece you rotate around the given point will subsequently move away from the position it should be moved
 * to. The more often the function is called, the larger this deviation becomes.
 */

void JigsawPiece::rotateAroundPoint(int angle, const QPointF &point, double constDistance, double constOriginalAngle)
{
    //Case 1: No original angle was set
    //Case 2: Original angle was set, but no valid distance was set

    if (constOriginalAngle <= -360.0 || constDistance <= 0.0) {
        int angleDif = m_angle - angle;
        setAngle(angle);
        QLineF line(point, mapToParent(m_maxRectForRotation.center() - m_maxRectForRotation.topLeft()));
        line.setAngle(line.angle() - angleDif);
        if (constDistance > 0.0) line.setLength(constDistance);
        QPointF newPosition = line.p2();
        move(newPosition - m_maxRectForRotation.center());
        if (angle == 0) emit requestPositionValidation(m_id);
    }

    //Case 3: Original angle was set and valid distance was set

    else {
        double newAngleForLine = constOriginalAngle + angle;
        setAngle(angle);
        QLineF line;
        line.setP1(point);
        line.setAngle(newAngleForLine);
        line.setLength(constDistance);
        QPointF newPosition = line.p2();
        move(newPosition - m_maxRectForRotation.center());
    }
}

int JigsawPiece::angle() const
{
    return m_angle;
}

void JigsawPiece::move(const QPointF &pos)
{
    m_actualPosition = pos + m_maxRectForRotation.topLeft();
    QLabel::move(m_actualPosition.toPoint());
    m_originalPosition = pos;
}

void JigsawPiece::move(double x, double y)
{
    m_actualPosition = QPointF(x, y) + m_maxRectForRotation.topLeft();
    QLabel::move(m_actualPosition.toPoint());
    m_originalPosition = QPoint(x, y);
}

bool JigsawPiece::rotationIsEnabled() const
{
    return m_rotationEnabled;
}

bool JigsawPiece::dragIsEnabled() const
{
    return m_dragEnabled;
}

void JigsawPiece::setRotationEnabled(bool val)
{
    m_rotationEnabled = val;
}

void JigsawPiece::setDragEnabled(bool val)
{
    m_dragEnabled = val;
}

int JigsawPiece::id() const
{
    return m_id;
}

void JigsawPiece::expandGeometryForRotation()
{
    int oldWidth = pixmap().size().width();
    int oldHeight = pixmap().size().height();
    double radius = qSqrt(qPow(1.0 * oldHeight / 2, 2) + qPow(1.0 * oldWidth / 2, 2));
    double newWidth = 2.0 * radius;
    double newHeight = newWidth;
    double offsetWidth = (newWidth - oldWidth) / 2;
    double offsetHeight = (newHeight - oldHeight) / 2;
    m_maxRectForRotation = QRectF(QPointF(-offsetWidth, -offsetHeight), QSizeF(newWidth, newHeight));
    m_actualPosition = m_originalPosition + m_maxRectForRotation.topLeft();
    setGeometry(m_maxRectForRotation.translated(m_originalPosition).toRect());
    redraw();
}

void JigsawPiece::moveTimerTimeOut()
{
    QPointF newPosition = QCursor::pos() + m_cursorOffset;
    QPointF draggedBy = newPosition - m_originalPosition;
    if (draggedBy.isNull()) return;
    move(newPosition);
    emit dragged(m_id, draggedBy);
    if (m_timerStartTime.msecsTo(QDateTime::currentDateTime()) >= DROPPEDAFTERMSECS) {
        m_moveTimer->stop();
        m_selected = false;
        m_dragged = false;
        emit dragStopped(m_id);
    }
}

void JigsawPiece::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_dragEnabled && !m_rotated) {
        m_cursorOffset = m_originalPosition - mapToGlobal(event->pos());
        m_selected = !m_selected;
        if (m_selected) {
            m_moveTimer->start(1000 / FPS);
            m_timerStartTime = QDateTime::currentDateTime();
            emit dragStarted(m_id);
        }
        else {
            m_moveTimer->stop();
            emit dragStopped(m_id);
        }
        emit leftClicked(m_id);
        return;
    }
    else if (event->button() == Qt::RightButton && !m_selected) {
        m_startingAngle = QLineF(m_maxRectForRotation.center(), event->pos() + m_maxRectForRotation.topLeft()).angle() - m_angle;
        m_rotated = true;
        emit rotateStarted(m_id);
        emit rightClicked(m_id);
    }
    emit clicked(m_id);
}

/*
 * To prevent users from accidently dragging a JigsawPiece, because they moved the cursor a little bit while they clicked, there is
 * a minimum drag distance. You have to drag the JigsawPiece by this distance, before the piece is dropped on mouse button release.
 * Otherwise it still is marked as selected and dragged until you click it again.
 */

void JigsawPiece::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_selected && m_dragged && m_draggedDistance >= MINDRAGDISTANCE) {
        m_selected = false;
        m_moveTimer->stop();
        emit dragStopped(m_id);
    }
    m_draggedDistance = 0;
    m_dragged = false;
    if (m_rotated) emit rotateStopped(m_id);
    m_rotated = false;
    emit released(m_id);
}
/*
 * If you rotate a JigsawPiece, you have to right click it and keep the right button pressed down. A starting angle is calculated when you
 * right click. It takes into account the angle the JigsawPiece might already be rotated by. When you move the cursor, a new angle ist calculated
 * between the center of the JigsawPiece and the cursor position. The starting angle is subtracted from this and the JigsawPiece's angle is set
 * to the difference. The pixmap is redrawn with the new angle and the rotated() signal is emitted. The rotation takes place in steps that are
 * defined via the MINROTATEANGLE constant, which is 10 (degrees) by default.
 */

void JigsawPiece::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && m_dragEnabled && !m_rotated) {
        m_dragged = true;
        ++m_draggedDistance;
    }
    else if (event->buttons() == Qt::RightButton && m_rotationEnabled && !m_selected) {
        int angle = QLineF(m_maxRectForRotation.center(), event->pos() + m_maxRectForRotation.topLeft()).angle();
        angle -= m_startingAngle;
        angle %= 360;
        if (angle < 0) angle += 360;
        setAngle((angle / MINROTATEANGLE) * MINROTATEANGLE);
    }
}

void JigsawPiece::enterEvent(QEnterEvent *event)
{
    emit entered(m_id);
}

void JigsawPiece::leaveEvent(QEvent *event)
{
    emit left(m_id);
}

void JigsawPiece::redraw()
{
    QPixmap rotatedPixmap(m_maxRectForRotation.toRect().size());
    rotatedPixmap.fill(Qt::transparent);

    QTransform transform;
    transform.translate(m_maxRectForRotation.width() / 2, m_maxRectForRotation.height() / 2);
    transform.rotate(-m_angle);
    transform.translate(m_maxRectForRotation.width() / -2, m_maxRectForRotation.height() / -2);
    transform.translate(-m_maxRectForRotation.left(), -m_maxRectForRotation.top());

    QPainter painter(&rotatedPixmap);
    QBrush brush = m_brush;
    QPainterPath path = jigsawPath();

    switch (m_mode) {
    case JigsawLabel::Mode::PIXMAP:
        painter.setTransform(transform);
        painter.drawPixmap(JigsawLabel::pixmap().rect(), JigsawLabel::pixmap(), JigsawLabel::pixmap().rect());
        break;
    case JigsawLabel::Mode::BRUSH:
        painter.setTransform(transform);
        painter.setPen(borderPen());
        painter.setBrush(brush);
        painter.drawPath(path);
        break;
    default:
        return;
    }

    QLabel::setPixmap(rotatedPixmap);
    setMask(rotatedPixmap.mask());
}
