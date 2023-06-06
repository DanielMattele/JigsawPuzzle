#include "jigsawpath.h"

JigsawPath::JigsawPath()
    : m_start(QPoint(0, 0))
    , m_end(QPoint(10, 10))
    , m_bounds(m_start, m_end)
    , m_typeOfPiece(Jigsaw::TypeOfPiece::TRAPEZOID)
{
    calculateParameters();
    generatePath();
}

JigsawPath::JigsawPath(const QPoint &start, const QPoint &end, const QRect &bounds, Jigsaw::TypeOfPiece typeOfPiece, const CustomJigsawPath &customPath, bool noRandom)
    : m_start(start)
    , m_end(end)
    , m_bounds(bounds)
    , m_typeOfPiece(typeOfPiece)
    , m_customPath(customPath)
    , m_noRandom(noRandom)
{
    if (typeOfPiece == Jigsaw::TypeOfPiece::count) m_typeOfPiece = Jigsaw::TypeOfPiece::TRAPEZOID;
    calculateParameters();
    generatePath();
}

JigsawPath::~JigsawPath()
{

}

void JigsawPath::setStart(const QPoint &start)
{
    m_start = start;
    calculateParameters();
    generatePath();
}

void JigsawPath::setEnd(const QPoint &end)
{
    m_end = end;
    calculateParameters();
    generatePath();
}

void JigsawPath::setBounds(const QRect &bounds)
{
    m_bounds = bounds;
    generatePath();
}

void JigsawPath::setTypeOfPiece(Jigsaw::TypeOfPiece typeOfPiece, const CustomJigsawPath &customPath, bool noRandom)
{
    m_typeOfPiece = typeOfPiece;
    m_customPath = customPath;
    m_noRandom = noRandom;
    if (typeOfPiece == Jigsaw::TypeOfPiece::count) m_typeOfPiece = Jigsaw::TypeOfPiece::TRAPEZOID;
    generatePath();
}

QPainterPath JigsawPath::path() const
{
    return m_path;
}

QPoint JigsawPath::start() const
{
    return m_start;
}

QPoint JigsawPath::end() const
{
    return m_end;
}

QRect JigsawPath::bounds() const
{
    return m_bounds;
}

Jigsaw::TypeOfPiece JigsawPath::typeOfPiece() const
{
    return m_typeOfPiece;
}

bool JigsawPath::creatingPathSuccessful() const
{
    return m_successful;
}

int JigsawPath::typeOfPieceToInt() const
{
    return JigsawPath::typeOfPieceToInt(m_typeOfPiece);
}

QString JigsawPath::tooltip() const
{
    return JigsawPath::tooltip(m_typeOfPiece);
}

Jigsaw::TypeOfPiece JigsawPath::intToTypeOfPiece(int index)
{
    switch (index) {
    case (0):
        return Jigsaw::TypeOfPiece::TRAPEZOID;
    case (1):
        return Jigsaw::TypeOfPiece::SIMPLEARC;
    case (2):
        return Jigsaw::TypeOfPiece::TRIANGLECONNECTIONS;
    case (3):
        return Jigsaw::TypeOfPiece::SIMPLECIRCLECONNECTIONS;
    case (4):
        return Jigsaw::TypeOfPiece::STANDARD;
    case (5):
        return Jigsaw::TypeOfPiece::STANDARDFUNNY;
    case (6):
        return Jigsaw::TypeOfPiece::CUSTOM;
    default:
        return Jigsaw::TypeOfPiece::TRAPEZOID;
    }
}

int JigsawPath::typeOfPieceToInt(Jigsaw::TypeOfPiece type)
{
    switch (type) {
    case Jigsaw::TypeOfPiece::TRAPEZOID:
        return 0;
    case Jigsaw::TypeOfPiece::SIMPLEARC:
        return 1;
    case Jigsaw::TypeOfPiece::TRIANGLECONNECTIONS:
        return 2;
    case Jigsaw::TypeOfPiece::SIMPLECIRCLECONNECTIONS:
        return 3;
    case Jigsaw::TypeOfPiece::STANDARD:
        return 4;
    case Jigsaw::TypeOfPiece::STANDARDFUNNY:
        return 5;
    case Jigsaw::TypeOfPiece::CUSTOM:
        return 6;
    default:
        return 0;
    }
}

QString JigsawPath::tooltip(Jigsaw::TypeOfPiece type)
{
    switch (type) {
    case Jigsaw::TypeOfPiece::TRAPEZOID:
        return "A simple form with straight lines.";
    case Jigsaw::TypeOfPiece::SIMPLEARC:
        return "A simple form with curved lines.";
    case Jigsaw::TypeOfPiece::TRIANGLECONNECTIONS:
        return "Straight lines with little triangles in the middle.";
    case Jigsaw::TypeOfPiece::SIMPLECIRCLECONNECTIONS:
        return "Straight lines with little curves in the middle.";
    case Jigsaw::TypeOfPiece::STANDARD:
        return "Standard jigsaw pieces.";
    case Jigsaw::TypeOfPiece::STANDARDFUNNY:
        return "Funny looking jigsaw pieces for kids.";
    case Jigsaw::TypeOfPiece::CUSTOM:
        return "Create your own design!";
    default:
        return "Tooltip missing";
    }
}

