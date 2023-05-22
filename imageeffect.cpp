#include "imageeffect.h"

ImageEffect::ImageEffect(QObject *parent)
    : QObject{parent}
    , m_target(nullptr)
    , m_source(QPixmap())
    , m_type(TypeOfEffect::FADEIN)
    , m_duration(1000)
    , m_timer(new QTimer(this))
{
    QObject::connect(m_timer, &QTimer::timeout, this, &ImageEffect::fadeInTimeout);
}

ImageEffect::ImageEffect(QLabel* target, const QPixmap &source, TypeOfEffect type, QObject *parent, double duration)
    : QObject{parent}
    , m_target(target)
    , m_source(source)
    , m_type(type)
    , m_duration(duration)
    , m_timer(new QTimer(this))
{
    setType(type);
}

ImageEffect::~ImageEffect()
{

}

void ImageEffect::run()
{
    if (m_target == nullptr || m_source.isNull() || qFuzzyIsNull(m_duration)) return;

    m_targetSize = m_target->size();
    m_source = m_source.scaled(m_targetSize, Qt::KeepAspectRatio);
    m_scaledSourceSize = m_source.size();
    m_startingPoint = QPoint((m_targetSize.width() - m_scaledSourceSize.width()) / 2, (m_targetSize.height() - m_scaledSourceSize.height()) / 2);
    m_counter = 0;
    m_percentagPerTimeout = 1.0 * TIMEOUTMSECS / m_duration;

    if (m_target->pixmap().isNull()) {
        m_unfinishedPixmap = QPixmap(m_targetSize);
        m_unfinishedPixmap.fill(Qt::transparent);
    }
    else {
        m_unfinishedPixmap = m_target->pixmap();
    }

    m_timer->start(TIMEOUTMSECS);
}

QPaintDevice *ImageEffect::target() const
{
    return m_target;
}

void ImageEffect::setTarget(QLabel *newTarget)
{
    m_target = newTarget;
}

const QPixmap &ImageEffect::source() const
{
    return m_source;
}

void ImageEffect::setSource(const QPixmap &newSource)
{
    m_source = newSource;
}

ImageEffect::TypeOfEffect ImageEffect::type() const
{
    return m_type;
}

void ImageEffect::setType(TypeOfEffect newType)
{
    switch (m_type) {
    case ImageEffect::TypeOfEffect::GROW:
        QObject::disconnect(m_timer, &QTimer::timeout, this, &ImageEffect::growTimeout);
        break;
    case ImageEffect::TypeOfEffect::FADEIN:
        QObject::disconnect(m_timer, &QTimer::timeout, this, &ImageEffect::fadeInTimeout);
        break;
    default:
        break;
    }
    switch (newType) {
    case ImageEffect::TypeOfEffect::GROW:
        QObject::connect(m_timer, &QTimer::timeout, this, &ImageEffect::growTimeout);
        break;
    case ImageEffect::TypeOfEffect::FADEIN:
        QObject::connect(m_timer, &QTimer::timeout, this, &ImageEffect::fadeInTimeout);
        break;
    }
    m_type = newType;
}

double ImageEffect::duration() const
{
    return m_duration;
}

void ImageEffect::setDuration(double newDuration)
{
    m_duration = newDuration;
}
void ImageEffect::growTimeout()
{
    if (m_percentagPerTimeout * m_counter >= 1.0) {
        m_timer->stop();
        emit effectFinished();
        return;
    }
    QPainter painter(&m_unfinishedPixmap);
    QPixmap scaledPixmap = m_source.scaled(m_scaledSourceSize * m_percentagPerTimeout * m_counter, Qt::KeepAspectRatio);
    QPoint offset((m_scaledSourceSize.width() - scaledPixmap.width()) / 2, (m_scaledSourceSize.height() - scaledPixmap.height()) / 2);
    painter.drawPixmap(QRect(m_startingPoint + offset, scaledPixmap.size()), scaledPixmap, scaledPixmap.rect());
    m_target->setPixmap(m_unfinishedPixmap);
    ++m_counter;
}

void ImageEffect::fadeInTimeout()
{
    if (m_percentagPerTimeout * m_counter >= 1.0) {
        m_timer->stop();
        emit effectFinished();
        return;
    }
    QPainter painter(&m_unfinishedPixmap);

    //No idea why, but the pixmap is fully opaque at a value of 0.1, and not 1.0 as stated in the documentation

    painter.setOpacity(m_percentagPerTimeout * m_counter / 10);
    painter.drawPixmap(QRect(m_startingPoint, m_source.size()), m_source, m_source.rect());
    m_target->setPixmap(m_unfinishedPixmap);
    ++m_counter;
}
