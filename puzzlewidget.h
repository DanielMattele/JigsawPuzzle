#ifndef PUZZLEWIDGET_H
#define PUZZLEWIDGET_H

#include "customjigsawpathcreator.h"
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

// @Julia: Das ist die Hauptklasse. Ignoriere das struct und scrolle runter bis zur class PuzzleWidget. Viel Spaß!



/*
 * The PuzzleWidget class is the core of the jigsaw puzzle game. You can load your own image or choose one of
 * the examples to turn it into a jigsaw puzzle. You can choose the shape and the number of your pieces and
 * whether you want them to be rotatable or not. In the GUI, the pieces can be dragged (or rotated) by clicking
 * them. When you put two pieces close enough together, they are merged and will be dragged (or rotated) together
 * from here on. The game is finished, when all pieces are merged.
 */

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
    Jigsaw::TypeOfPiece m_typeOfPiece;
    CustomJigsawPath m_customJigsawPath;

    int m_pieceWidth;
    int m_pieceHeight;

    PuzzleGrid *m_grid;

    void setupImage();

    void generatePuzzlePieces();
    void placePuzzlePieces();

    QPixmap createImageFragment(int pieceID);

    //Menu Widgets

    Jigsaw::Parameters m_parameters;

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
    JigsawButton* m_ownShapeLabel;
    QString m_filename;
    JigsawSlider* m_sliderButton;

    void setNewWidget();
    void setNewWidgetExWidget(QWidget* parent, const Jigsaw::Parameters &par);
    void setNewWidgetPuzzlePieceWidget(QWidget* parent, const Jigsaw::Parameters &par);
    void setNewWidgetNumberOfPiecesWidget(QWidget* parent, const Jigsaw::Parameters &par);
    void setNewWidgetButtonsWidget(QWidget* parent, const Jigsaw::Parameters &par);

    QWidget* m_wonWidget;

    void setWonWidget();
    void wonGame();

    QWidget* m_createOwnShapeWidget;
    CustomJigsawPathCreator* m_customJigsawPathCreator;

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
    void customJigsawPathCreatorApplyClicked(const CustomJigsawPath &customJigsawPath);

    void dragMergedPieces(int id, const QPointF &draggedBy);
    void rotateMergedPieces(int id, int angle, const QPointF &rotatingPoint);
    void raisePieces(int id);
    void fixPieceIfPossible(int id);
    void fixMergedPieceIfPossible(int id);

signals:

};

#endif // PUZZLEWIDGET_H
