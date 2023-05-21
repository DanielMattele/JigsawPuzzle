#ifndef JIGSAWLABEL_H
#define JIGSAWLABEL_H

#include <QLabel>
#include <QPainterPath>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QPainter>

class JigsawLabel : public QLabel
{
    Q_OBJECT
private:
    const int DEFAULTWIDTH = 100;
    const int DEFAULTHEIGHT = 100;


    QString m_text;
    QPixmap m_pixmap;
    QPainterPath m_jigsawPath;
    QRect m_textArea;
    QPen m_borderPen;
    QFont m_font;
    QColor m_textColor;
    QFlags<Qt::AlignmentFlag> m_alignment;

protected:
    enum class Mode {PIXMAP, BRUSH} m_mode;

    QBrush m_brush;
    QPointF m_originalPosition;
    QSizeF m_originalSize;
    void redraw();

public:
    explicit JigsawLabel(QWidget* parent = nullptr);
    explicit JigsawLabel(const QPixmap &background, QWidget* parent = nullptr, const QString &text = "", QRect textarea = QRect());
    explicit JigsawLabel(const QSize &size, const QBrush &background, const QPainterPath &jigsawPath, QWidget* parent = nullptr, const QString &text = "", QRect textarea = QRect());
    ~ JigsawLabel();

    const QString &text() const;
    void setText(const QString &newText);
    const QPixmap &pixmap() const;
    void setPixmap(const QPixmap &newPixmap, bool resize = true, QRect newTextArea = QRect());
    QPainterPath jigsawPath() const;
    void setJigsawPath(const QPainterPath &newJigsawPath, const QSize &newSize, const QBrush &newBrush);
    void setJigsawPath(const QPainterPath &newJigsawPath, const QSize &newSize);
    void setJigsawPath(const QPainterPath &newJigsawPath);
    const QRect &textArea() const;
    void setTextArea(const QRect &newTextArea);
    const QPen &borderPen() const;
    void setBorderPen(const QPen &newBorderPen);
    const QFont &font() const;
    void setFont(const QFont &newFont);
    const QColor &textColor() const;
    void setTextColor(const QColor &newTextColor);
    const QFlags<Qt::AlignmentFlag> &alignment() const;
    void setAlignment(const QFlags<Qt::AlignmentFlag> &newAlignment);

    void move(const QPointF &pos);
    void move(double x, double y);
    QPointF originalPosition() const;
};

#endif // JIGSAWLABEL_H
