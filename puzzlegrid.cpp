#include "puzzlegrid.h"
#include "qdebug.h"
#include "qpainter.h"
#include "qpen.h"
#include <random>

int PuzzleGrid::currentRow(int pointID) const
{
    return (pointID / m_cols);
}

int PuzzleGrid::currentCol(int pointID) const
{
    return (pointID % m_cols);
}

bool PuzzleGrid::isOnBorder(const QPoint &point) const
{
    if (isOnTopBorder(point) || isOnRightBorder(point) || isOnBottomBorder(point) || isOnLeftBorder(point)) return true;
    return false;
}

bool PuzzleGrid::isOnTopBorder(const QPoint &point) const
{
    for (int i = 0; i < m_numberOfGridPoints; ++i) {
        if ((m_overlayGrid[i] == point && isOnTopBorder(i)) ||
            (m_symmetricGrid[i] == point && isOnTopBorder(i)) ||
            (m_puzzlePiecesGrid[i] == point && isOnTopBorder(i)))
            return true;
    }
    return false;
}

bool PuzzleGrid::isOnRightBorder(const QPoint &point) const
{
    for (int i = 0; i < m_numberOfGridPoints; ++i) {
        if ((m_overlayGrid[i] == point && isOnRightBorder(i)) ||
            (m_symmetricGrid[i] == point && isOnRightBorder(i)) ||
            (m_puzzlePiecesGrid[i] == point && isOnRightBorder(i)))
            return true;
    }
    return false;
}

bool PuzzleGrid::isOnBottomBorder(const QPoint &point) const
{
    for (int i = 0; i < m_numberOfGridPoints; ++i) {
        if ((m_overlayGrid[i] == point && isOnBottomBorder(i)) ||
            (m_symmetricGrid[i] == point && isOnBottomBorder(i)) ||
            (m_puzzlePiecesGrid[i] == point && isOnBottomBorder(i)))
            return true;
    }
    return false;
}

bool PuzzleGrid::isOnLeftBorder(const QPoint &point) const
{
    for (int i = 0; i < m_numberOfGridPoints; ++i) {
        if ((m_overlayGrid[i] == point && isOnLeftBorder(i)) ||
            (m_symmetricGrid[i] == point && isOnLeftBorder(i)) ||
            (m_puzzlePiecesGrid[i] == point && isOnLeftBorder(i)))
            return true;
    }
    return false;
}

bool PuzzleGrid::isOnBorder(int pointID) const
{
    if (isOnTopBorder(pointID) || isOnRightBorder(pointID) || isOnBottomBorder(pointID) || isOnLeftBorder(pointID)) return true;
    return false;
}

bool PuzzleGrid::isOnTopBorder(int pointID) const
{
    return (currentRow(pointID) == 0);
}

bool PuzzleGrid::isOnRightBorder(int pointID) const
{
    return (currentCol(pointID) == m_cols - 1);
}

bool PuzzleGrid::isOnBottomBorder(int pointID) const
{
    return (currentRow(pointID) == m_rows - 1);
}

bool PuzzleGrid::isOnLeftBorder(int pointID) const
{
    return (currentCol(pointID) == 0);
}

void PuzzleGrid::calculateHorizontalOverlap()
{
    m_horizontalOverlap = m_puzzlePiecesHeight / 2;
}

void PuzzleGrid::calculateVerticalOverlap()
{
    m_verticalOverlap = m_puzzlePiecesWidth / 2;
}

int PuzzleGrid::puzzleTotalHeight() const
{
    return m_puzzleTotalHeight;
}

QSize PuzzleGrid::puzzleTotalSize() const
{
    return QSize(puzzleTotalWidth(), puzzleTotalHeight());
}

const QPainterPath &PuzzleGrid::puzzlePath(int pieceID) const
{
    if (pieceID >= m_combinedPaths.size()) return m_combinedPaths[0];
    return m_combinedPaths[pieceID];
}