QPainterPath JigsawPath::singleJigsawPiecePath(const QRect &outerBounds, const QRect &innerBounds, Jigsaw::TypeOfPiece typeOfPiece,
                                               int minForcedPaths, bool useRecommendedInnerBounds,
                                               const CustomJigsawPath &customPath)
{
    if (typeOfPiece == Jigsaw::TypeOfPiece::count) return QPainterPath();

    QRect usedInnerBounds = useRecommendedInnerBounds && typeOfPiece != Jigsaw::TypeOfPiece::CUSTOM
                            ? recommendedInnerBounds(outerBounds, typeOfPiece, minForcedPaths)
                            : innerBounds;
    QPoint topLeft, topRight, bottomRight, bottomLeft;
    QRect topBounds, rightBounds, bottomBounds, leftBounds;
    QVector<JigsawPath> jigsawPaths(4);
    QPainterPath path;
    bool debug = false;
    int collisions = 0;
    int unsuccessful = 0;
    int emergencyCounter = 0;
    int maxUnsuccessfulPaths = minForcedPaths > 0 && minForcedPaths <= 4 ? 4 - minForcedPaths : 2;
    int maxAttempts = 50;

    do {
        collisions = 0;
        unsuccessful = 0;
        topLeft = QPoint(Jigsaw::randomNumber(outerBounds.left(), usedInnerBounds.left()), Jigsaw::randomNumber(outerBounds.top(), usedInnerBounds.top()));
        topRight = QPoint(Jigsaw::randomNumber(usedInnerBounds.right(), outerBounds.right()), Jigsaw::randomNumber(outerBounds.top(), usedInnerBounds.top()));
        bottomRight = QPoint(Jigsaw::randomNumber(usedInnerBounds.right(), outerBounds.right()), Jigsaw::randomNumber(usedInnerBounds.bottom(), outerBounds.bottom()));
        bottomLeft = QPoint(Jigsaw::randomNumber(outerBounds.left(), usedInnerBounds.left()), Jigsaw::randomNumber(usedInnerBounds.bottom(), outerBounds.bottom()));

        topBounds = QRect(outerBounds.topLeft(), QPoint(outerBounds.right(), usedInnerBounds.top()));
        rightBounds = QRect(QPoint(usedInnerBounds.right(), outerBounds.top()), outerBounds.bottomRight());
        bottomBounds = QRect(QPoint(outerBounds.left(), usedInnerBounds.bottom()), outerBounds.bottomRight());
        leftBounds = QRect(outerBounds.topLeft(), QPoint(usedInnerBounds.left(), outerBounds.bottom()));

        jigsawPaths[0] = JigsawPath(topLeft, topRight, topBounds, typeOfPiece, customPath);
        jigsawPaths[1] = JigsawPath(topRight, bottomRight, rightBounds, typeOfPiece, customPath);
        jigsawPaths[2] = JigsawPath(bottomRight, bottomLeft, bottomBounds, typeOfPiece, customPath);
        jigsawPaths[3] = JigsawPath(bottomLeft, topLeft, leftBounds, typeOfPiece, customPath);

        for (const auto &jigsawPath : jigsawPaths) {
            collisions += pathHasCollisions(jigsawPath, jigsawPaths) ? 1 : 0;
            unsuccessful += jigsawPath.creatingPathSuccessful() ? 0 : 1;
            if (debug) qDebug() << jigsawPath.creatingPathSuccessful();
            if (debug) qDebug() << unsuccessful;
        }

        ++emergencyCounter;
    }
    while ((unsuccessful > maxUnsuccessfulPaths || collisions > 0) && emergencyCounter < maxAttempts);

    if (emergencyCounter >= maxAttempts && collisions > 0) {
        if (debug) qDebug() << "Could not generate paths without collisions; generating trapezoid instead.";
        path = JigsawPath(topLeft, topRight, topBounds, Jigsaw::TypeOfPiece::TRAPEZOID).path();
        path.connectPath(JigsawPath(topRight, bottomRight, rightBounds, Jigsaw::TypeOfPiece::TRAPEZOID).path());
        path.connectPath(JigsawPath(bottomRight, bottomLeft, bottomBounds, Jigsaw::TypeOfPiece::TRAPEZOID).path());
        path.connectPath(JigsawPath(bottomLeft, topLeft, leftBounds, Jigsaw::TypeOfPiece::TRAPEZOID).path());
    }
    else {
        if (debug) qDebug() << "Paths generated at attemp" << emergencyCounter;
        path = jigsawPaths[0].path();
        path.connectPath(jigsawPaths[1].path());
        path.connectPath(jigsawPaths[2].path());
        path.connectPath(jigsawPaths[3].path());
    }

    return path;
}

