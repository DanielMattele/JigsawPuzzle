#ifndef JIGSAWPATH_H
#define JIGSAWPATH_H

#include "customjigsawpath.h"
#include <QPainterPath>
#include <QString>
#include <random>
#include <QDebug>

enum class TypeOfPiece
{
    TRAPEZOID,
    SIMPLEARC,
    TRIANGLECONNECTIONS,
    SIMPLECIRCLECONNECTIONS,
    STANDARD,
    STANDARDFUNNY,
    CUSTOM,
    count
};

class JigsawPath
{
public:
    JigsawPath();
    JigsawPath(const QPoint &start, const QPoint &end, const QRect &bounds, TypeOfPiece typeOfPiece, const CustomJigsawPath &customPath = CustomJigsawPath(), bool noRandom = false);
    ~JigsawPath();

    void setStart(const QPoint &start);
    void setEnd(const QPoint &end);
    void setBounds(const QRect &bounds);
    void setTypeOfPiece(TypeOfPiece typeOfPiece, const CustomJigsawPath &customPath = CustomJigsawPath(), bool noRandom = false);

    QPainterPath path() const;
    QPoint start() const;
    QPoint end() const;
    QRect bounds() const;
    TypeOfPiece typeOfPiece() const;
    bool creatingPathSuccessful() const;

    int typeOfPieceToInt() const;
    QString tooltip() const;

    static TypeOfPiece intToTypeOfPiece(int index);
    static int typeOfPieceToInt(TypeOfPiece type);
    static QString tooltip(TypeOfPiece type);

    static QPainterPath singleJigsawPiecePath(const QRect &outerBounds, const QRect &innerBounds, TypeOfPiece typeOfPiece,
                                              int minForcedPaths = 2, bool useRecommendedInnerBounds = false,
                                              const CustomJigsawPath &customPath = CustomJigsawPath());

    /* WARNING: This function is very slow. It can take a few minutes to process.*/
    static QVector<double> calculateRecommendedInnerBoundsPercentage(QVector<TypeOfPiece> types = {TypeOfPiece::TRAPEZOID,
                                                                                           TypeOfPiece::SIMPLEARC,
                                                                                           TypeOfPiece::TRIANGLECONNECTIONS,
                                                                                           TypeOfPiece::SIMPLECIRCLECONNECTIONS,
                                                                                           TypeOfPiece::STANDARD,
                                                                                           TypeOfPiece::STANDARDFUNNY},
                                                             QSize size = QSize(1000, 1000), int startPercentage = 100, int startPaths = 1,
                                                             const CustomJigsawPath &customPath = CustomJigsawPath());

private:
    int MAXATTEMPTSPERPATH = 30;

    QPoint m_start, m_end;
    QPointF m_distanceVector, m_orthogonalVector, m_middlePoint;
    QRect m_bounds;
    TypeOfPiece m_typeOfPiece;
    QPainterPath m_path;
    bool m_successful;
    CustomJigsawPath m_customPath;
    bool m_noRandom;
    bool m_debug = false;

    void calculateParameters();

    void generatePath();

    bool generateTrapezoidPath();
    bool generateSimpleArcPath();
    bool generateTriangleConnectionsPath();
    bool generateSimpleCircleConnectionsPath();
    bool generateStandardPath();
    bool generateStandardFunnyPath();
    bool generateCustomPath();

    static bool pathHasCollisions(const JigsawPath &jigsawPath, const QVector<JigsawPath> &collisionPaths);
    static int randomNumber(int min, int max);
    static QRect recommendedInnerBounds(const QRect &outerBounds, TypeOfPiece typeOfPiece, int minForcedPaths);
};

#endif // JIGSAWPATH_H