int PuzzleGrid::puzzleTotalWidth() const
{
    return m_puzzleTotalWidth;
}

void PuzzleGrid::calculateTotalWidth()
{
    m_puzzleTotalWidth = (m_cols - 1) * m_puzzlePiecesWidth + 2 * m_verticalOverlap + 1;
}

void PuzzleGrid::calculateTotalHeight()
{
    m_puzzleTotalHeight = (m_rows - 1) * m_puzzlePiecesHeight + 2 * m_horizontalOverlap + 1;
}

int PuzzleGrid::pieceTotalWidth() const
{
    return m_puzzlePiecesWidth + 2 * m_verticalOverlap;
}

int PuzzleGrid::pieceTotalHeight() const
{
    return m_puzzlePiecesHeight + 2 * m_horizontalOverlap;
}

QSize PuzzleGrid::pieceTotalSize() const
{
    return QSize(pieceTotalWidth(), pieceTotalHeight());
}

void PuzzleGrid::createGrids()
{
    createSymmetricGrid();
    createOverlayGrid();
    createPuzzlePiecesGrid();
}

void PuzzleGrid::createSymmetricGrid()
{
    int x, y;

    for (int i = 0; i < m_numberOfGridPoints; ++i) {
        x = m_verticalOverlap + currentCol(i) * m_puzzlePiecesWidth;
        y = m_horizontalOverlap + currentRow(i) * m_puzzlePiecesHeight;
        m_symmetricGrid[i] = (QPoint(x, y));
    }
}

void PuzzleGrid::createOverlayGrid()
{
    int x, y;

    for (int i = 0; i < m_numberOfGridPoints; ++i) {
        x = currentCol(i) * m_puzzlePiecesWidth;
        y = currentRow(i) * m_puzzlePiecesHeight;
        m_overlayGrid[i] = (QPoint(x, y));
    }
}

void PuzzleGrid::createPuzzlePiecesGrid()
{
    int horizontalOffset, verticalOffset;

    for (unsigned int i = 0; i < m_numberOfGridPoints; ++i) {
        if (isOnLeftBorder(i) || isOnRightBorder(i)) {
            verticalOffset = 0;
        }
        else {
            verticalOffset = randomNumber(- m_verticalOverlap / 2, m_verticalOverlap / 2);
        }

        if (isOnTopBorder(i) || isOnBottomBorder(i)) {
            horizontalOffset = 0;
        }
        else {
            horizontalOffset = randomNumber(- m_horizontalOverlap / 2, m_horizontalOverlap / 2);
        }

        m_puzzlePiecesGrid[i] = (m_symmetricGrid[i] + QPoint(verticalOffset, horizontalOffset));
    }
}

int PuzzleGrid::pieceIDtoGridPointID(int pieceID, Direction direction) const
{
    unsigned int index1 = pieceID + (pieceID / (m_cols - 1));
    unsigned int index2 = index1 + 1;
    unsigned int index3 = index2 + m_cols;
    unsigned int index4 = index3 - 1;

    switch (direction) {
    case Direction::TOPLEFT:
        return index1;
        break;
    case Direction::TOPRIGHT:
        return index2;
        break;
    case Direction::BOTTOMLEFT:
        return index4;
        break;
    case Direction::BOTTOMRIGHT:
        return index3;
        break;
    default:
        return index1;
        break;
    }
}

int PuzzleGrid::gridPointIDtoPieceID(int gridPointID, Direction direction) const
{
    if (gridPointID >= m_puzzlePiecesGrid.size()) return -1;

    unsigned int index1 = gridPointID - (gridPointID / (m_cols));
    unsigned int index2 = index1 - 1;
    unsigned int index3 = index1 - m_cols;
    unsigned int index4 = index1 - m_cols + 1;

    switch (direction) {
    case Direction::TOPLEFT:
        return (isOnRightBorder(gridPointID) || isOnBottomBorder(gridPointID)) ? -1 : index1;
        break;
    case Direction::TOPRIGHT:
        return (isOnLeftBorder(gridPointID) || isOnBottomBorder(gridPointID)) ? -1 : index2;
        break;
    case Direction::BOTTOMRIGHT:
        return (isOnLeftBorder(gridPointID) || isOnTopBorder(gridPointID)) ? -1 : index3;
        break;
    case Direction::BOTTOMLEFT:
        return (isOnRightBorder(gridPointID) || isOnTopBorder(gridPointID)) ? -1 : index4;
        break;
    default:
        return -1;
        break;
    }
}

