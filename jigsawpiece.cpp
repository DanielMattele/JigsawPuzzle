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
{
    calculateMaxRectForRotation();
    setGeometry(m_maxRectForRotation.toRect());
    m_actualPosition = m_maxRectForRotation.topLeft();
    redraw();

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
{
    calculateMaxRectForRotation();
    setGeometry(m_maxRectForRotation.toRect());
    m_actualPosition = m_maxRectForRotation.topLeft();
    redraw();

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
{
    calculateMaxRectForRotation();
    setGeometry(m_maxRectForRotation.toRect());
    m_actualPosition = m_maxRectForRotation.topLeft();
    redraw();

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
    newAngle %= 360;
    if (newAngle < 0) newAngle += 360;
    if (newAngle == m_angle) return;
    m_angle = newAngle;
    redraw();
    emit rotated(m_id, m_angle, m_actualPosition);
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

void JigsawPiece::rotateAroundPoint(int angle, const QPointF &point)
{
    int angleDif = m_angle - angle;
    setAngle(angle);
    QLineF line(point, m_actualPosition);
    line.setAngle(line.angle() + angleDif);
    QPointF newPosition = line.p2();
    move(newPosition - m_maxRectForRotation.topLeft());
}

int JigsawPiece::id() const
{
    return m_id;
}

void JigsawPiece::calculateMaxRectForRotation()
{
    int oldWidth = pixmap().size().width();
    int oldHeight = pixmap().size().height();
    double radius = qSqrt(qPow(1.0 * oldHeight / 2, 2) + qPow(1.0 * oldWidth / 2, 2));
    double newWidth = 2.0 * radius;
    double newHeight = newWidth;
    double offsetWidth = (newWidth - oldWidth) / 2;
    double offsetHeight = (newHeight - oldHeight) / 2;
    m_maxRectForRotation = QRectF(QPointF(-offsetWidth, -offsetHeight), QSizeF(newWidth, newHeight));
}

void JigsawPiece::moveTimerTimeOut()
{
    QPointF newPosition = QCursor::pos() + m_cursorOffset;
    QPointF draggedBy = newPosition - m_originalPosition;
    if (draggedBy.isNull()) return;
    move(newPosition);
    emit dragged(m_id, draggedBy);
}

void JigsawPiece::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_dragEnabled && !m_rotated) {
        m_cursorOffset = m_originalPosition - mapToGlobal(event->pos());
        m_selected = !m_selected;
        if (m_selected) {
            m_moveTimer->start(1000 / FPS);
            raise();
        }
        else {
            m_moveTimer->stop();
            emit dragStopped(m_id);
        }
        emit leftClicked(m_id);
        return;
    }
    else if (event->button() == Qt::RightButton && !m_selected) {
        m_startingAngle = m_angle + QLineF(QPointF(m_maxRectForRotation.center()), QPointF(event->pos() + m_maxRectForRotation.topLeft())).angle();
        m_rotated = true;
        emit rightClicked(m_id);
    }
    emit clicked(m_id);
}

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

void JigsawPiece::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClicked(m_id);
}

void JigsawPiece::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && m_dragEnabled && !m_rotated) {
        m_dragged = true;
        ++m_draggedDistance;
    }
    else if (event->buttons() == Qt::RightButton && m_rotationEnabled && !m_selected) {
        int angle = QLineF(QPointF(m_maxRectForRotation.center()), QPointF(event->pos() + m_maxRectForRotation.topLeft())).angle();
        angle -= m_startingAngle;
        angle /= MINROTATEANGLE;
        setAngle(-angle * MINROTATEANGLE);
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
    calculateMaxRectForRotation();
    m_actualPosition = m_originalPosition + m_maxRectForRotation.topLeft();
    setGeometry(m_maxRectForRotation.translated(m_originalPosition).toRect());

    QPixmap rotatedPixmap(m_maxRectForRotation.toRect().size());
    rotatedPixmap.fill(Qt::transparent);

    QTransform transform;
    transform.translate(m_maxRectForRotation.width() / 2, m_maxRectForRotation.height() / 2);
    transform.rotate(m_angle);
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