void JigsawPath::calculateParameters()
{
    m_distanceVector = m_end - m_start;
    m_orthogonalVector.setX(-m_distanceVector.y());
    m_orthogonalVector.setY(m_distanceVector.x());
    m_middlePoint = m_start + m_distanceVector / 2;
}

void JigsawPath::generatePath()
{
    switch (m_typeOfPiece) {
    case Jigsaw::TypeOfPiece::TRAPEZOID:
        m_successful = generateTrapezoidPath();
        break;
    case Jigsaw::TypeOfPiece::SIMPLEARC:
        m_successful = generateSimpleArcPath();
        break;
    case Jigsaw::TypeOfPiece::TRIANGLECONNECTIONS:
        m_successful = generateTriangleConnectionsPath();
        break;
    case Jigsaw::TypeOfPiece::SIMPLECIRCLECONNECTIONS:
        m_successful = generateSimpleCircleConnectionsPath();
        break;
    case Jigsaw::TypeOfPiece::STANDARD:
        m_successful = generateStandardPath();
        break;
    case Jigsaw::TypeOfPiece::STANDARDFUNNY:
        m_successful = generateStandardFunnyPath();
        break;
    case Jigsaw::TypeOfPiece::CUSTOM:
        m_successful = generateCustomPath();
        break;
    default:
        m_successful = generateTrapezoidPath();
        break;
    }
}

bool JigsawPath::generateTrapezoidPath()
{
    m_path.clear();
    m_path.moveTo(m_start);
    m_path.lineTo(m_end);
    return true;
}

bool JigsawPath::generateSimpleArcPath()
{
    QPointF controlPoint;

    int offset;
    int side;
    int emergencyCounter = 0;

    do {
        m_path.clear();

        offset = Jigsaw::randomNumber(5, 8);
        side = Jigsaw::randomNumber(1, 2);
        side = (side == 1) ? 1 : -1;
        controlPoint = m_middlePoint + m_orthogonalVector / offset * side;
        ++emergencyCounter;

        m_path.moveTo(m_start);
        m_path.quadTo(controlPoint, m_end);
    }
    while (!m_bounds.contains(m_path.boundingRect().toAlignedRect()) && emergencyCounter <= MAXATTEMPTSPERPATH);

    if (emergencyCounter >= MAXATTEMPTSPERPATH) {
        if (m_debug) qDebug() << "Could not generate path from" << m_start << "to" << m_end;
        generateTrapezoidPath();
        return false;
    }
    else {
        if (m_debug) qDebug() << "Path generated at attemp" << emergencyCounter;
        return true;
    }
}

bool JigsawPath::generateTriangleConnectionsPath()
{
    QPointF triangleStartingPoint = m_start + m_distanceVector / 5 * 2;
    QPointF triangleTipPoint;
    QPointF triangleEndingPoint = m_end - m_distanceVector / 5 * 2;

    int offset;
    int side;
    int emergencyCounter = 0;

    do {
        m_path.clear();

        offset = Jigsaw::randomNumber(3, 5);
        side = Jigsaw::randomNumber(1, 2);
        side = (side == 1) ? 1 : -1;
        triangleTipPoint = m_middlePoint + m_orthogonalVector / offset * side;
        ++emergencyCounter;

        m_path.moveTo(m_start);
        m_path.lineTo(triangleStartingPoint);
        m_path.lineTo(triangleTipPoint);
        m_path.lineTo(triangleEndingPoint);
        m_path.lineTo(m_end);
    }
    while (!m_bounds.contains(m_path.boundingRect().toAlignedRect()) && emergencyCounter <= MAXATTEMPTSPERPATH);

    if (emergencyCounter >= MAXATTEMPTSPERPATH) {
        if (m_debug) qDebug() << "Could not generate path from" << m_start << "to" << m_end;
        generateTrapezoidPath();
        return false;
    }
    else {
        if (m_debug) qDebug() << "Path generated at attemp" << emergencyCounter;
        return true;
    }
}