void PuzzleGrid::createPuzzlePieceBounds()
{
    for (int i = 0; i < m_numberOfPieces; ++i) {
        m_puzzlePieceBounds[i] = QRect(m_overlayGrid[pieceIDtoGridPointID(i)], QSize(pieceTotalWidth(), pieceTotalHeight()));
    }
}

QRect PuzzleGrid::puzzlePieceBounds(int pieceID)
{
    if (pieceID < m_puzzlePieceBounds.size()) return m_puzzlePieceBounds[pieceID];
    return QRect(0, 0, 0, 0);
}

void PuzzleGrid::createGridPaths(TypeOfPiece typeOfPiece)
{
    QPoint start, end;
    QRect boundsCompletePuzzle (QPoint(0, 0), QSize(m_puzzleTotalWidth, m_puzzleTotalHeight));
    QRect boundsFirstPiece;
    QRect boundsSecondPiece;
    QRect boundsForPath;
    TypeOfPiece type;
    bool hasCollision;
    int emergencyCounter;

    for (unsigned int i = 0; i < m_horizontalGridPaths.size(); ++i) {
        if (isOnRightBorder(i)) continue;

        start = m_puzzlePiecesGrid[i];
        end = m_puzzlePiecesGrid[i + 1];

        boundsFirstPiece = isOnTopBorder(i) ? boundsCompletePuzzle : m_puzzlePieceBounds[gridPointIDtoPieceID(i - m_cols)];
        boundsSecondPiece = isOnBottomBorder(i) ? boundsCompletePuzzle : m_puzzlePieceBounds[gridPointIDtoPieceID(i)];
        boundsForPath = boundsFirstPiece.intersected(boundsSecondPiece);
        type = (isOnTopBorder(i) || isOnBottomBorder(i)) ? TypeOfPiece::TRAPEZOID : typeOfPiece;

        m_horizontalGridPaths[i] = JigsawPath(start, end, boundsForPath, type).path();
    }

    for (unsigned int j = 0; j < m_verticalGridPaths.size(); ++j) {
        if (isOnBottomBorder(j)) continue;

        emergencyCounter = 0;

        start = m_puzzlePiecesGrid[j];
        end = m_puzzlePiecesGrid[j + m_cols];

        boundsFirstPiece = isOnLeftBorder(j) ? boundsCompletePuzzle : m_puzzlePieceBounds[gridPointIDtoPieceID(j - 1)];
        boundsSecondPiece = isOnRightBorder(j) ? boundsCompletePuzzle : m_puzzlePieceBounds[gridPointIDtoPieceID(j)];
        boundsForPath = boundsFirstPiece.intersected(boundsSecondPiece);
        type = (isOnLeftBorder(j) || isOnRightBorder(j)) ? TypeOfPiece::TRAPEZOID : typeOfPiece;

        do {
            m_verticalGridPaths[j] = JigsawPath(start, end, boundsForPath, type).path();

            hasCollision = (!isOnRightBorder(j)
                                && ((m_verticalGridPaths[j].intersected(m_horizontalGridPaths[j])).elementCount() > 0
                                    || (m_verticalGridPaths[j].intersected(m_horizontalGridPaths[j + m_cols])).elementCount() > 0))
                            || (!isOnLeftBorder(j)
                                && ((m_verticalGridPaths[j].intersected(m_horizontalGridPaths[j - 1])).elementCount() > 0
                                    || (m_verticalGridPaths[j].intersected(m_horizontalGridPaths[j + m_cols - 1])).elementCount() > 0));

            if (hasCollision) ++emergencyCounter;
        }
        while (hasCollision && emergencyCounter <= 20);

        if (emergencyCounter >= 20) {
            //qDebug() << "Could not solve intersection from vertical grid path" << j << "from" << start << "to" << end;
            m_verticalGridPaths[j] = JigsawPath(start, end, boundsForPath, TypeOfPiece::SIMPLEARC).path();
        }
//        else {
//            qDebug() << "Intersection solved at attemp" << emergencyCounter;
//        }
    }
}

