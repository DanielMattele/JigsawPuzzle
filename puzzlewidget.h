#ifndef PUZZLEWIDGET_H
#define PUZZLEWIDGET_H

#include "custommenuwidget.h"
#include "jigsawpath.h"
#include "puzzlegrid.h"
#include "jigsawlabel.h"
#include "jigsawpiece.h"
#include "jigsawbutton.h"
#include "jigsawslider.h"
#include "imageeffect.h"
#include <QRadioButton>
#include <QCheckBox>
#include <QWidget>
#include <QTimer>
#include <QPainterPath>
#include <QLabel>

struct Parameters {
    int screenWidth = 1920;
    int screenHeight = 1080;
    int innerOuterBoundsDifference = screenHeight / 5;

    int minCaptionHeight = 50;
    int minLabelExHeight = 100;
    int minLabelExWidth = minLabelExHeight * 16 / 9;
    int minBorderWidth = 10;
    int minRadioButtonHeight = 20;
    int minLabelPuzzlePieceWidth = 100;
    int minLabelPuzzlePieceHeight = minLabelPuzzlePieceWidth;

    int outerWidth = screenWidth;
    int outerHeight = screenHeight;
    int innerWidth = screenWidth - innerOuterBoundsDifference * 2;
    int innerHeight = screenHeight - innerOuterBoundsDifference * 2;
    int freeAreaWidth = screenWidth * 2 / 3;
    int freeAreaHeight = screenHeight;
    int freeAreaAlignment = 1; // 0 = Center, 1 = Left, 2 = Right

    int widthWidgetCaption = innerWidth;
    int widthWidgetEx = innerWidth;
    int widthWidgetOpenFile = innerWidth;
    int widthWidgetPuzzlePiece = innerWidth * 2 / 3;
    int widthWidgetNumberOfPieces = innerWidth / 3;
    int widthWidgetButtons = innerWidth;

    int heightWidgetCaption = innerHeight / 6;
    int heightWidgetEx = innerHeight / 4;
    int heightWidgetOpenFile = innerHeight / 12;
    int heightWidgetPuzzlePiece = innerHeight / 4;
    int heightWidgetNumberOfPieces = heightWidgetPuzzlePiece;
    int heightWidgetButtons = innerHeight / 4;

    QPoint positionWidget = QPoint(screenWidth / 2 - outerWidth / 2, screenHeight / 2 - outerHeight / 2);
    QPoint positionWidgetArea = QPoint(innerOuterBoundsDifference, innerOuterBoundsDifference);
    QPoint positionWidgetCaption = QPoint(0, 0);
    QPoint positionWidgetEx = QPoint(0, heightWidgetCaption);
    QPoint positionWidgetOpenFile = QPoint(0, heightWidgetCaption + heightWidgetEx);
    QPoint positionWidgetPuzzlePiece = QPoint(0, heightWidgetCaption + heightWidgetEx + heightWidgetOpenFile);
    QPoint positionWidgetNumberOfPieces = QPoint(widthWidgetPuzzlePiece, heightWidgetCaption + heightWidgetEx + heightWidgetOpenFile);
    QPoint positionWidgetButtons = QPoint(0, heightWidgetCaption + heightWidgetEx + heightWidgetOpenFile + heightWidgetPuzzlePiece);
    QPoint positionFreeArea = freeAreaAlignment == 0 ? QPoint((screenWidth - freeAreaWidth) / 2, (screenHeight - freeAreaHeight) / 2)
                            : freeAreaAlignment == 1 ? QPoint(0, (screenHeight - freeAreaHeight) / 2)
                                                     : QPoint((screenWidth - freeAreaWidth), (screenHeight - freeAreaHeight) / 2);

    QSize sizeWidget = QSize(outerWidth, outerHeight);
    QSize sizeWidgetArea = QSize(innerWidth, innerHeight);
    QSize sizeWidgetCaption = QSize(widthWidgetCaption, heightWidgetCaption);
    QSize sizeWidgetEx = QSize(widthWidgetEx, heightWidgetEx);
    QSize sizeWidgetOpenFile = QSize(widthWidgetOpenFile, heightWidgetOpenFile);
    QSize sizeWidgetPuzzlePiece = QSize(widthWidgetPuzzlePiece, heightWidgetPuzzlePiece);
    QSize sizeWidgetNumberOfPieces = QSize(widthWidgetNumberOfPieces, heightWidgetNumberOfPieces);
    QSize sizeWidgetButtons = QSize(widthWidgetButtons, heightWidgetButtons);
    QSize sizeFreeArea = QSize(freeAreaWidth, freeAreaHeight);

