#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_puzzleWidget(new PuzzleWidget(this))
{
    ui->setupUi(this);
    setCentralWidget(m_puzzleWidget);
    setWindowTitle("Jigsaw Puzzle");
}

MainWindow::~MainWindow()
{
    delete ui;
}

