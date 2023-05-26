#include "jigsawbutton.h"

JigsawButton::JigsawButton(QWidget *parent)
    : JigsawLabel{parent}
    , m_animate(false)
    , m_animationTypes{AnimationType::LIGHTERONENTER, AnimationType::CUSTOMCOLORTEXTONENTER}
    , m_customTextColor(Qt::darkGray)
    , m_pixmapTemp(QPixmap())
{

}

JigsawButton::JigsawButton(const QPixmap &background, QWidget *parent, const QString &text, QRect textarea)
    : JigsawLabel{background, parent, text, textarea}
    , m_animate(false)
    , m_animationTypes{AnimationType::LIGHTERONENTER, AnimationType::CUSTOMCOLORTEXTONENTER}
    , m_customTextColor(Qt::darkGray)
    , m_pixmapTemp(QPixmap())
{

}

JigsawButton::JigsawButton(const QSize &size, const QBrush &background, const QPainterPath &jigsawPath, QWidget *parent, const QString &text, QRect textarea)
    : JigsawLabel{size, background, jigsawPath, parent, text, textarea}
    , m_animate(false)
    , m_animationTypes{AnimationType::LIGHTERONENTER, AnimationType::CUSTOMCOLORTEXTONENTER}
    , m_customTextColor(Qt::darkGray)
    , m_pixmapTemp(QPixmap())
{

}

JigsawButton::~JigsawButton()
{

}

void JigsawButton::animate(bool val)
{
    m_animate = val;
}

void JigsawButton::setAnimationTypes(const QSet<AnimationType> &newAnimationTypes)
{
    m_animationTypes = newAnimationTypes;
    solveAnimationConflicts();
}

void JigsawButton::setCustomTextColor(const QColor &newCustomTextColor)
{
    m_customTextColor = newCustomTextColor;
}

void JigsawButton::setTextColorToBlack()
{
    JigsawButton::setTextColor(Qt::black);
}

void JigsawButton::setTextColorToWhite()
{
    JigsawButton::setTextColor(Qt::white);
}

void JigsawButton::setTextColor(const QColor &color)
{
    m_textColorTemp = JigsawLabel::textColor();
    JigsawLabel::setTextColor(color);
}

void JigsawButton::setTextColorLighter()
{
    JigsawButton::setTextColor(JigsawLabel::textColor().lighter());
}

void JigsawButton::setTextColorDarker()
{
    JigsawButton::setTextColor(JigsawLabel::textColor().darker());
}

void JigsawButton::resetTextColor()
{
    if (!m_textColorTemp.isValid()) return;
    JigsawLabel::setTextColor(m_textColorTemp);
}

void JigsawButton::setPixmapLighter()
{
    m_modeTemp = m_mode;
    QImage image;
    switch (m_mode) {
    case JigsawLabel::Mode::PIXMAP: {
        if (JigsawLabel::pixmap().isNull()) return;

        m_pixmapTemp = JigsawLabel::pixmap();
        image = m_pixmapTemp.toImage();
        break;
    }
    case JigsawLabel::Mode::BRUSH: {
        if (JigsawLabel::jigsawPath().isEmpty()) return;
        QPixmap pixmap(QLabel::pixmap().size());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setPen(borderPen());
        painter.setBrush(m_brush);
        painter.drawPath(jigsawPath());

        image = pixmap.toImage();
        break;
    }
    }
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            QColor color = image.pixelColor(j, i);
            if (color == Qt::transparent) continue;
            color = color.lighter();
            image.setPixelColor(j, i, color);
        }
    }
    JigsawLabel::setPixmap(QPixmap::fromImage(image), true);
}

void JigsawButton::setPixmapDarker()
{
    m_modeTemp = m_mode;
    QImage image;
    switch (m_mode) {
    case JigsawLabel::Mode::PIXMAP: {
        if (JigsawLabel::pixmap().isNull()) return;

        m_pixmapTemp = JigsawLabel::pixmap();
        image = m_pixmapTemp.toImage();
        break;
    }
    case JigsawLabel::Mode::BRUSH: {
        if (JigsawLabel::jigsawPath().isEmpty()) return;
        QPixmap pixmap(QLabel::pixmap().size());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setPen(borderPen());
        painter.setBrush(m_brush);
        painter.drawPath(jigsawPath());

        image = pixmap.toImage();
        break;
    }
    }
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            QColor color = image.pixelColor(j, i);
            if (color == Qt::transparent) continue;
            color = color.darker();
            image.setPixelColor(j, i, color);
        }
    }
    JigsawLabel::setPixmap(QPixmap::fromImage(image), false);
}

