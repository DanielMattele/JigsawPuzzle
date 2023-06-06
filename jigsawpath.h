#ifndef JIGSAWPATH_H
#define JIGSAWPATH_H

#include "customjigsawpath.h"
#include <QPainterPath>
#include <QString>
#include <random>
#include <QDebug>
#include "Jigsaw.h"


/*
 * The JigsawPath class creates the different shapes of the jigsaw pieces. These different shapes can be chosen via the
 * enum class TypeOfPiece. Every type of piece (except the trapezoid piece) has some random element to it. The function
 * generatePath() generates a path which lies completely inside the given bounds. This function is repeated with different
 * random numbers until a valid path is found or a limit for max attempts is reached. In that case, a simpler path is drawn.
 * There is the possibility of creating a custom path via an editor, but this function isn't fully implemented yet.
 */

class JigsawPath
{
public:
    JigsawPath();
    JigsawPath(const QPoint &start, const QPoint &end, const QRect &bounds, Jigsaw::TypeOfPiece typeOfPiece, const CustomJigsawPath &customPath = CustomJigsawPath(), bool noRandom = false);
    ~JigsawPath();

    void setStart(const QPoint &start);
    void setEnd(const QPoint &end);
    void setBounds(const QRect &bounds);
    void setTypeOfPiece(Jigsaw::TypeOfPiece typeOfPiece, const CustomJigsawPath &customPath = CustomJigsawPath(), bool noRandom = false);

    QPainterPath path() const;
    QPoint start() const;
    QPoint end() const;
    QRect bounds() const;
    Jigsaw::TypeOfPiece typeOfPiece() const;
    bool creatingPathSuccessful() const;

    int typeOfPieceToInt() const;
    QString tooltip() const;

    static Jigsaw::TypeOfPiece intToTypeOfPiece(int index);
    static int typeOfPieceToInt(Jigsaw::TypeOfPiece type);
    static QString tooltip(Jigsaw::TypeOfPiece type);


    /*
     * This function creates a single jigsaw border. It can be used, for example, to create the shape of a JigsawButton. You can either
     * submit the inner bounds (where the textarea is) yourself or use the recommended inner bounds. They guarantee that the function has enough
     * room between outer and inner bounds to find valid paths.
     */
    static QPainterPath singleJigsawPiecePath(const QRect &outerBounds, const QRect &innerBounds, Jigsaw::TypeOfPiece typeOfPiece,
                                              int minForcedPaths = 2, bool useRecommendedInnerBounds = false,
                                              const CustomJigsawPath &customPath = CustomJigsawPath());

    /* WARNING: This function is very slow. It can take a few minutes to process.*/
    static QVector<double> calculateRecommendedInnerBoundsPercentage(QVector<Jigsaw::TypeOfPiece> types = {Jigsaw::TypeOfPiece::TRAPEZOID,
                                                                                           Jigsaw::TypeOfPiece::SIMPLEARC,
                                                                                           Jigsaw::TypeOfPiece::TRIANGLECONNECTIONS,
                                                                                           Jigsaw::TypeOfPiece::SIMPLECIRCLECONNECTIONS,
                                                                                           Jigsaw::TypeOfPiece::STANDARD,
                                                                                           Jigsaw::TypeOfPiece::STANDARDFUNNY},
                                                             QSize size = QSize(1000, 1000), int startPercentage = 100, int startPaths = 1,
                                                             const CustomJigsawPath &customPath = CustomJigsawPath());

private:
    int MAXATTEMPTSPERPATH = 30;

    QPoint m_start, m_end;
    QPointF m_distanceVector, m_orthogonalVector, m_middlePoint;
    QRect m_bounds;
    Jigsaw::TypeOfPiece m_typeOfPiece;
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
    static QRect recommendedInnerBounds(const QRect &outerBounds, Jigsaw::TypeOfPiece typeOfPiece, int minForcedPaths);
};

#endif // JIGSAWPATH_H
