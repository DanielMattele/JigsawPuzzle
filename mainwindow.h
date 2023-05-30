#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "puzzlewidget.h"
#include <QMainWindow>

/*
 * @Julia: Nur interessant für Qt, beachte diese Klasse einfach nicht.
 */

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PuzzleWidget* m_puzzleWidget;
};
#endif // MAINWINDOW_H