bool JigsawPath::generateSimpleCircleConnectionsPath()
{
    QPointF quadStartingPoint = m_start + m_distanceVector / 5 * 2;
    QPointF controlPoint;
    QPointF quadEndindPoint = m_end - m_distanceVector / 5 * 2;

    int offset;
    int side;
    int emergencyCounter = 0;

    do {
        m_path.clear();

        offset = Jigsaw::randomNumber(3, 5);
        side = Jigsaw::randomNumber(1, 2);
        side = (side == 1) ? 1 : -1;
        controlPoint = m_middlePoint + m_orthogonalVector / offset * side;
        ++emergencyCounter;

        m_path.moveTo(m_start);
        m_path.lineTo(quadStartingPoint);
        m_path.quadTo(controlPoint, quadEndindPoint);
        m_path.lineTo(m_end);
    }
    while (!m_bounds.contains(m_path.boundingRect().toAlignedRect()) && emergencyCounter <= MAXATTEMPTSPERPATH);

    if (emergencyCounter >= MAXATTEMPTSPERPATH) {
        if (m_debug) qDebug() << "Could not generate path from" << m_start << "to" << m_end;
        generateTrapezoidPath();
        return false;
    }
    else {
        if (m_debug) qDebug() << "Path generated at attemp" << emergencyCounter;
        return true;
    }
}

bool JigsawPath::generateStandardPath()
{
    QVector<QPointF> pathPoint(10);
    pathPoint[1] = 1.0 * m_start;
    pathPoint[9] = 1.0 * m_end;

    QVector<QPointF> controlPoint(9);

    int side;
    int size;
    int emergencyCounter = 0;

    QPointF distanceUnitVector;
    QPointF orthogonalUnitVector;
    QPointF intersectionPoint(0.0, 0.0);

    do {
        m_path.clear();

        side = Jigsaw::randomNumber(1, 2);
        side = (side == 1) ? 1 : -1;

        size = Jigsaw::randomNumber(12, 20);
        distanceUnitVector = m_distanceVector / size;
        orthogonalUnitVector = m_orthogonalVector / size;

        pathPoint[2] = 1.0 * m_middlePoint - distanceUnitVector;
        pathPoint[3] = 1.0 * m_middlePoint - 1.5 * distanceUnitVector + orthogonalUnitVector * side;
        pathPoint[4] = 1.0 * m_middlePoint - 1.5 * distanceUnitVector + 3 * orthogonalUnitVector * side;
        pathPoint[5] = 1.0 * m_middlePoint + 3.5 * orthogonalUnitVector * side;
        pathPoint[6] = 1.0 * m_middlePoint + 1.5 * distanceUnitVector + 3 * orthogonalUnitVector * side;
        pathPoint[7] = 1.0 * m_middlePoint + 1.5 * distanceUnitVector + orthogonalUnitVector * side;
        pathPoint[8] = 1.0 * m_middlePoint + distanceUnitVector;

        controlPoint[1] = 1.0 * m_middlePoint - distanceUnitVector - 2 * orthogonalUnitVector * side;
        controlPoint[3] = 1.0 * m_middlePoint - 2.5 * distanceUnitVector + 2 * orthogonalUnitVector * side;
        controlPoint[6] = 1.0 * m_middlePoint + 2.5 * distanceUnitVector + 2 * orthogonalUnitVector * side;
        controlPoint[8] = 1.0 * m_middlePoint + distanceUnitVector - 2 * orthogonalUnitVector * side;

        QLineF(controlPoint[1], pathPoint[2]).intersects(QLineF(pathPoint[3], controlPoint[3]), &intersectionPoint);
        controlPoint[2] = intersectionPoint;
        intersectionPoint.setX(0.0);
        intersectionPoint.setY(0.0);
        QLineF(controlPoint[3], pathPoint[4]).intersects(QLineF(pathPoint[5], pathPoint[5] + distanceUnitVector), &intersectionPoint);
        controlPoint[4] = intersectionPoint;
        intersectionPoint.setX(0.0);
        intersectionPoint.setY(0.0);
        QLineF(controlPoint[6], pathPoint[6]).intersects(QLineF(pathPoint[5], pathPoint[5] + distanceUnitVector), &intersectionPoint);
        controlPoint[5] = intersectionPoint;
        intersectionPoint.setX(0.0);
        intersectionPoint.setY(0.0);
        QLineF(controlPoint[8], pathPoint[8]).intersects(QLineF(pathPoint[7], controlPoint[6]), &intersectionPoint);
        controlPoint[7] = intersectionPoint;
        intersectionPoint.setX(0.0);
        intersectionPoint.setY(0.0);

        ++emergencyCounter;

        m_path.moveTo(pathPoint[1]);
        m_path.quadTo(controlPoint[1], pathPoint[2]);
        m_path.quadTo(controlPoint[2], pathPoint[3]);
        m_path.quadTo(controlPoint[3], pathPoint[4]);
        m_path.quadTo(controlPoint[4], pathPoint[5]);
        m_path.quadTo(controlPoint[5], pathPoint[6]);
        m_path.quadTo(controlPoint[6], pathPoint[7]);
        m_path.quadTo(controlPoint[7], pathPoint[8]);
        m_path.quadTo(controlPoint[8], pathPoint[9]);
    }
    while ((!m_bounds.contains(m_path.boundingRect().toAlignedRect())
          || controlPoint[2].isNull() || controlPoint[4].isNull()
          || controlPoint[5].isNull() || controlPoint[7].isNull())
          && emergencyCounter <= MAXATTEMPTSPERPATH);

    if (emergencyCounter >= MAXATTEMPTSPERPATH) {
        if (m_debug) qDebug() << "Could not generate path from" << m_start << "to" << m_end;
        generateSimpleArcPath();
        return false;
    }
    else {
        if (m_debug) qDebug() << "Path generated at attemp" << emergencyCounter;
        return true;
    }
}

