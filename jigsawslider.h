#ifndef JIGSAWSLIDER_H
#define JIGSAWSLIDER_H

#include "jigsawbutton.h"

/*
 * The JigsawButton class is derived from JigsawLabel and adds some of the functionality of a QPushButton. In addition, there
 * are som simple animations you can use when the cursor enters the JigsawButton's area.
 *
 * The JigsawSlider class is derived from JigsawButton and adds some of the functionality of a QSlider. There are two possible
 * orientations: horizontal and vertical. When you construct a JigsawSlider, its size is defined by the pixmap. If the orientation
 * is horizontal (the default value), the overall width is four times the buttons' width, if the orientation is vertical, the
 * overall height is four times the buttons height. You can change that via the setGeometry function, but in this function the
 * height (for horizontal orientation)  or width (for vertical orientation) value is ignored. If you want to change these values,
 * you have to provide a new pixmap via setPixmap or a new (button-)size via setJigsawPath.
 *
 * The JigsawSlider uses a JigsawLabel for the background. It has to be stacked under the button at any time, so the raise(), lower()
 * and stackUnder() functions of QWidget are overridden.
 *
 * You can set the minimum and maximum values of the JigsawSlider, as well as the actual value. If you do so, the button is repositioned.
 * If you drag the button, the value and text changes and the signal valueChanged is emitted.
 */

class JigsawSlider : public JigsawButton
{
    Q_OBJECT
    static constexpr double SIZEPROPORTIONBUTTON = 0.25;
    static constexpr unsigned int SLIDERPENWIDTH = 2;
    static constexpr unsigned int SLIDERWIDTH = 10;
    static constexpr unsigned int SLIDERRADIUS = 3;

public:
    enum class Orientation {HORIZONTAL, VERTICAL};

    explicit JigsawSlider(QWidget* parent = nullptr);
    explicit JigsawSlider(const QPixmap &backgroundButton,
                          const QBrush &backgroundSlider,
                          QWidget* parent = nullptr,
                          int val = 0,
                          int minVal = 0,
                          int maxVal = 100,
                          JigsawSlider::Orientation orientation = Orientation::HORIZONTAL);
    explicit JigsawSlider(const QSize &sizeButton,
                          const QBrush &backgroundButton,
                          const QPainterPath &jigsawPathButton,
                          const QBrush &backgroundSlider,
                          QWidget* parent = nullptr,
                          int val = 0,
                          int minVal = 0,
                          int maxVal = 100,
                          JigsawSlider::Orientation orientation = Orientation::HORIZONTAL);
    ~ JigsawSlider();

    void setGeometry(int x, int y, int w, int h);
    void setGeometry(const QRect &rect);
    void move(int x, int y);
    void move(const QPoint &point);
    QPoint pos() const;
    void raise();
    void lower();
    void stackUnder(QWidget* w);

    void setPixmap(const QPixmap &newPixmap);
    void setJigsawPath(const QPainterPath &newJigsawPath, const QSize &newSize, const QBrush &newBrush);
    void setJigsawPath(const QPainterPath &newJigsawPath, const QSize &newSize);
    void setJigsawPath(const QPainterPath &newJigsawPath);

    int val() const;
    int minVal() const;
    int maxVal() const;
    JigsawSlider::Orientation orientation() const;

    const QBrush &sliderBrush() const;
    void setSliderBrush(const QBrush &newSliderBrush);

public slots:
    void setVal(int newVal);
    void setMinVal(int newMinVal);
    void setMaxVal(int newMaxVal);
    void setOrientation(JigsawSlider::Orientation newOrientation);

signals:
    void positionButtonChanged(const QPoint &newPosition);
    void valueChanged(int val);

private:
    JigsawLabel* m_backgroundLabel;
    QBrush m_sliderBrush;
    void paintBackgroundLabel();
    QRect m_overallRect;
    int m_val;
    QPointF calculateButtonPositionOffset(const QSize &buttonSize) const;
    int m_minVal;
    int m_maxVal;
    JigsawSlider::Orientation m_orientation;
    QPoint m_cursorOffset;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // JIGSAWSLIDER_H
