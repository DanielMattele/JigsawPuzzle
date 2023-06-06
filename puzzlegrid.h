#ifndef PUZZLEGRID_H
#define PUZZLEGRID_H

#include "jigsawpath.h"
#include "qpushbutton.h"
#include <QPainterPath>
#include <QObject>
#include <QPoint>
#include <QRect>



/*
 * This class calculates the coordinates for the corners of the JigsawPieces as well as the JigsawPaths. The image
 * you want to turn into a jigsaw puzzle is cut into small rectangles, but each of these rectangles has to be bigger
 * than the image width divided by the number of pieces in one row or column. Otherwise the image fragment would be
 * cut of at the edges. To achieve this, the class calculates a grid with some overlay. Inside this overlay, a grid
 * point is chosen randomly. After that, all JigsawPaths are calculated.
 */

class PuzzleGrid : public QObject
{
    Q_OBJECT
private:
    int m_rows;
    int m_cols;
    int m_numberOfGridPoints;
    int m_numberOfPieces;
    Jigsaw::TypeOfPiece m_typeOfPiece;
    CustomJigsawPath m_customPath;

    int currentRow(int pointID) const;
    int currentCol(int pointID) const;

    bool isOnBorder(const QPoint &point) const;
    bool isOnTopBorder(const QPoint &point) const;
    bool isOnRightBorder(const QPoint &point) const;
    bool isOnBottomBorder(const QPoint &point) const;
    bool isOnLeftBorder(const QPoint &point) const;

    bool isOnBorder(int pointID) const;
    bool isOnTopBorder(int pointID) const;
    bool isOnRightBorder(int pointID) const;
    bool isOnBottomBorder(int pointID) const;
    bool isOnLeftBorder(int pointID) const;

    int m_puzzlePiecesWidth;
    int m_puzzlePiecesHeight;

    int m_horizontalOverlap;
    int m_verticalOverlap;

    void calculateHorizontalOverlap();
    void calculateVerticalOverlap();

    int m_puzzleTotalWidth;
    int m_puzzleTotalHeight;

    void calculateTotalWidth();
    void calculateTotalHeight();

    QVector<QPoint> m_symmetricGrid;
    QVector<QPoint> m_overlayGrid;
    QVector<QPoint> m_puzzlePiecesGrid;

    void createGrids();
    void createSymmetricGrid();
    void createOverlayGrid();
    void createPuzzlePiecesGrid();

    QVector<QRect> m_puzzlePieceBounds;

    int pieceIDtoGridPointID(int pieceID, Jigsaw::Direction direction = Jigsaw::Direction::TOPLEFT) const;
    int gridPointIDtoPieceID(int gridPointID, Jigsaw::Direction direction = Jigsaw::Direction::TOPLEFT) const;
    void createPuzzlePieceBounds();
    QRect puzzlePieceBounds(int pieceID);

    QVector<QPainterPath> m_horizontalGridPaths;
    QVector<QPainterPath> m_verticalGridPaths;

    void createGridPaths(Jigsaw::TypeOfPiece typeOfPiece);
    QVector<QPainterPath> m_combinedPaths;

    void createCombinedPaths();
    QPainterPath combinePath(int pieceID) const;

    void debugGrid();

public:
    explicit PuzzleGrid(int rowsOfPieces, int colsOfPieces, int puzzlePiecesWidth, int puzzlePiecesHeight, Jigsaw::TypeOfPiece typeOfPiece, QObject *parent = nullptr, const CustomJigsawPath &customPath = CustomJigsawPath());
    ~PuzzleGrid();

    QPoint symmetricGridPoint(int pieceID, Jigsaw::Direction direction = Jigsaw::Direction::TOPLEFT) const;
    QPoint overlayGridPoint(int pieceID, Jigsaw::Direction direction = Jigsaw::Direction::TOPLEFT) const;
    QPoint puzzlePieceGridPoint(int pieceID, Jigsaw::Direction direction = Jigsaw::Direction::TOPLEFT) const;

    int pieceTotalWidth() const;
    int pieceTotalHeight() const;
    QSize pieceTotalSize() const;
    int puzzleTotalWidth() const;
    int puzzleTotalHeight() const;
    QSize puzzleTotalSize() const;

    const QPainterPath &puzzlePath(int pieceID) const;

signals:

};

#endif // PUZZLEGRID_H
