#include "customjigsawpath.h"
#include "qdebug.h"
#include <random>

CustomJigsawPath::CustomJigsawPath()
    : m_pathPoints(QVector<CustomJigsawPath::PathPoint>(0))
{
    m_pathPoints.push_back(CustomJigsawPath::PathPoint(QPointF(0.0, 0.0), RandomOffset(TypeOfRandomOffset::NONE, 0.0), Restriction(TypeOfRestriction::FIXEDPOSITION)));
    m_pathPoints.push_back(CustomJigsawPath::PathPoint(QPoint(0.0, 100.0), RandomOffset(TypeOfRandomOffset::NONE, 0.0), Restriction(TypeOfRestriction::FIXEDPOSITION)));
}

CustomJigsawPath::CustomJigsawPath(const QVector<PathPoint> &pathPoints)
    : m_pathPoints(pathPoints)
{
}

CustomJigsawPath::~CustomJigsawPath()
{

}

void CustomJigsawPath::insertNewPoint(const PathPoint &pathPoint, int index)
{
    if (index < 0 || index >= numberOfPoints()) m_pathPoints.push_back(pathPoint);
    else m_pathPoints.insert(index, pathPoint);
}

void CustomJigsawPath::removePoint(int index)
{
    if (index >= m_pathPoints.size() || index < -1 || m_pathPoints.size() == 0) {
        return;
    }
    else if (index == -1) {
        m_pathPoints.pop_back();
    }
    else {
        m_pathPoints.removeAt(index);
    }
}

void CustomJigsawPath::setPositionPathPoint(int index, const QPointF &pathPoint)
{
    if (index < 0 || index >= m_pathPoints.size() || QLineF(pathPoint, m_pathPoints[index].position).length() < 1) return;

    switch (m_pathPoints[index].restriction.type) {
    case CustomJigsawPath::TypeOfRestriction::NONE: {
        m_pathPoints[index].position = pathPoint;
        break;
    }
    case CustomJigsawPath::TypeOfRestriction::FIXEDPOSITION: {
        break;
    }
    case CustomJigsawPath::TypeOfRestriction::LINE: {
        if (m_pathPoints[index].restriction.line1P1 == nullptr || m_pathPoints[index].restriction.line1P2 == nullptr) return;
        m_pathPoints[index].position = pathPoint;
        changeDistanceFromLine(m_pathPoints[index].position,
                               QLineF(*m_pathPoints[index].restriction.line1P1, *m_pathPoints[index].restriction.line1P2),
                               m_pathPoints[index].restriction.distanceFromLine);
        break;
    }
    case CustomJigsawPath::TypeOfRestriction::INTERSECTIONPOINT: {
        if (m_pathPoints[index].restriction.line1P1 == nullptr || m_pathPoints[index].restriction.line1P2 == nullptr ||
            m_pathPoints[index].restriction.line2P1 == nullptr || m_pathPoints[index].restriction.line2P2 == nullptr) return;
        m_pathPoints[index].randomOffset.type = TypeOfRandomOffset::NONE;
        QPointF intersectionPoint(0.0, 0.0);
        QLineF::IntersectionType intersectionType = QLineF(*m_pathPoints[index].restriction.line1P1, *m_pathPoints[index].restriction.line1P2).intersects(
                                                    QLineF(*m_pathPoints[index].restriction.line2P1, *m_pathPoints[index].restriction.line2P2), &intersectionPoint);
        if (intersectionType != QLineF::NoIntersection) m_pathPoints[index].position = intersectionPoint;
        break;
    }
    }
}

