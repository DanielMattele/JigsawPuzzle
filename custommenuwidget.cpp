#include "custommenuwidget.h"

JigsawButton *CustomMenuWidget::newPuzzleButton() const
{
    return m_newPuzzleButton;
}

JigsawButton *CustomMenuWidget::quitButton() const
{
    return m_quitButton;
}

void CustomMenuWidget::enterEvent(QEnterEvent *event)
{
    raise();
    emit enterMenu();
}

void CustomMenuWidget::leaveEvent(QEvent *event)
{
    emit leaveMenu();
}

CustomMenuWidget::CustomMenuWidget(QSize size, QWidget *parent)
    : QWidget{parent}
{
    QPixmap backgroundMenu(":/backgrounds/back3");
    QPixmap backgroundButtons(":/backgrounds/back2");

    QLabel* backgroundLabel = new QLabel(this);
    backgroundLabel->setGeometry(QRect(QPoint(0, 0), size));
    backgroundLabel->setPixmap(backgroundMenu);

    QSize sizeInnerBounds(70, 60);
    QSize sizeOuterBounds(130, 100);

    QPainterPath pathNewButton = JigsawPath::singleJigsawPiecePath(QRect(QPoint(0, 0), sizeOuterBounds), QRect(), TypeOfPiece::STANDARD, 4, true);
    QPainterPath pathQuitButton = JigsawPath::singleJigsawPiecePath(QRect(QPoint(0, 0), sizeOuterBounds), QRect(), TypeOfPiece::STANDARD, 4, true);

    m_newPuzzleButton = new JigsawButton(sizeOuterBounds, QBrush(backgroundButtons), pathNewButton, this, "New");
    m_newPuzzleButton->setFont(QFont("Georgia", 16, QFont::Bold));
    m_newPuzzleButton->animate();
    m_newPuzzleButton->move(10, 10);

    m_quitButton = new JigsawButton(sizeOuterBounds, QBrush(backgroundButtons), pathQuitButton, this, "Quit");
    m_quitButton->setFont(QFont("Georgia", 16, QFont::Bold));
    m_quitButton->animate();
    m_quitButton->move(10, 370);
}
