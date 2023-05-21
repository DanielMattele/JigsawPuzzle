#include "jigsawslider.h"

JigsawSlider::JigsawSlider(QWidget *parent)
    : JigsawButton{parent}
    , m_backgroundLabel(new JigsawLabel(parent))
    , m_sliderBrush(Qt::gray)
    , m_overallRect(QRect(0, 0, JigsawLabel::pixmap().width() / SIZEPROPORTIONBUTTON, JigsawLabel::pixmap().height()))
    , m_val(0)
    , m_minVal(0)
    , m_maxVal(100)
    , m_orientation(JigsawSlider::Orientation::HORIZONTAL)
    , m_cursorOffset(QPoint(0, 0))
{
    paintBackgroundLabel();
    setGeometry(m_overallRect);
}

JigsawSlider::JigsawSlider(const QPixmap &backgroundButton, const QBrush &backgroundSlider, QWidget *parent, int val, int minVal, int maxVal, Orientation orientation)
    : JigsawButton{backgroundButton, parent, QString::number(val)}
    , m_backgroundLabel(new JigsawLabel(parent))
    , m_sliderBrush(backgroundSlider)
    , m_overallRect(orientation == JigsawSlider::Orientation::HORIZONTAL
                    ? QRect(0, 0, backgroundButton.width() / SIZEPROPORTIONBUTTON, backgroundButton.height())
                    : QRect(0, 0, backgroundButton.width(), backgroundButton.height() / SIZEPROPORTIONBUTTON))
    , m_val(val)
    , m_minVal(minVal)
    , m_maxVal(maxVal)
    , m_orientation(orientation)
    , m_cursorOffset(QPoint(0, 0))
{
    paintBackgroundLabel();
    setGeometry(m_overallRect);
}

JigsawSlider::JigsawSlider(const QSize &sizeButton, const QBrush &backgroundButton, const QPainterPath &jigsawPathButton, const QBrush &backgroundSlider, QWidget *parent, int val, int minVal, int maxVal, Orientation orientation)
    : JigsawButton{sizeButton, backgroundButton, jigsawPathButton, parent, QString::number(val)}
    , m_backgroundLabel(new JigsawLabel(parent))
    , m_sliderBrush(backgroundSlider)
    , m_overallRect(orientation == JigsawSlider::Orientation::HORIZONTAL
                    ? QRect(0, 0, sizeButton.width() / SIZEPROPORTIONBUTTON, sizeButton.height())
                    : QRect(0, 0, sizeButton.width(), sizeButton.height() / SIZEPROPORTIONBUTTON))
    , m_val(val)
    , m_minVal(minVal)
    , m_maxVal(maxVal)
    , m_orientation(orientation)
    , m_cursorOffset(QPoint(0, 0))
{
    paintBackgroundLabel();
    setGeometry(m_overallRect);
}

JigsawSlider::~JigsawSlider()
{

}


void JigsawSlider::setGeometry(int x, int y, int w, int h)
{
    QRect rect(x, y, w, h);
    JigsawSlider::setGeometry(rect);
}

void JigsawSlider::setGeometry(const QRect &rect)
{
    int width = m_orientation == JigsawSlider::Orientation::HORIZONTAL ? rect.width() : QLabel::width();
    int height = m_orientation == JigsawSlider::Orientation::VERTICAL ? rect.height() : QLabel::height();

    m_overallRect = QRect(rect.topLeft(), QSize(width, height));

    paintBackgroundLabel();
    move(m_overallRect.topLeft());
}

void JigsawSlider::move(int x, int y)
{
    QPoint point(x, y);
    JigsawSlider::move(point);
}

void JigsawSlider::move(const QPoint &point)
{
    m_overallRect = QRect(point, m_overallRect.size());
    m_backgroundLabel->QLabel::move(point);
    QPoint buttonPoint(point + calculateButtonPositionOffset(JigsawLabel::pixmap().size()).toPoint());
    QLabel::move(buttonPoint);
    m_originalPosition = buttonPoint;
}

QPoint JigsawSlider::pos() const
{
    return m_overallRect.topLeft();
}

void JigsawSlider::raise()
{
    QLabel::raise();
    m_backgroundLabel->stackUnder(this);
}

void JigsawSlider::lower()
{
    QLabel::lower();
    m_backgroundLabel->stackUnder(this);
}

void JigsawSlider::setPixmap(const QPixmap &newPixmap)
{
    switch (m_orientation) {
    case JigsawSlider::Orientation::HORIZONTAL:
        m_overallRect.setBottom(m_overallRect.top() + newPixmap.height());
        break;
    case JigsawSlider::Orientation::VERTICAL:
        m_overallRect.setRight(m_overallRect.left() + newPixmap.width());
        break;
    }

    JigsawLabel::setPixmap(newPixmap, true);
    setGeometry(m_overallRect);
}

void JigsawSlider::setJigsawPath(const QPainterPath &newJigsawPath, const QSize &newSize, const QBrush &newBrush)
{
    m_brush = newBrush;
    setJigsawPath(newJigsawPath, newSize);
}

void JigsawSlider::setJigsawPath(const QPainterPath &newJigsawPath, const QSize &newSize)
{
    switch (m_orientation) {
    case JigsawSlider::Orientation::HORIZONTAL:
        m_overallRect.setBottom(m_overallRect.top() + newSize.height());
        break;
    case JigsawSlider::Orientation::VERTICAL:
        m_overallRect.setRight(m_overallRect.left() + newSize.width());
        break;
    }
    JigsawLabel::setJigsawPath(newJigsawPath, newSize);
    setGeometry(m_overallRect);
}