bool JigsawPath::generateStandardFunnyPath()
{
    QVector<QPointF> pathPoint(8);
    pathPoint[1] = m_start;
    pathPoint[7] = m_end;

    QVector<QPointF> controlPoint(7);

    QPointF intersectionPoint(0.0, 0.0);

    int offsetBaseline = 50;
    int randomNumberMin = -2;
    int randomNumberMax = 2;

    QVector<int> offsetPathPoint;
    QVector<int> orthogonalOffsetPathPoint;
    QVector<int> offsetControlPoint;
    QVector<int> orthogonalOffsetControlPoint;

    int side;
    int emergencyCounter = 0;

    do {
        m_path.clear();

        offsetPathPoint = QVector<int>(8, Jigsaw::randomNumber(randomNumberMin, randomNumberMax));
        orthogonalOffsetPathPoint = QVector<int>(8, Jigsaw::randomNumber(randomNumberMin, randomNumberMax));
        offsetControlPoint = QVector<int>(7, Jigsaw::randomNumber(randomNumberMin, randomNumberMax));
        orthogonalOffsetControlPoint = QVector<int>(7, Jigsaw::randomNumber(randomNumberMin, randomNumberMax));

        orthogonalOffsetControlPoint[3] = orthogonalOffsetPathPoint[4];
        orthogonalOffsetControlPoint[4] = orthogonalOffsetPathPoint[4];

        side = Jigsaw::randomNumber(1, 2);
        side = (side == 1) ? 1 : -1;

        pathPoint[2] = pathPoint[1] + m_distanceVector / 5 * 2;
        pathPoint[3] = pathPoint[2] + m_orthogonalVector / 10 * side;
        pathPoint[4] = m_middlePoint + m_orthogonalVector / 3 * side;
        pathPoint[5] = pathPoint[3] + m_distanceVector / 5;
        pathPoint[6] = pathPoint[2] + m_distanceVector / 5;

        controlPoint[1] = pathPoint[1] + m_distanceVector / 5 - m_orthogonalVector / 10 * side;
        controlPoint[3] = pathPoint[1] + m_distanceVector / 4 + m_orthogonalVector / 3 * side;
        controlPoint[4] = pathPoint[7] - m_distanceVector / 4 + m_orthogonalVector / 3 * side;
        controlPoint[6] = pathPoint[7] - m_distanceVector / 5 - m_orthogonalVector / 10 * side;

        for (int i = 1; i <= 6; ++i) {
            if (i != 1) pathPoint[i] += m_distanceVector / offsetBaseline * offsetPathPoint[i] + m_orthogonalVector / offsetBaseline * orthogonalOffsetPathPoint[i];
            if (i != 2 && i != 5) controlPoint[i] += m_distanceVector / offsetBaseline * offsetControlPoint[i] + m_orthogonalVector / offsetBaseline * orthogonalOffsetControlPoint[i];
        }

        QLineF(controlPoint[1], pathPoint[2]).intersects(QLineF(controlPoint[3], pathPoint[3]), &intersectionPoint);
        controlPoint[2] = intersectionPoint;
        intersectionPoint.setX(0.0);
        intersectionPoint.setY(0.0);
        QLineF(controlPoint[6], pathPoint[6]).intersects(QLineF(controlPoint[4], pathPoint[5]), &intersectionPoint);
        controlPoint[5] = intersectionPoint;
        intersectionPoint.setX(0.0);
        intersectionPoint.setY(0.0);

        ++emergencyCounter;

        m_path.moveTo(pathPoint[1]);
        m_path.quadTo(controlPoint[1], pathPoint[2]);
        m_path.quadTo(controlPoint[2], pathPoint[3]);
        m_path.quadTo(controlPoint[3], pathPoint[4]);
        m_path.quadTo(controlPoint[4], pathPoint[5]);
        m_path.quadTo(controlPoint[5], pathPoint[6]);
        m_path.quadTo(controlPoint[6], pathPoint[7]);
    }
    while ((!m_bounds.contains(m_path.boundingRect().toAlignedRect())
          || controlPoint[2].isNull() || controlPoint[5].isNull())
          && emergencyCounter <= MAXATTEMPTSPERPATH);

    if (emergencyCounter >= MAXATTEMPTSPERPATH) {
        if (m_debug) qDebug() << "Could not generate path from" << m_start << "to" << m_end;
        generateSimpleArcPath();
        return false;
    }
    else {
        if (m_debug) qDebug() << "Path generated at attemp" << emergencyCounter;
        return true;
    }
}