void CustomJigsawPath::setPositionControlPoint(int index, const QPointF &controlPoint)
{
    if (index <= 0 || index >= m_pathPoints.size() || QLineF(controlPoint, m_pathPoints[index].positionControlPoint).length() < 1) return;

    switch (m_pathPoints[index].restrictionControlPoint.type) {
    case CustomJigsawPath::TypeOfRestriction::NONE: {
        m_pathPoints[index].positionControlPoint = controlPoint;
        break;
    }
    case CustomJigsawPath::TypeOfRestriction::FIXEDPOSITION: {
        break;
    }
    case CustomJigsawPath::TypeOfRestriction::LINE: {
        m_pathPoints[index].positionControlPoint = controlPoint;
        changeDistanceFromLine(m_pathPoints[index].positionControlPoint,
                               QLineF(*m_pathPoints[index].restrictionControlPoint.line1P1, *m_pathPoints[index].restrictionControlPoint.line1P2),
                               m_pathPoints[index].restrictionControlPoint.distanceFromLine);
        break;
    }
    case CustomJigsawPath::TypeOfRestriction::INTERSECTIONPOINT: {
        if (m_pathPoints[index].restrictionControlPoint.line1P1 == nullptr || m_pathPoints[index].restrictionControlPoint.line1P2 == nullptr ||
            m_pathPoints[index].restrictionControlPoint.line2P1 == nullptr || m_pathPoints[index].restrictionControlPoint.line2P2 == nullptr) return;
        m_pathPoints[index].randomOffsetControlPoint.type = TypeOfRandomOffset::NONE;
        QPointF intersectionPoint(0.0, 0.0);
        QLineF::IntersectionType intersectionType = QLineF(*m_pathPoints[index].restrictionControlPoint.line1P1, *m_pathPoints[index].restrictionControlPoint.line1P2).intersects(
                                                    QLineF(*m_pathPoints[index].restrictionControlPoint.line2P1, *m_pathPoints[index].restrictionControlPoint.line2P2), &intersectionPoint);
        if (intersectionType != QLineF::NoIntersection) m_pathPoints[index].positionControlPoint = intersectionPoint;
        break;
    }
    }
}

void CustomJigsawPath::setTypeOfLine(int index, TypeOfLine typeOfLine)
{
    if (index <= 0 || index >= m_pathPoints.size()) return;

    m_pathPoints[index].typeOfLine = typeOfLine;
}

void CustomJigsawPath::setRestrictionPathPoint(int index, TypeOfRestriction type, QPointF *line1P1, QPointF *line1P2, QPointF *line2P1, QPointF *line2P2, double distanceFromLine)
{
    if (index < 0 || index >= m_pathPoints.size()) return;

    switch(type) {
    case CustomJigsawPath::TypeOfRestriction::NONE: {
        if (m_pathPoints[index].randomOffset.type == TypeOfRandomOffset::LINE) m_pathPoints[index].randomOffset.type = TypeOfRandomOffset::CIRCULAR;
        break;
    }
    case CustomJigsawPath::TypeOfRestriction::FIXEDPOSITION: {
        m_pathPoints[index].randomOffset.type = TypeOfRandomOffset::NONE;
        break;
    }
    case CustomJigsawPath::TypeOfRestriction::LINE: {
        if (line1P1 == nullptr || line1P2 == nullptr) return;
        m_pathPoints[index].randomOffset.type = TypeOfRandomOffset::LINE;
        m_pathPoints[index].randomOffset.lineP1 = line1P1;
        m_pathPoints[index].randomOffset.lineP2 = line1P2;
        double newDistance = QLineF(m_pathPoints.last().position, m_pathPoints[0].position).length() * distanceFromLine / 100;
        changeDistanceFromLine(m_pathPoints[index].position, QLineF(*line1P1, *line1P2), newDistance);
        break;
    }
    case CustomJigsawPath::TypeOfRestriction::INTERSECTIONPOINT: {
        if (line1P1 == nullptr || line1P2 == nullptr || line2P1 == nullptr || line2P2 == nullptr) return;
        m_pathPoints[index].randomOffset.type = TypeOfRandomOffset::NONE;
        QPointF intersectionPoint(0.0, 0.0);
        QLineF::IntersectionType intersectionType = QLineF(*line1P1, *line1P2).intersects(QLineF(*line2P1, *line2P2), &intersectionPoint);
        if (intersectionType != QLineF::NoIntersection) m_pathPoints[index].position = intersectionPoint;
        else return;
        break;
    }
    }
    m_pathPoints[index].restriction.type = type;
    m_pathPoints[index].restriction.line1P1 = line1P1;
    m_pathPoints[index].restriction.line1P2 = line1P2;
    m_pathPoints[index].restriction.line2P1 = line2P1;
    m_pathPoints[index].restriction.line2P2 = line2P2;
    m_pathPoints[index].restriction.distanceFromLine = distanceFromLine;
}