    int minNumberOfPieces = 1;
    int maxNumberOfPieces = 300;
    QSize sizeButtonOuterBounds = QSize(widthWidgetNumberOfPieces / 4, heightWidgetNumberOfPieces * 3 / 4);
    QSize sizeButtonInnerBounds = QSize(widthWidgetNumberOfPieces / 8, heightWidgetNumberOfPieces / 4);

    QRect rectWidget = QRect(positionWidget, sizeWidget);
    QRect rectWidgetArea = QRect(positionWidgetArea, sizeWidgetArea);
    QRect rectWidgetCaption = QRect(positionWidgetCaption, sizeWidgetCaption);
    QRect rectWidgetEx = QRect(positionWidgetEx, sizeWidgetEx);
    QRect rectWidgetOpenFile = QRect(positionWidgetOpenFile, sizeWidgetOpenFile);
    QRect rectWidgetPuzzlePiece = QRect(positionWidgetPuzzlePiece, sizeWidgetPuzzlePiece);
    QRect rectWidgetNumberOfPieces = QRect(positionWidgetNumberOfPieces, sizeWidgetNumberOfPieces);
    QRect rectWidgetButtons = QRect(positionWidgetButtons, sizeWidgetButtons);
    QRect rectFreeArea = QRect(positionFreeArea, sizeFreeArea);

    QFont mainFont = QFont("Georgia", 16, QFont::Bold);
};

class PuzzleWidget : public QWidget
{
    Q_OBJECT

private:
    QLabel* m_background;
    QVector<JigsawPiece*> m_puzzlePieces;

    QVector<QVector<JigsawPiece*>> m_mergedPieces;

    void createNewMergedPiece(JigsawPiece* firstPiece);
    void addPuzzlePieceToMergedPiece(JigsawPiece* piece, int mergedPieceID);
    void combineTwoMergedPieces(int firstMergedPieceID, int secondMergedPieceID);
    bool isPartOfMergedPiece(JigsawPiece* piece, int &mergedPieceID);
    bool areMerged(JigsawPiece* first, JigsawPiece* second);

    bool isInCorrectPosition(JigsawPiece* piece, JigsawPiece *neighbor, int tolerance = 5);
    void repositionPiece(JigsawPiece* piece, JigsawPiece* neighbor);

    int m_numberOfPieces;
    bool m_rotationAllowed;

    void calculateRowsAndCols(int numberOfPieces, const QPixmap &image);

    int m_rows;
    int m_cols;
    QPixmap m_image;
    TypeOfPiece m_typeOfPiece;

    int m_pieceWidth;
    int m_pieceHeight;

    PuzzleGrid *m_grid;

    void setupImage();

    void generatePuzzlePieces();
    void placePuzzlePieces();

    QPixmap createImageFragment(int pieceID);

    //Menu Widgets

    Parameters m_parameters;

    CustomMenuWidget* m_menuWidget;

    void setMenuWidget();

    QTimer* m_moveMenuInTimer;
    QTimer* m_moveMenuOutTimer;

    QWidget* m_quitWidget;

    void setQuitWidget();

    QWidget* m_newWidget;

    QVector<QRadioButton*> m_radioButtonEx;
    QVector<QRadioButton*> m_radioButtonPuzzlePiece;
    QCheckBox* m_rotationAllowedCheckBox;
    JigsawButton* m_ownImageLabel;
    QString m_filename;
    JigsawSlider* m_sliderButton;

    void setNewWidget();
    void setNewWidgetExWidget(QWidget* parent, const Parameters &par);
    void setNewWidgetPuzzlePieceWidget(QWidget* parent, const Parameters &par);
    void setNewWidgetNumberOfPiecesWidget(QWidget* parent, const Parameters &par);
    void setNewWidgetButtonsWidget(QWidget* parent, const Parameters &par);

    QWidget* m_wonWidget;

    void setWonWidget();
    void wonGame();

    QWidget* m_createOwnShapeWidget;

    void setCreateOwnShapeWidget();

    void loadImage(const QPixmap &image);
    void setupPuzzle();

public:
    explicit PuzzleWidget(QWidget *parent = nullptr);

private slots:
    void menuNewButtonClicked();
    void menuQuitButtonClicked();

    void enterMenu();
    void leaveMenu();
    void moveMenuInTimerTimeout();
    void moveMenuOutTimerTimeout();

    void quitWidgetYesClicked();
    void newWidgetOkClicked();
    void newWidgetOwnImageClicked();

    void dragMergedPieces(int id, const QPointF &draggedBy);
    void rotateMergedPieces(int id, int angle, const QPointF &rotatingPoint);
    void raisePieces(int id);
    void fixPieceIfPossible(int id);
    void fixMergedPieceIfPossible(int id);

signals:

};

#endif // PUZZLEWIDGET_H
