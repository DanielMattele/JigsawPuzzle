#ifndef CUSTOMJIGSAWPATH_H
#define CUSTOMJIGSAWPATH_H

#include <QPainterPath>
#include <QPoint>

class CustomJigsawPath
{
public:
    enum class TypeOfLine{STRAIGHT, CURVE};

    enum class TypeOfRandomOffset{NONE, CIRCULAR, LINE};
    struct RandomOffset{
        RandomOffset() {
            type = TypeOfRandomOffset::NONE;
            value = 0.0;
            lineP1 = nullptr;
            lineP2 = nullptr;
        };
        RandomOffset(TypeOfRandomOffset type, double value = 0.0, QPointF* lineP1 = nullptr, QPointF* lineP2 = nullptr) {
            this->type = type;
            this->value = value;
            this->lineP1 = lineP1;
            this->lineP2 = lineP2;
        };
        ~RandomOffset() {};
        TypeOfRandomOffset type;
        double value; //as percentage of the path's distance from start to end
        QPointF* lineP1;
        QPointF* lineP2;
    };

    enum class TypeOfRestriction{NONE, FIXEDPOSITION, LINE, INTERSECTIONPOINT};
    struct Restriction{
        Restriction() {
            type = TypeOfRestriction::NONE;
            line1P1 = nullptr;
            line1P2 = nullptr;
            line2P1 = nullptr;
            line2P2 = nullptr;
            distanceFromLine = 0.0;
        };
        Restriction(TypeOfRestriction type) {
            this->type = type;
            line1P1 = nullptr;
            line1P2 = nullptr;
            line2P1 = nullptr;
            line2P2 = nullptr;
            distanceFromLine = 0.0;
        };
        Restriction(TypeOfRestriction type, QPointF* line1P1, QPointF* line1P2, double distanceFromLine = 0.0) {
            this->type = type;
            this->line1P1 = line1P1;
            this->line1P2 = line1P2;
            line2P1 = nullptr;
            line2P2 = nullptr;
            this->distanceFromLine = distanceFromLine;
        };
        Restriction(TypeOfRestriction type, QPointF* line1P1, QPointF* line1P2, QPointF* line2P1, QPointF* line2P2) {
            this->type = type;
            this->line1P1 = line1P1;
            this->line1P2 = line1P2;
            this->line2P1 = line2P1;
            this->line2P2 = line2P2;
            distanceFromLine = 0.0;
        };
        ~Restriction() {};
        TypeOfRestriction type;
        QPointF* line1P1;
        QPointF* line1P2;
        QPointF* line2P1;
        QPointF* line2P2;
        double distanceFromLine;
    };

    struct PathPoint {
        PathPoint() {
            position = QPointF(0.0, 0.0);
            randomOffset = RandomOffset();
            restriction = Restriction(TypeOfRestriction::NONE);
            positionControlPoint = QPoint(0.0, 0.0);
            randomOffsetControlPoint = RandomOffset();
            restrictionControlPoint = Restriction(TypeOfRestriction::NONE);
            typeOfLine = TypeOfLine::STRAIGHT;
        }
        PathPoint(QPointF position, RandomOffset randomOffset, Restriction restriction) {
            this->position = position;
            this->randomOffset = randomOffset;
            this->restriction = restriction;
            positionControlPoint = QPointF(0.0, 0.0);
            randomOffsetControlPoint = RandomOffset();
            restrictionControlPoint = Restriction(TypeOfRestriction::NONE);
            typeOfLine = TypeOfLine::STRAIGHT;
        }
        PathPoint(QPointF position, RandomOffset randomOffset, Restriction restriction,
                  QPointF positionControlPoint, RandomOffset randomOffsetControlPoint, Restriction restrictionControlPoint,
                  TypeOfLine typeOfLine = TypeOfLine::CURVE) {
            this->position = position;
            this->randomOffset = randomOffset;
            this->restriction = restriction;
            this->positionControlPoint = positionControlPoint;
            this->randomOffsetControlPoint = randomOffsetControlPoint;
            this->restrictionControlPoint = restrictionControlPoint;
            this->typeOfLine = typeOfLine;
        }
        ~PathPoint() {}
        QPointF position;
        RandomOffset randomOffset;
        Restriction restriction;
        QPointF positionControlPoint;
        RandomOffset randomOffsetControlPoint;
        Restriction restrictionControlPoint;
        TypeOfLine typeOfLine;
    };

    CustomJigsawPath();
    CustomJigsawPath(const QVector<PathPoint> &pathPoints);
    ~CustomJigsawPath();

    void insertNewPoint(const PathPoint &pathPoint, int index = -1);
    void removePoint(int index = -1);

    void setPositionPathPoint(int index, const QPointF &pathPoint);
    void setPositionControlPoint(int index, const QPointF &controlPoint);
    void setTypeOfLine(int index, TypeOfLine typeOfLine);

    void setRestrictionPathPoint(int index, TypeOfRestriction type, QPointF* line1P1 = nullptr, QPointF* line1P2 = nullptr,
                                 QPointF* line2P1 = nullptr, QPointF* line2P2 = nullptr, double distanceFromLine = 0.0);
    void setRestrictionControlPoint(int index, TypeOfRestriction type, QPointF* line1P1 = nullptr, QPointF* line1P2 = nullptr,
                                    QPointF* line2P1 = nullptr, QPointF* line2P2 = nullptr, double distanceFromLine = 0.0);

    void setRandomOffsetPathPoint(int index, TypeOfRandomOffset type, double value, QPointF* lineP1 = nullptr, QPointF* lineP2 = nullptr);
    void setRandomOffsetControlPoint(int index, TypeOfRandomOffset type, double value, QPointF* lineP1 = nullptr, QPointF* lineP2 = nullptr);

    CustomJigsawPath::PathPoint pathPoint(int index) const;
    QPointF* positionPathPoint(int index);
    QPointF* positionControlPoint(int index);
    int numberOfPoints() const;

    static void changeDistanceFromLine(QPointF &pointToChange, const QLineF &line, double newDistance);

private:
    QVector<CustomJigsawPath::PathPoint> m_pathPoints;
};

#endif // CUSTOMJIGSAWPATH_H
