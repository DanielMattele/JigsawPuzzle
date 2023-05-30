#ifndef CUSTOMMENUWIDGET_H
#define CUSTOMMENUWIDGET_H


#include "jigsawbutton.h"
#include "jigsawpath.h"
#include <QWidget>

/*
 * This class simply provides a widget with signals, when you enter and leave it. It is not essential for the jigsaw puzzle.
 *
 * @Julia: Nur interessant für Qt, beachte diese Klasse einfach nicht.
 */

class CustomMenuWidget : public QWidget
{
    Q_OBJECT
private:
    JigsawButton* m_newPuzzleButton;
    JigsawButton* m_quitButton;
public:
    explicit CustomMenuWidget(QSize size, QWidget *parent = nullptr);

    JigsawButton *newPuzzleButton() const;
    JigsawButton *quitButton() const;

signals:
    void enterMenu();
    void leaveMenu();

protected:
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

};

#endif // CUSTOMMENUWIDGET_H