void JigsawButton::resetPixmap()
{
    switch (m_modeTemp) {
    case (Mode::PIXMAP):
        if (m_pixmapTemp.isNull()) return;
        JigsawLabel::setPixmap(m_pixmapTemp, false);
        break;
    case(Mode::BRUSH):
        JigsawLabel::setJigsawPath(jigsawPath());
        break;
    }
}

void JigsawButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) emit leftClicked();
    if (event->button() == Qt::RightButton) emit rightClicked();
    emit clicked();
}

void JigsawButton::mouseReleaseEvent(QMouseEvent *event)
{
    emit released();
}

void JigsawButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClicked();
}

void JigsawButton::enterEvent(QEnterEvent *event)
{
    emit entered();
    if (!m_animate) return;
    applyAnimation();
}

void JigsawButton::leaveEvent(QEvent *event)
{
    emit left();
    if (!m_animate) return;
    resetTextColor();
    resetPixmap();
}

/*
 * Some animations can't be used at the same time, so every time the types are set, this function is called to eliminate
 * the conflicts. It should be obvious, which animations can't be paired, so there is no complicated logic, which animations
 * are deleted from the set and which are not.
 */

void JigsawButton::solveAnimationConflicts()
{
    if (m_animationTypes.contains(AnimationType::LIGHTERONENTER)) {
        m_animationTypes.remove(AnimationType::DARKERONENTER);
    }
    if (m_animationTypes.contains(AnimationType::DARKERONENTER)) {
        m_animationTypes.remove(AnimationType::LIGHTERONENTER);
    }
    if (m_animationTypes.contains(AnimationType::BLACKTEXTONENTER)) {
        m_animationTypes.remove(AnimationType::WHITETEXTONENTER);
        m_animationTypes.remove(AnimationType::LIGHTERTEXTONENTER);
        m_animationTypes.remove(AnimationType::DARKERTEXTONENTER);
        m_animationTypes.remove(AnimationType::CUSTOMCOLORTEXTONENTER);
    }
    if (m_animationTypes.contains(AnimationType::WHITETEXTONENTER)) {
        m_animationTypes.remove(AnimationType::BLACKTEXTONENTER);
        m_animationTypes.remove(AnimationType::LIGHTERTEXTONENTER);
        m_animationTypes.remove(AnimationType::DARKERTEXTONENTER);
        m_animationTypes.remove(AnimationType::CUSTOMCOLORTEXTONENTER);
    }
    if (m_animationTypes.contains(AnimationType::LIGHTERTEXTONENTER)) {
        m_animationTypes.remove(AnimationType::WHITETEXTONENTER);
        m_animationTypes.remove(AnimationType::BLACKTEXTONENTER);
        m_animationTypes.remove(AnimationType::DARKERTEXTONENTER);
        m_animationTypes.remove(AnimationType::CUSTOMCOLORTEXTONENTER);
    }
    if (m_animationTypes.contains(AnimationType::DARKERTEXTONENTER)) {
        m_animationTypes.remove(AnimationType::WHITETEXTONENTER);
        m_animationTypes.remove(AnimationType::LIGHTERTEXTONENTER);
        m_animationTypes.remove(AnimationType::BLACKTEXTONENTER);
        m_animationTypes.remove(AnimationType::CUSTOMCOLORTEXTONENTER);
    }
    if (m_animationTypes.contains(AnimationType::CUSTOMCOLORTEXTONENTER)) {
        m_animationTypes.remove(AnimationType::WHITETEXTONENTER);
        m_animationTypes.remove(AnimationType::LIGHTERTEXTONENTER);
        m_animationTypes.remove(AnimationType::DARKERTEXTONENTER);
        m_animationTypes.remove(AnimationType::BLACKTEXTONENTER);
    }
}

void JigsawButton::applyAnimation()
{
    if (m_animationTypes.contains(AnimationType::LIGHTERONENTER)) setPixmapLighter();
    if (m_animationTypes.contains(AnimationType::DARKERONENTER)) setPixmapDarker();
    if (m_animationTypes.contains(AnimationType::BLACKTEXTONENTER)) setTextColorToBlack();
    if (m_animationTypes.contains(AnimationType::WHITETEXTONENTER)) setTextColorToWhite();
    if (m_animationTypes.contains(AnimationType::LIGHTERTEXTONENTER)) setTextColorLighter();
    if (m_animationTypes.contains(AnimationType::DARKERTEXTONENTER)) setTextColorDarker();
    if (m_animationTypes.contains(AnimationType::CUSTOMCOLORTEXTONENTER)) JigsawButton::setTextColor(m_customTextColor);
}