void JigsawSlider::setJigsawPath(const QPainterPath &newJigsawPath)
{
    JigsawLabel::setJigsawPath(newJigsawPath);
    setGeometry(m_overallRect);
}

int JigsawSlider::val() const
{
    return m_val;
}

void JigsawSlider::setVal(int newVal)
{
    m_val = newVal;
}

int JigsawSlider::minVal() const
{
    return m_minVal;
}

void JigsawSlider::setMinVal(int newMinVal)
{
    m_minVal = newMinVal;
}

int JigsawSlider::maxVal() const
{
    return m_maxVal;
}

void JigsawSlider::setMaxVal(int newMaxVal)
{
    m_maxVal = newMaxVal;
}

const QBrush &JigsawSlider::sliderBrush() const
{
    return m_sliderBrush;
}

void JigsawSlider::setSliderBrush(const QBrush &newSliderBrush)
{
    m_sliderBrush = newSliderBrush;
    paintBackgroundLabel();
}

JigsawSlider::Orientation JigsawSlider::orientation() const
{
    return m_orientation;
}

void JigsawSlider::setOrientation(Orientation newOrientation)
{
    m_orientation = newOrientation;
    QRect newRect(m_overallRect.topLeft(), QSize(m_overallRect.height(), m_overallRect.width()));
    setGeometry(newRect);
}

void JigsawSlider::paintBackgroundLabel()
{
    QPixmap background(m_overallRect.size());
    background.fill(Qt::transparent);
    QPainter painter(&background);
    painter.setPen(QPen(Qt::black, SLIDERPENWIDTH));
    painter.setBrush(m_sliderBrush);
    QRect paintArea = m_orientation == JigsawSlider::Orientation::HORIZONTAL
                    ? QRect(QPoint(SLIDERPENWIDTH, m_overallRect.height() / 2 - SLIDERWIDTH / 2), QSize(m_overallRect.width() - SLIDERPENWIDTH * 2, SLIDERWIDTH))
                    : QRect(QPoint(m_overallRect.width() / 2 - SLIDERWIDTH / 2, SLIDERPENWIDTH), QSize(SLIDERWIDTH, m_overallRect.height() - SLIDERPENWIDTH * 2));
    painter.drawRoundedRect(paintArea, SLIDERRADIUS, SLIDERRADIUS);
    m_backgroundLabel->setPixmap(background, false);
    m_backgroundLabel->setMask((background.mask()));
    m_backgroundLabel->stackUnder(this);
}

QPointF JigsawSlider::calculateButtonPositionOffset(const QSize &buttonSize) const
{
    int distance = m_orientation == JigsawSlider::Orientation::HORIZONTAL
                 ? m_overallRect.width() - buttonSize.width()
                 : m_overallRect.height() - buttonSize.height();
    double positionOffsetF = 1.0 * distance / (m_maxVal - m_minVal) * m_val;
    QPointF positionOffset(m_orientation == JigsawSlider::Orientation::HORIZONTAL ? positionOffsetF : 0.0,
                           m_orientation == JigsawSlider::Orientation::VERTICAL ? positionOffsetF : 0.0);
    return positionOffset;
}

void JigsawSlider::mousePressEvent(QMouseEvent *event)
{
    QPoint cursorPosition = QCursor::pos();
    m_cursorOffset = QLabel::pos() - cursorPosition;
}

void JigsawSlider::mouseMoveEvent(QMouseEvent *event)
{
    int sliderButtonMinPosition;
    int sliderButtonMaxPosition;
    int distance;
    int distanceValue = m_maxVal - m_minVal;

    QPoint cursorPosition = QCursor::pos();
    int newXPosition = cursorPosition.x() + m_cursorOffset.x();
    int newYPosition = cursorPosition.y() + m_cursorOffset.y();
    QPointF newPosition;

    double newValue;
    int newIntValue;

    switch (m_orientation) {
    case JigsawSlider::Orientation::HORIZONTAL:
        sliderButtonMinPosition = m_overallRect.left();
        sliderButtonMaxPosition = m_overallRect.left() + m_overallRect.width() - width();
        distance = sliderButtonMaxPosition - sliderButtonMinPosition;

        newXPosition = newXPosition < sliderButtonMinPosition ? sliderButtonMinPosition : newXPosition > sliderButtonMaxPosition ? sliderButtonMaxPosition : newXPosition;
        newPosition = QPointF(newXPosition, QLabel::y());

        newValue =  1.0 * distanceValue / distance * (newXPosition - sliderButtonMinPosition) + m_minVal;
        newIntValue = round(newValue);
        break;
    case JigsawSlider::Orientation::VERTICAL:
        sliderButtonMinPosition = m_overallRect.top();
        sliderButtonMaxPosition = m_overallRect.top() + m_overallRect.height() - height();
        distance = sliderButtonMaxPosition - sliderButtonMinPosition;

        newYPosition = newYPosition < sliderButtonMinPosition ? sliderButtonMinPosition : newYPosition > sliderButtonMaxPosition ? sliderButtonMaxPosition : newYPosition;
        newPosition = QPointF(QLabel::x(), newYPosition);

        newValue =  1.0 * distanceValue / distance * (newYPosition - sliderButtonMinPosition) + m_minVal;
        newIntValue = round(newValue);
        break;
    }

    JigsawLabel::move(newPosition);
    setText(QString::number(newIntValue));
    m_val = newIntValue;
    emit valueChanged(newIntValue);
}
