#ifndef PUZZLEGRID_H
#define PUZZLEGRID_H

#include "jigsawpath.h"
#include "qpushbutton.h"
#include <QPainterPath>
#include <QObject>
#include <QPoint>
#include <QRect>

enum class Direction {
    TOPLEFT,
    TOPRIGHT,
    BOTTOMLEFT,
    BOTTOMRIGHT
};

class PuzzleGrid : public QObject
{
    Q_OBJECT
private:
    int m_rows;
    int m_cols;
    int m_numberOfGridPoints;
    int m_numberOfPieces;
    TypeOfPiece m_typeOfPiece;

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

    int pieceIDtoGridPointID(int pieceID, Direction direction = Direction::TOPLEFT) const;
    int gridPointIDtoPieceID(int gridPointID, Direction direction = Direction::TOPLEFT) const;
    void createPuzzlePieceBounds();
    QRect puzzlePieceBounds(int pieceID);

    QVector<QPainterPath> m_horizontalGridPaths;
    QVector<QPainterPath> m_verticalGridPaths;

    void createGridPaths(TypeOfPiece typeOfPiece);
    QVector<QPainterPath> m_combinedPaths;

    void createCombinedPaths();
    QPainterPath combinePath(int pieceID) const;

    void debugGrid();

public:
    explicit PuzzleGrid(int rowsOfPieces, int colsOfPieces, int puzzlePiecesWidth, int puzzlePiecesHeight, TypeOfPiece typeOfPiece, QObject *parent = nullptr);
    ~PuzzleGrid();

    QPoint symmetricGridPoint(int pieceID, Direction direction = Direction::TOPLEFT) const;
    QPoint overlayGridPoint(int pieceID, Direction direction = Direction::TOPLEFT) const;
    QPoint puzzlePieceGridPoint(int pieceID, Direction direction = Direction::TOPLEFT) const;

    int pieceTotalWidth() const;
    int pieceTotalHeight() const;
    QSize pieceTotalSize() const;
    int puzzleTotalWidth() const;
    int puzzleTotalHeight() const;
    QSize puzzleTotalSize() const;

    const QPainterPath &puzzlePath(int pieceID) const;

    static int randomNumber(int min, int max);

signals:

};

#endif // PUZZLEGRID_H
