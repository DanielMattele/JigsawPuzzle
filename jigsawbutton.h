#ifndef JIGSAWBUTTON_H
#define JIGSAWBUTTON_H

#include "jigsawlabel.h"
#include <QMouseEvent>

class JigsawButton : public JigsawLabel
{
    Q_OBJECT

public:
    enum class AnimationType {
        DARKERONENTER,
        LIGHTERONENTER,
        BLACKTEXTONENTER,
        WHITETEXTONENTER,
        DARKERTEXTONENTER,
        LIGHTERTEXTONENTER,
        CUSTOMCOLORTEXTONENTER
    };

    explicit JigsawButton(QWidget* parent = nullptr);
    explicit JigsawButton(const QPixmap &background, QWidget* parent = nullptr, const QString &text = "", QRect textarea = QRect());
    explicit JigsawButton(const QSize &size, const QBrush &background, const QPainterPath &jigsawPath, QWidget* parent = nullptr, const QString &text = "", QRect textarea = QRect());
    ~ JigsawButton();

    void animate(bool val = true);
    void setAnimationTypes(const QSet<AnimationType> &newAnimationTypes);
    void setCustomTextColor(const QColor &newCustomTextColor);

public slots:
    void setTextColorToBlack();
    void setTextColorToWhite();
    void setTextColor(const QColor &color);
    void setTextColorLighter();
    void setTextColorDarker();
    void resetTextColor();

    void setPixmapLighter();
    void setPixmapDarker();
    void resetPixmap();

signals:
    void clicked();
    void leftClicked();
    void rightClicked();
    void doubleClicked();
    void released();
    void entered();
    void left();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:
    QColor m_textColorTemp;
    QColor m_customTextColor;
    QPixmap m_pixmapTemp;
    Mode m_modeTemp;
    bool m_animate;
    QSet<JigsawButton::AnimationType> m_animationTypes;

    void solveAnimationConflicts();
    void applyAnimation();
};

#endif // JIGSAWBUTTON_H
