#ifndef JIGSAWSLIDER_H
#define JIGSAWSLIDER_H

#include "jigsawbutton.h"

class JigsawSlider : public JigsawButton
{
    Q_OBJECT
    const double SIZEPROPORTIONBUTTON = 0.25;
    const int SLIDERPENWIDTH = 2;
    const int SLIDERWIDTH = 10;
    const int SLIDERRADIUS = 3;

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