void PuzzleGrid::createCombinedPaths()
{
    for (int i = 0; i < m_combinedPaths.size(); ++i) {
        m_combinedPaths[i] = combinePath(i);
    }
}

QPainterPath PuzzleGrid::combinePath(int pieceID) const
{
    unsigned int index1 = pieceIDtoGridPointID(pieceID, Direction::TOPLEFT);
    unsigned int index2 = pieceIDtoGridPointID(pieceID, Direction::TOPRIGHT);
    //unsigned int index3 = pieceIDtoGridPointID(pieceID, Direction::BOTTOMRIGHT);
    unsigned int index4 = pieceIDtoGridPointID(pieceID, Direction::BOTTOMLEFT);

    QPainterPath combinedPath = m_horizontalGridPaths[index1];
    combinedPath.connectPath(m_verticalGridPaths[index2]);
    combinedPath.connectPath(m_horizontalGridPaths[index4].toReversed());
    combinedPath.connectPath(m_verticalGridPaths[index1].toReversed());

    combinedPath.translate(m_overlayGrid[pieceIDtoGridPointID(pieceID)] * (-1));

    return combinedPath;
}

int PuzzleGrid::randomNumber(int min, int max)
{
    std::random_device rd;
    std::mt19937 g(rd());
    int range = (max - min) + 1;
    if (range < 1) return 1;
    int result = (g() % range) + min;
    return result;
}

void PuzzleGrid::debugGrid()
{
    QPoint topLeft;
    QPoint topRight;
    QPoint bottomRight;
    QPoint bottomLeft;

    for (int i = 0; i < m_numberOfPieces; ++i) {
        qDebug() << "Piece" << i << ":";
        qDebug() << "Puzzle pieces grid points:";
        qDebug() << "Top    Left :" << m_puzzlePiecesGrid[pieceIDtoGridPointID(i, Direction::TOPLEFT)] << "Is on Border:" << isOnBorder(pieceIDtoGridPointID(i, Direction::TOPLEFT));
        qDebug() << "Top    Right:" << m_puzzlePiecesGrid[pieceIDtoGridPointID(i, Direction::TOPRIGHT)] << "Is on Border:" << isOnBorder(pieceIDtoGridPointID(i, Direction::TOPRIGHT));
        qDebug() << "Bottom Right:" << m_puzzlePiecesGrid[pieceIDtoGridPointID(i, Direction::BOTTOMRIGHT)] << "Is on Border:" << isOnBorder(pieceIDtoGridPointID(i, Direction::BOTTOMRIGHT));
        qDebug() << "Bottom Left :" << m_puzzlePiecesGrid[pieceIDtoGridPointID(i, Direction::BOTTOMLEFT)] << "Is on Border:" << isOnBorder(pieceIDtoGridPointID(i, Direction::BOTTOMLEFT));

        qDebug() << "Bounds:" << m_puzzlePieceBounds[i];

        qDebug() << "Path:" << m_combinedPaths[i];

        qDebug() << "Inside bounds:" << m_puzzlePieceBounds[0].contains(m_combinedPaths[i].controlPointRect().toAlignedRect());
    }
}

