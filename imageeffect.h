#ifndef IMAGEEFFECT_H
#define IMAGEEFFECT_H

#include <QImage>
#include <QTimer>
#include <QObject>
#include <QPainter>
#include <QLabel>
#include <random>

/*
 * This class creates the effect for the image you see, when you finish a jigsaw puzzle. It has
 * a slot run() and a signal effectFinished(), so it is possible to run it on a different thread.
 */

class ImageEffect : public QObject
{
    Q_OBJECT

    const int TIMEOUTMSECS = 10;

public:
    enum class TypeOfEffect {
        GROW,
        FADEIN
    };

    explicit ImageEffect(QObject *parent = nullptr);
    explicit ImageEffect(QLabel *target, const QPixmap &source, TypeOfEffect type, QObject *parent = nullptr, double duration = 1000);
    ~ImageEffect();

    QPaintDevice *target() const;
    void setTarget(QLabel *newTarget);
    const QPixmap &source() const;
    void setSource(const QPixmap &newSource);
    TypeOfEffect type() const;
    void setType(TypeOfEffect newType);
    double duration() const;
    void setDuration(double newDuration);

public slots:
    void run();

private:
    QLabel* m_target;
    QPixmap m_source;
    TypeOfEffect m_type;
    double m_duration;
    QTimer* m_timer;

    QSize m_targetSize;
    QSize m_scaledSourceSize;
    QPoint m_startingPoint;
    QPixmap m_unfinishedPixmap;
    int m_counter;
    double m_percentagPerTimeout;

private slots:
    void growTimeout();
    void fadeInTimeout();

signals:
    void effectFinished(bool successful);

};

#endif // IMAGEEFFECT_H