bool JigsawPath::generateCustomPath()
{
    CustomJigsawPath customPathTemp;
    if (m_noRandom) customPathTemp = m_customPath;

    QLineF lineStartToEnd(m_start, m_end);
    double lengthStartToEnd = lineStartToEnd.length();
    double angleStartToEnd = lineStartToEnd.angle();

    QLineF customLineStartToEnd(m_customPath.pathPoint(0).position, m_customPath.pathPoint(m_customPath.numberOfPoints() - 1).position);
    double customLengthStartToEnd = customLineStartToEnd.length();

    QVector<QPointF> actualPathPoints;
    actualPathPoints.push_back(m_start);

    QVector<QPointF> actualControlPoints;
    actualControlPoints.push_back(QPoint());

    int side;
    int emergencyCounter = 0;

    do {

        side = Jigsaw::randomNumber(1, 2);
        side = (side == 1) ? 1 : -1;
        if (m_noRandom) side = 1;
        else customPathTemp = m_customPath.getRandomizedPath();

        for (int i = 1; i < customPathTemp.numberOfPoints(); ++i) {
            QLineF customLineStartToPathPoint(customPathTemp.pathPoint(0).position, customPathTemp.pathPoint(i).position);
            double angle = angleStartToEnd + customLineStartToEnd.angleTo(customLineStartToPathPoint) * side;
            double length = customLineStartToPathPoint.length() / customLengthStartToEnd * lengthStartToEnd;
            QLineF lineStartToActualPathPoint;
            lineStartToActualPathPoint.setP1(m_start);
            lineStartToActualPathPoint.setAngle(angle);
            lineStartToActualPathPoint.setLength(length);
            QPointF actualPathPoint = lineStartToActualPathPoint.p2();
            actualPathPoints.push_back(actualPathPoint);

            QLineF customLineStartToControlPoint(customPathTemp.pathPoint(0).position, customPathTemp.pathPoint(i).positionControlPoint);
            angle = angleStartToEnd + customLineStartToEnd.angleTo(customLineStartToControlPoint) * side;
            length = customLineStartToControlPoint.length() / customLengthStartToEnd * lengthStartToEnd;
            QLineF lineStartToActualControlPoint;
            lineStartToActualControlPoint.setP1(m_start);
            lineStartToActualControlPoint.setAngle(angle);
            lineStartToActualControlPoint.setLength(length);
            QPointF actualControlPoint = lineStartToActualControlPoint.p2();
            actualControlPoints.push_back(actualControlPoint);
        }

        ++emergencyCounter;

        m_path.clear();
        m_path.moveTo(m_start);

        for (int i = 1; i < actualPathPoints.size(); ++i) {
            m_customPath.pathPoint(i).typeOfLine == CustomJigsawPath::TypeOfLine::STRAIGHT
                    ? m_path.lineTo(actualPathPoints[i])
                    : m_path.quadTo(actualControlPoints[i], actualPathPoints[i]);
        }
        if (m_noRandom) break;
    }
    while(!m_bounds.contains(m_path.boundingRect().toAlignedRect()) && emergencyCounter <= MAXATTEMPTSPERPATH);

    if (emergencyCounter >= MAXATTEMPTSPERPATH) {
        if (m_debug) qDebug() << "Could not generate path from" << m_start << "to" << m_end;
        generateSimpleArcPath();
        return false;
    }
    else {
        if (m_debug) qDebug() << "Path generated at attemp" << emergencyCounter;
        return true;
    }
}

bool JigsawPath::pathHasCollisions(const JigsawPath &jigsawPath, const QVector<JigsawPath> &collisionPaths)
{
    bool checkFromHere = false;

    for (const auto &collisionPath : collisionPaths) {
        if (collisionPath.path() == jigsawPath.path()) {
            checkFromHere = true;
            continue;
        }
        if (checkFromHere && jigsawPath.path().intersected(collisionPath.path()).elementCount() > 0) return true;
        }
        return false;
}