PuzzleGrid::PuzzleGrid(int rowsOfPieces, int colsOfPieces, int puzzlePiecesWidth, int puzzlePiecesHeight, TypeOfPiece typeOfPiece, QObject *parent)
    : QObject{parent}
    , m_rows(rowsOfPieces + 1)
    , m_cols(colsOfPieces + 1)
    , m_numberOfGridPoints(m_rows * m_cols)
    , m_numberOfPieces(rowsOfPieces * colsOfPieces)
    , m_typeOfPiece(typeOfPiece)
    , m_puzzlePiecesWidth(puzzlePiecesWidth)
    , m_puzzlePiecesHeight(puzzlePiecesHeight)
    , m_horizontalOverlap(0)
    , m_verticalOverlap(0)
    , m_puzzleTotalWidth(0)
    , m_puzzleTotalHeight(0)
    , m_symmetricGrid(m_numberOfGridPoints)
    , m_overlayGrid(m_numberOfGridPoints)
    , m_puzzlePiecesGrid(m_numberOfGridPoints)
    , m_puzzlePieceBounds(m_numberOfPieces)
    , m_horizontalGridPaths(m_numberOfGridPoints)
    , m_verticalGridPaths(m_numberOfGridPoints)
    , m_combinedPaths(m_numberOfPieces)
{
    calculateHorizontalOverlap();
    calculateVerticalOverlap();
    calculateTotalWidth();
    calculateTotalHeight();

    createGrids();
    createPuzzlePieceBounds();
    createGridPaths(m_typeOfPiece);
    createCombinedPaths();

    //debugGrid();
}

PuzzleGrid::~PuzzleGrid()
{

}

QPoint PuzzleGrid::symmetricGridPoint(int pieceID, Direction direction) const
{
    unsigned int index1 = pieceID + (pieceID / (m_cols - 1));
    unsigned int index2 = index1 + 1;
    unsigned int index3 = index2 + m_cols;
    unsigned int index4 = index3 - 1;

    switch (direction) {
    case Direction::TOPLEFT:
        return m_symmetricGrid[index1];
        break;
    case Direction::TOPRIGHT:
        return m_symmetricGrid[index2];
        break;
    case Direction::BOTTOMRIGHT:
        return m_symmetricGrid[index3];
        break;
    case Direction::BOTTOMLEFT:
        return m_symmetricGrid[index4];
        break;
    default:
        return m_symmetricGrid[index1];
        break;
    }
}

QPoint PuzzleGrid::overlayGridPoint(int pieceID, Direction direction) const
{
    unsigned int index1 = pieceID + (pieceID / (m_cols - 1));
    unsigned int index2 = index1 + 1;
    unsigned int index3 = index2 + m_cols;
    unsigned int index4 = index3 - 1;

    switch (direction) {
    case Direction::TOPLEFT:
        return m_overlayGrid[index1];
        break;
    case Direction::TOPRIGHT:
        return m_overlayGrid[index2];
        break;
    case Direction::BOTTOMRIGHT:
        return m_overlayGrid[index3];
        break;
    case Direction::BOTTOMLEFT:
        return m_overlayGrid[index4];
        break;
    default:
        return m_overlayGrid[index1];
        break;
    }
}

QPoint PuzzleGrid::puzzlePieceGridPoint(int pieceID, Direction direction) const
{
    unsigned int index1 = pieceID + (pieceID / (m_cols - 1));
    unsigned int index2 = index1 + 1;
    unsigned int index3 = index2 + m_cols;
    unsigned int index4 = index3 - 1;

    switch (direction) {
    case Direction::TOPLEFT:
        return m_puzzlePiecesGrid[index1];
        break;
    case Direction::TOPRIGHT:
        return m_puzzlePiecesGrid[index2];
        break;
    case Direction::BOTTOMRIGHT:
        return m_puzzlePiecesGrid[index3];
        break;
    case Direction::BOTTOMLEFT:
        return m_puzzlePiecesGrid[index4];
        break;
    default:
        return m_puzzlePiecesGrid[index1];
        break;
    }
}