void CustomJigsawPath::setRestrictionControlPoint(int index, TypeOfRestriction type, QPointF *line1P1, QPointF *line1P2, QPointF *line2P1, QPointF *line2P2, double distanceFromLine)
{
    if (index <= 0 || index >= m_pathPoints.size()) return;

    switch(type) {
    case CustomJigsawPath::TypeOfRestriction::NONE: {
        if (m_pathPoints[index].randomOffsetControlPoint.type == TypeOfRandomOffset::LINE) m_pathPoints[index].randomOffsetControlPoint.type = TypeOfRandomOffset::CIRCULAR;
        break;
    }
    case CustomJigsawPath::TypeOfRestriction::FIXEDPOSITION: {
        m_pathPoints[index].randomOffsetControlPoint.type = TypeOfRandomOffset::NONE;
        break;
    }
    case CustomJigsawPath::TypeOfRestriction::LINE: {
        if (line1P1 == nullptr || line1P2 == nullptr) return;
        m_pathPoints[index].randomOffsetControlPoint.type = TypeOfRandomOffset::LINE;
        m_pathPoints[index].randomOffsetControlPoint.lineP1 = line1P1;
        m_pathPoints[index].randomOffsetControlPoint.lineP2 = line1P2;
        double newDistance = QLineF(m_pathPoints.last().position, m_pathPoints[0].position).length() * distanceFromLine / 100;
        changeDistanceFromLine(m_pathPoints[index].positionControlPoint, QLineF(*line1P1, *line1P2), newDistance);
        break;
    }
    case CustomJigsawPath::TypeOfRestriction::INTERSECTIONPOINT: {
        if (line1P1 == nullptr || line1P2 == nullptr || line2P1 == nullptr || line2P2 == nullptr) return;
        m_pathPoints[index].randomOffsetControlPoint.type = TypeOfRandomOffset::NONE;
        QPointF intersectionPoint(0.0, 0.0);
        QLineF::IntersectionType intersectionType = QLineF(*line1P1, *line1P2).intersects(QLineF(*line2P1, *line2P2), &intersectionPoint);
        if (intersectionType != QLineF::NoIntersection) m_pathPoints[index].positionControlPoint = intersectionPoint;
        else return;
        break;
    }
    }
    m_pathPoints[index].restrictionControlPoint.type = type;
    m_pathPoints[index].restrictionControlPoint.line1P1 = line1P1;
    m_pathPoints[index].restrictionControlPoint.line1P2 = line1P2;
    m_pathPoints[index].restrictionControlPoint.line2P1 = line2P1;
    m_pathPoints[index].restrictionControlPoint.line2P2 = line2P2;
    m_pathPoints[index].restrictionControlPoint.distanceFromLine = distanceFromLine;
}

void CustomJigsawPath::setRandomOffsetPathPoint(int index, TypeOfRandomOffset type, double value, QPointF *lineP1, QPointF *lineP2)
{
    if (index < 0 || index >= m_pathPoints.size()) return;

    switch (type) {
    case CustomJigsawPath::TypeOfRandomOffset::NONE: {
        break;
    }
    case CustomJigsawPath::TypeOfRandomOffset::CIRCULAR: {
        m_pathPoints[index].restriction.type = TypeOfRestriction::NONE;
        break;
    }
    case CustomJigsawPath::TypeOfRandomOffset::LINE: {
        if (lineP1 == nullptr || lineP2 == nullptr) return;
        m_pathPoints[index].restriction.type = TypeOfRestriction::LINE;
        m_pathPoints[index].restriction.line1P1 = lineP1;
        m_pathPoints[index].restriction.line1P2 = lineP2;
        double newDistance = QLineF(m_pathPoints.last().position, m_pathPoints[0].position).length() * m_pathPoints[index].restriction.distanceFromLine / 100;
        changeDistanceFromLine(m_pathPoints[index].position, QLineF(*lineP1, *lineP2), newDistance);
        break;
    }
    }
    m_pathPoints[index].randomOffset.type = type;
    m_pathPoints[index].randomOffset.value = value;
    m_pathPoints[index].randomOffset.lineP1 = lineP1;
    m_pathPoints[index].randomOffset.lineP2 = lineP2;
}