QRect JigsawPath::recommendedInnerBounds(const QRect &outerBounds, Jigsaw::TypeOfPiece typeOfPiece, int minForcedPaths)
{
    if (minForcedPaths > 4 || minForcedPaths <= 0) minForcedPaths = 2;
    double sizePercentage;
    switch (typeOfPiece) {
    case Jigsaw::TypeOfPiece::TRAPEZOID:
        switch (minForcedPaths) {
        case 1:
            sizePercentage = 0.85;
            break;
        case 2:
            sizePercentage = 0.85;
            break;
        case 3:
            sizePercentage = 0.85;
            break;
        case 4:
            sizePercentage = 0.85;
            break;
        }
        break;
    case Jigsaw::TypeOfPiece::SIMPLEARC:
        switch (minForcedPaths) {
        case 1:
            sizePercentage = 0.85;
            break;
        case 2:
            sizePercentage = 0.84;
            break;
        case 3:
            sizePercentage = 0.82;
            break;
        case 4:
            sizePercentage = 0.79;
            break;
        }
        break;
    case Jigsaw::TypeOfPiece::TRIANGLECONNECTIONS:
        switch (minForcedPaths) {
        case 1:
            sizePercentage = 0.65;
            break;
        case 2:
            sizePercentage = 0.6;
            break;
        case 3:
            sizePercentage = 0.55;
            break;
        case 4:
            sizePercentage = 0.52;
            break;
        }
        break;
    case Jigsaw::TypeOfPiece::SIMPLECIRCLECONNECTIONS:
        switch (minForcedPaths) {
        case 1:
            sizePercentage = 0.79;
            break;
        case 2:
            sizePercentage = 0.76;
            break;
        case 3:
            sizePercentage = 0.74;
            break;
        case 4:
            sizePercentage = 0.72;
            break;
        }
        break;
    case Jigsaw::TypeOfPiece::STANDARD:
        switch (minForcedPaths) {
        case 1:
            sizePercentage = 0.63;
            break;
        case 2:
            sizePercentage = 0.59;
            break;
        case 3:
            sizePercentage = 0.56;
            break;
        case 4:
            sizePercentage = 0.51;
            break;
        }
        break;
    case Jigsaw::TypeOfPiece::STANDARDFUNNY:
        switch (minForcedPaths) {
        case 1:
            sizePercentage = 0.51;
            break;
        case 2:
            sizePercentage = 0.46;
            break;
        case 3:
            sizePercentage = 0.4;
            break;
        case 4:
            sizePercentage = 0.35;
            break;
        }
        break;
    default:
        sizePercentage = 0.85;
        break;
    }

    double width = sizePercentage * outerBounds.width();
    double height = sizePercentage * outerBounds.height();
    if (width > height) width = height;
    if (height > width) height = width;

    QPoint innerBoundsTopLeft = outerBounds.topLeft() + QPoint((outerBounds.width() - round(width)) / 2, (outerBounds.height() - round(height)) / 2);
    QSize innerBoundsSize(width, height);
    QRect innerBounds(innerBoundsTopLeft, innerBoundsSize);
    return innerBounds;

//    Test Result 2023-05-09 with QSize(1000, 1000):
//    Type of Piece: TRAPEZOID
//        Minimum Forced Paths: 1 Recommended size percentage: 0.98
//        Minimum Forced Paths: 2 Recommended size percentage: 0.98
//        Minimum Forced Paths: 3 Recommended size percentage: 0.98
//        Minimum Forced Paths: 4 Recommended size percentage: 0.98
//    Type of Piece: SIMPLEARC
//        Minimum Forced Paths: 1 Recommended size percentage: 0.85
//        Minimum Forced Paths: 2 Recommended size percentage: 0.84
//        Minimum Forced Paths: 3 Recommended size percentage: 0.82
//        Minimum Forced Paths: 4 Recommended size percentage: 0.79
//    Type of Piece: TRIANGLECONNECTIONS
//        Minimum Forced Paths: 1 Recommended size percentage: 0.65
//        Minimum Forced Paths: 2 Recommended size percentage: 0.6
//        Minimum Forced Paths: 3 Recommended size percentage: 0.55
//        Minimum Forced Paths: 4 Recommended size percentage: 0.52
//    Type of Piece: SIMPLECIRCLECONNECTIONS
//        Minimum Forced Paths: 1 Recommended size percentage: 0.79
//        Minimum Forced Paths: 2 Recommended size percentage: 0.76
//        Minimum Forced Paths: 3 Recommended size percentage: 0.74
//        Minimum Forced Paths: 4 Recommended size percentage: 0.72
//    Type of Piece: STANDARD
//        Minimum Forced Paths: 1 Recommended size percentage: 0.63
//        Minimum Forced Paths: 2 Recommended size percentage: 0.59
//        Minimum Forced Paths: 3 Recommended size percentage: 0.56
//        Minimum Forced Paths: 4 Recommended size percentage: 0.51
//    Type of Piece: STANDARDFUNNY
//        Minimum Forced Paths: 1 Recommended size percentage: 0.51
//        Minimum Forced Paths: 2 Recommended size percentage: 0.46
//        Minimum Forced Paths: 3 Recommended size percentage: 0.4
//        Minimum Forced Paths: 4 Recommended size percentage: 0.35
}

