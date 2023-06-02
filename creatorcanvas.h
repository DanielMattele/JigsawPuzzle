#ifndef CREATORCANVAS_H
#define CREATORCANVAS_H

#include <QLabel>
#include <QMouseEvent>

class CreatorCanvas : public QLabel
{
    Q_OBJECT
public:
    CreatorCanvas(QWidget* parent = nullptr);
    ~CreatorCanvas();

signals:
    void clicked(const QPoint &position);
    void released();
    void dragged(const QPoint &position);

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // CREATORCANVAS_H