void CustomJigsawPath::setRandomOffsetControlPoint(int index, TypeOfRandomOffset type, double value, QPointF *lineP1, QPointF *lineP2)
{
    if (index <= 0 || index >= m_pathPoints.size()) return;

    switch (type) {
    case CustomJigsawPath::TypeOfRandomOffset::NONE: {
        break;
    }
    case CustomJigsawPath::TypeOfRandomOffset::CIRCULAR: {
        m_pathPoints[index].restrictionControlPoint.type = TypeOfRestriction::NONE;
        break;
    }
    case CustomJigsawPath::TypeOfRandomOffset::LINE: {
        if (lineP1 == nullptr || lineP2 == nullptr) return;
        m_pathPoints[index].restrictionControlPoint.type = TypeOfRestriction::LINE;
        m_pathPoints[index].restrictionControlPoint.line1P1 = lineP1;
        m_pathPoints[index].restrictionControlPoint.line1P2 = lineP2;
        double newDistance = QLineF(m_pathPoints.last().position, m_pathPoints[0].position).length() * m_pathPoints[index].restrictionControlPoint.distanceFromLine / 100;
        changeDistanceFromLine(m_pathPoints[index].positionControlPoint, QLineF(*lineP1, *lineP2), newDistance);
        break;
    }
    }
    m_pathPoints[index].randomOffsetControlPoint.type = type;
    m_pathPoints[index].randomOffsetControlPoint.value = value;
    m_pathPoints[index].randomOffsetControlPoint.lineP1 = lineP1;
    m_pathPoints[index].randomOffsetControlPoint.lineP2 = lineP2;
}

CustomJigsawPath::PathPoint CustomJigsawPath::pathPoint(int index) const
{
    if (index < 0 || index >= m_pathPoints.size()) return PathPoint();

    return m_pathPoints[index];
}

QPointF *CustomJigsawPath::positionPathPoint(int index)
{
    if (index < 0 || index >= m_pathPoints.size()) return nullptr;

    return &m_pathPoints[index].position;
}

QPointF *CustomJigsawPath::positionControlPoint(int index)
{
    if (index <= 0 || index >= m_pathPoints.size()) return nullptr;

    return &m_pathPoints[index].positionControlPoint;
}

int CustomJigsawPath::numberOfPoints() const
{
    return m_pathPoints.size();
}