QVector<double> JigsawPath::calculateRecommendedInnerBoundsPercentage(QVector<Jigsaw::TypeOfPiece> types, QSize size, int startPercentage, int startPaths, const CustomJigsawPath &customPath)
{
    bool liveDebug = false;

    QVector<double> testResult(0, 0.0);
    int cycles = 10;
    int maxAttempts = 30;
    int minSuccesfullPercentages = 3;
    int percentageForNextMinForcedPathsCycle = startPercentage;

    QPoint outerBoundsTopLeft(0, 0);
    QRect outerBounds(outerBoundsTopLeft, size);

    for (const auto typeOfPiece : types) {

        //testResult+= "Type of Piece: " + QString::number(typeOfPieceToInt(typeOfPiece)) + " ";
        percentageForNextMinForcedPathsCycle = startPercentage;

        for (int minForcedPaths = startPaths; minForcedPaths <= 4; ++minForcedPaths) {

            //testResult+= "Minimum Forced Paths: " + QString::number(minForcedPaths) + " ";

            int successfulPercentages = 0;

            for (int i = percentageForNextMinForcedPathsCycle; i > 0; --i) {

                if (successfulPercentages >= minSuccesfullPercentages) {
                    //testResult+= "Recommended size percentage: " + QString::number(0.01 * (i + 1)) + " ";
                    testResult.push_back(0.01 * (i + 1));
                    percentageForNextMinForcedPathsCycle = i + minSuccesfullPercentages;
                    if (liveDebug) {
                        qDebug() << "Percentage calculated for type of Piece" << typeOfPieceToInt(typeOfPiece) << "with" <<  minForcedPaths << "forced Paths:" << 0.01 * (i + 1);
                    }
                    break;
                }

                int successfulAttempts = 0;
                double sizePercentage = 0.01 * i;

                QPoint innerBoundsTopLeft = outerBoundsTopLeft + QPoint((1.0 - sizePercentage) / 2 * outerBounds.width(), (1.0 - sizePercentage) / 2 * outerBounds.height());
                QSize innerBoundsSize(sizePercentage * outerBounds.width(), sizePercentage * outerBounds.height());
                QRect innerBounds(innerBoundsTopLeft, innerBoundsSize);

                for (int c = 0; c < cycles; ++c) {

                    QPoint topLeft, topRight, bottomRight, bottomLeft;
                    QRect topBounds, rightBounds, bottomBounds, leftBounds;
                    QVector<JigsawPath> jigsawPaths(4);
                    int collisions = 0;
                    int unsuccessful = 0;
                    int emergencyCounter = 0;
                    int maxUnsuccessfulPaths = 4 - minForcedPaths;

                    do {
                        collisions = 0;
                        unsuccessful = 0;
                        topLeft = QPoint(Jigsaw::randomNumber(outerBounds.left(), innerBounds.left()), Jigsaw::randomNumber(outerBounds.top(), innerBounds.top()));
                        topRight = QPoint(Jigsaw::randomNumber(innerBounds.right(), outerBounds.right()), Jigsaw::randomNumber(outerBounds.top(), innerBounds.top()));
                        bottomRight = QPoint(Jigsaw::randomNumber(innerBounds.right(), outerBounds.right()), Jigsaw::randomNumber(innerBounds.bottom(), outerBounds.bottom()));
                        bottomLeft = QPoint(Jigsaw::randomNumber(outerBounds.left(), innerBounds.left()), Jigsaw::randomNumber(innerBounds.bottom(), outerBounds.bottom()));

                        topBounds = QRect(outerBounds.topLeft(), QPoint(outerBounds.right(), innerBounds.top()));
                        rightBounds = QRect(QPoint(innerBounds.right(), outerBounds.top()), outerBounds.bottomRight());
                        bottomBounds = QRect(QPoint(outerBounds.left(), innerBounds.bottom()), outerBounds.bottomRight());
                        leftBounds = QRect(outerBounds.topLeft(), QPoint(innerBounds.left(), outerBounds.bottom()));

                        jigsawPaths[0] = JigsawPath(topLeft, topRight, topBounds, typeOfPiece, customPath);
                        jigsawPaths[1] = JigsawPath(topRight, bottomRight, rightBounds, typeOfPiece, customPath);
                        jigsawPaths[2] = JigsawPath(bottomRight, bottomLeft, bottomBounds, typeOfPiece, customPath);
                        jigsawPaths[3] = JigsawPath(bottomLeft, topLeft, leftBounds, typeOfPiece, customPath);

                        for (const auto &jigsawPath : jigsawPaths) {
                            collisions += pathHasCollisions(jigsawPath, jigsawPaths) ? 1 : 0;
                            unsuccessful += jigsawPath.creatingPathSuccessful() ? 0 : 1;
                        }

                        ++emergencyCounter;
                    }
                    while ((unsuccessful > maxUnsuccessfulPaths || collisions > 0) && emergencyCounter < maxAttempts);
                    if (emergencyCounter < maxAttempts) ++successfulAttempts;
                }
                if (successfulAttempts >= cycles - 1) ++successfulPercentages;
            }
        }
    }
    if (testResult.isEmpty()) testResult.push_back(0.0);
    return testResult;
}