CustomJigsawPath CustomJigsawPath::getRandomizedPath() const
{
    CustomJigsawPath randomizedPath = *this;
    double lengthStartToEnd = QLineF(randomizedPath.pathPoint(0).position, randomizedPath.pathPoint(randomizedPath.numberOfPoints() - 1).position).length();

    //Solve circular offset

    for (int i = 0; i < randomizedPath.numberOfPoints(); ++i) {
        if (randomizedPath.pathPoint(i).randomOffset.type == TypeOfRandomOffset::CIRCULAR) {
            QLineF pathPointToRandomOffset;
            pathPointToRandomOffset.setP1(randomizedPath.pathPoint(i).position);
            pathPointToRandomOffset.setAngle(Jigsaw::randomNumber(0, 359));
            pathPointToRandomOffset.setLength(Jigsaw::randomNumber(0, randomizedPath.pathPoint(i).randomOffset.value) * lengthStartToEnd / 100);
            randomizedPath.setRandomOffsetPathPoint(i, TypeOfRandomOffset::NONE, 0.0);
            randomizedPath.setPositionPathPoint(i, pathPointToRandomOffset.p2());
            for (int j = 0; j < randomizedPath.numberOfPoints(); ++j) {
                randomizedPath.setPositionPathPoint(j, randomizedPath.pathPoint(j).position);
                if (j == 0) continue;
                randomizedPath.setPositionControlPoint(j, randomizedPath.pathPoint(j).positionControlPoint);
            }
        }
        if (i == 0) continue;
        if (randomizedPath.pathPoint(i).randomOffsetControlPoint.type == TypeOfRandomOffset::CIRCULAR) {
            QLineF pathControlPointToRandomOffset;
            pathControlPointToRandomOffset.setP1(randomizedPath.pathPoint(i).positionControlPoint);
            pathControlPointToRandomOffset.setAngle(Jigsaw::randomNumber(0, 359));
            pathControlPointToRandomOffset.setLength(Jigsaw::randomNumber(0, randomizedPath.pathPoint(i).randomOffsetControlPoint.value) * lengthStartToEnd / 100);
            randomizedPath.setRandomOffsetControlPoint(i, TypeOfRandomOffset::NONE, 0.0);
            randomizedPath.setPositionControlPoint(i, pathControlPointToRandomOffset.p2());
            for (int j = 0; j < randomizedPath.numberOfPoints(); ++j) {
                randomizedPath.setPositionPathPoint(j, randomizedPath.pathPoint(j).position);
                if (j == 0) continue;
                randomizedPath.setPositionControlPoint(j, randomizedPath.pathPoint(j).positionControlPoint);
            }
        }
    }

    //Solve line offset

    for (int i = 0; i < randomizedPath.numberOfPoints(); ++i) {
        if (randomizedPath.pathPoint(i).randomOffset.type == TypeOfRandomOffset::LINE) {
            QLineF pathPointToRandomOffset;
            pathPointToRandomOffset.setP1(randomizedPath.pathPoint(i).position);
            pathPointToRandomOffset.setAngle(QLineF(*randomizedPath.pathPoint(i).randomOffset.lineP1, *randomizedPath.pathPoint(i).randomOffset.lineP2).angle());
            pathPointToRandomOffset.setAngle(Jigsaw::randomNumber(0, 1) == 0 ? pathPointToRandomOffset.angle() : pathPointToRandomOffset.angle() - 180);
            pathPointToRandomOffset.setLength(Jigsaw::randomNumber(0, randomizedPath.pathPoint(i).randomOffset.value) * lengthStartToEnd / 100);
            randomizedPath.setRandomOffsetPathPoint(i, TypeOfRandomOffset::NONE, 0.0);
            randomizedPath.setPositionPathPoint(i, pathPointToRandomOffset.p2());
            for (int i = 0; i < randomizedPath.numberOfPoints(); ++i) {
                randomizedPath.setPositionPathPoint(i, randomizedPath.pathPoint(i).position);
                if (i == 0) continue;
                randomizedPath.setPositionControlPoint(i, randomizedPath.pathPoint(i).positionControlPoint);
            }
        }
        if (i == 0) continue;
        if (randomizedPath.pathPoint(i).randomOffsetControlPoint.type == TypeOfRandomOffset::LINE) {
            QLineF pathControlPointToRandomOffset;
            pathControlPointToRandomOffset.setP1(randomizedPath.pathPoint(i).positionControlPoint);
            pathControlPointToRandomOffset.setAngle(QLineF(*randomizedPath.pathPoint(i).randomOffsetControlPoint.lineP1, *randomizedPath.pathPoint(i).randomOffsetControlPoint.lineP2).angle());
            pathControlPointToRandomOffset.setAngle(Jigsaw::randomNumber(0, 1) == 0 ? pathControlPointToRandomOffset.angle() : pathControlPointToRandomOffset.angle() - 180);
            pathControlPointToRandomOffset.setLength(Jigsaw::randomNumber(0, randomizedPath.pathPoint(i).randomOffsetControlPoint.value) * lengthStartToEnd / 100);
            randomizedPath.setRandomOffsetControlPoint(i, TypeOfRandomOffset::NONE, 0.0);
            randomizedPath.setPositionControlPoint(i, pathControlPointToRandomOffset.p2());
            for (int i = 0; i < randomizedPath.numberOfPoints(); ++i) {
                randomizedPath.setPositionPathPoint(i, randomizedPath.pathPoint(i).position);
                if (i == 0) continue;
                randomizedPath.setPositionControlPoint(i, randomizedPath.pathPoint(i).positionControlPoint);
            }
        }
    }

    return randomizedPath;
}

void CustomJigsawPath::changeDistanceFromLine(QPointF &pointToChange, const QLineF &line, double newDistance)
{
    QLineF lineNormalVector = line.normalVector();
    lineNormalVector.translate(pointToChange - lineNormalVector.p1());
    QPointF intersectionPoint(0.0, 0.0);
    if (line.intersects(lineNormalVector, &intersectionPoint) == QLineF::NoIntersection) return;
    lineNormalVector.translate(intersectionPoint - lineNormalVector.p1());
    lineNormalVector.setLength(newDistance);
    pointToChange = lineNormalVector.p2();
}
