#include "customjigsawpathcreator.h"

CustomJigsawPathCreator::CustomJigsawPathCreator(QWidget *parent)
    : QWidget{parent}
    , m_selectedPathPoint(-1)
    , m_selectedControlPoint(-1)
    , m_mainLayout(new QHBoxLayout(this))
    , m_leftSideWidget(new QWidget(this))
    , m_leftSideLayout(new QVBoxLayout(m_leftSideWidget))
    , m_canvas(new CreatorCanvas(m_leftSideWidget))
    , m_menuButtonsWidget1(new QWidget(m_leftSideWidget))
    , m_menuButtonsLayout1(new QHBoxLayout(m_menuButtonsWidget1))
    , m_menuButtonInsert(new QPushButton("Insert before", m_menuButtonsWidget1))
    , m_menuComboBoxInsertBefore(new QComboBox(m_menuButtonsWidget1))
    , m_menuButtonRemove(new QPushButton("Remove", m_menuButtonsWidget1))
    , m_menuComboBoxRemoveAt(new QComboBox(m_menuButtonsWidget1))
    , m_testWidget(new QWidget(m_leftSideWidget))
    , m_testLayout(new QGridLayout(m_testWidget))
    , m_testLabel1(new QLabel(m_testWidget))
    , m_testLabel2(new QLabel(m_testWidget))
    , m_testLabel3(new QLabel(m_testWidget))
    , m_testButton(new QPushButton("Test!", m_testWidget))
    , m_testCheckBox(new QCheckBox("Calculate quality (Slow!)", m_testWidget))
    , m_testResultLabel(new QLabel(m_testWidget))
    , m_menuButtonsWidget2(new QWidget(m_leftSideWidget))
    , m_menuButtonsLayout2(new QHBoxLayout(m_menuButtonsWidget2))
    , m_menuButtonApply(new QPushButton("Apply", m_menuButtonsWidget2))
    , m_menuButtonOpen(new QPushButton("Open", m_menuButtonsWidget2))
    , m_menuButtonSave(new QPushButton("Save", m_menuButtonsWidget2))
    , m_rightSideWidget(new QWidget(this))
    , m_rightSideScrollArea(new QScrollArea(this))
    , m_rightSideLayout(new QVBoxLayout(m_rightSideWidget))
    , m_entriesWidget(QVector<QWidget*>(0, nullptr))
    , m_entriesLayout(QVector<QHBoxLayout*>(0, nullptr))
    , m_pointsLabel(QVector<QLabel*>(0, nullptr))
    , m_pointsXSpinBox(QVector<QDoubleSpinBox*>(0, nullptr))
    , m_pointsYSpinBox(QVector<QDoubleSpinBox*>(0, nullptr))
    , m_pointsRandomOffsetType(QVector<QComboBox*>(0, nullptr))
    , m_pointsRandomOffsetValue(QVector<QDoubleSpinBox*>(0, nullptr))
    , m_pointsRandomOffsetL1P1(QVector<QComboBox*>(0, nullptr))
    , m_pointsRandomOffsetL1P2(QVector<QComboBox*>(0, nullptr))
    , m_pointsRandomOffsetButton(QVector<QPushButton*>(0, nullptr))
    , m_pointsRestrictionLabel(QVector<QLabel*>(0, nullptr))
    , m_pointsRestrictionType(QVector<QComboBox*>(0, nullptr))
    , m_pointsRestrictionL1P1(QVector<QComboBox*>(0, nullptr))
    , m_pointsRestrictionL1P2(QVector<QComboBox*>(0, nullptr))
    , m_pointsRestrictionL2P1(QVector<QComboBox*>(0, nullptr))
    , m_pointsRestrictionL2P2(QVector<QComboBox*>(0, nullptr))
    , m_pointsRestrictionDistanceSpinBox(QVector<QDoubleSpinBox*>(0, nullptr))
    , m_pointsRestrictionButton(QVector<QPushButton*>(0, nullptr))
    , m_linesLabel(QVector<QLabel*>(0, nullptr))
    , m_linesType(QVector<QComboBox*>(0, nullptr))
    , m_controlPointsLabel(QVector<QLabel*>(0, nullptr))
    , m_controlPointsXSpinBox(QVector<QDoubleSpinBox*>(0, nullptr))
    , m_controlPointsYSpinBox(QVector<QDoubleSpinBox*>(0, nullptr))
    , m_controlPointsRandomOffsetType(QVector<QComboBox*>(0, nullptr))
    , m_controlPointsRandomOffsetValue(QVector<QDoubleSpinBox*>(0, nullptr))
    , m_controlPointsRandomOffsetL1P1(QVector<QComboBox*>(0, nullptr))
    , m_controlPointsRandomOffsetL1P2(QVector<QComboBox*>(0, nullptr))
    , m_controlPointsRandomOffsetButton(QVector<QPushButton*>(0, nullptr))
    , m_controlPointsRestrictionLabel(QVector<QLabel*>(0, nullptr))
    , m_controlPointsRestrictionType(QVector<QComboBox*>(0, nullptr))
    , m_controlPointsRestrictionL1P1(QVector<QComboBox*>(0, nullptr))
    , m_controlPointsRestrictionL1P2(QVector<QComboBox*>(0, nullptr))
    , m_controlPointsRestrictionL2P1(QVector<QComboBox*>(0, nullptr))
    , m_controlPointsRestrictionL2P2(QVector<QComboBox*>(0, nullptr))
    , m_controlPointsRestrictionDistanceSpinBox(QVector<QDoubleSpinBox*>(0, nullptr))
    , m_controlPointsRestrictionButton(QVector<QPushButton*>(0, nullptr))
{
    setMinimumWidth(1400);

    m_rightSideLayout->setSpacing(0);
    m_mainLayout->addWidget(m_leftSideWidget);
        m_leftSideLayout->addWidget(m_canvas);
        m_leftSideLayout->addWidget(m_menuButtonsWidget1);
            m_menuButtonsLayout1->addWidget(m_menuButtonInsert);
            m_menuButtonsLayout1->addWidget(m_menuComboBoxInsertBefore);
            m_menuButtonsLayout1->addSpacing(20);
            m_menuButtonsLayout1->addWidget(m_menuButtonRemove);
            m_menuButtonsLayout1->addWidget(m_menuComboBoxRemoveAt);
        m_leftSideLayout->addWidget(m_testWidget);
            m_testLayout->addWidget(m_testLabel1, 0, 0);
            m_testLayout->addWidget(m_testLabel2, 0, 1);
            m_testLayout->addWidget(m_testLabel3, 0, 2);
            m_testLabel1->setFixedSize(80, 80);
            m_testLabel2->setFixedSize(80, 80);
            m_testLabel3->setFixedSize(80, 80);
            QPixmap pixmap(80, 80);
            pixmap.fill();
            m_testLabel1->setPixmap(pixmap);
            m_testLabel2->setPixmap(pixmap);
            m_testLabel3->setPixmap(pixmap);
            QLabel* test1 = new QLabel("Test 1", m_testWidget);
            QLabel* test2 = new QLabel("Test 2", m_testWidget);
            QLabel* test3 = new QLabel("Test 3", m_testWidget);
            m_testLayout->addWidget(test1, 1, 0);
            m_testLayout->addWidget(test2, 1, 1);
            m_testLayout->addWidget(test3, 1, 2);
            m_testLayout->addWidget(m_testButton, 2, 0);
            m_testLayout->addWidget(m_testCheckBox, 2, 1, 1, 2);
            m_testCheckBox->setToolTip("How likely is it for the jigsawpuzzle to find a valid path with Your creation?\n"
                                       "A value of 0.5 and above makes it more likely, a value of 0.0 makes ist very unlikely.");
            m_testLayout->addWidget(m_testResultLabel, 3, 0, 1, 3);
        m_leftSideLayout->addWidget(m_menuButtonsWidget2);
            m_menuButtonsLayout2->addWidget(m_menuButtonApply);
            m_menuButtonsLayout2->addWidget(m_menuButtonOpen);
            m_menuButtonOpen->setEnabled(false);
            m_menuButtonsLayout2->addWidget(m_menuButtonSave);
            m_menuButtonSave->setEnabled(false);
    m_mainLayout->addWidget(m_rightSideScrollArea);
        m_rightSideScrollArea->setWidget(m_rightSideWidget);
        m_rightSideScrollArea->setWidgetResizable(true);
        m_rightSideScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPoint controlPoint1 = START + (MIDDLE - START) / 2 - QPoint(0, 30);
    QPoint controlPoint2 = MIDDLE + (END - MIDDLE) / 2 + QPoint(0, 30);

    CustomJigsawPath::RandomOffset noOffset;
    CustomJigsawPath::RandomOffset circularOffset(CustomJigsawPath::TypeOfRandomOffset::CIRCULAR);

    CustomJigsawPath::Restriction noRestriction;
    CustomJigsawPath::Restriction fixedPosition(CustomJigsawPath::TypeOfRestriction::FIXEDPOSITION);

    CustomJigsawPath::PathPoint pathPointStart(START, noOffset, fixedPosition);
    CustomJigsawPath::PathPoint pathPoint1(MIDDLE, circularOffset, noRestriction, controlPoint1, noOffset, noRestriction);
    CustomJigsawPath::PathPoint pathPointEnd(END, noOffset, fixedPosition, controlPoint2, circularOffset, noRestriction);

    m_customJigsawPath = CustomJigsawPath({pathPointStart, pathPoint1, pathPointEnd});
    addPathPoint(0, false, true);
    addPathPoint(1, false);
    addPathPoint(2, false);

    synchronizeWidgetsWithCustomJigsawPath();
    updateInsertRemoveComboBoxes();
    repaintCanvas();

    QObject::connect(m_canvas, &CreatorCanvas::clicked, this, &CustomJigsawPathCreator::clicked);
    QObject::connect(m_canvas, &CreatorCanvas::released, this, &CustomJigsawPathCreator::released);
    QObject::connect(m_canvas, &CreatorCanvas::dragged, this, &CustomJigsawPathCreator::dragged);

    QObject::connect(m_menuButtonInsert, &QPushButton::clicked, this, &CustomJigsawPathCreator::insert);
    QObject::connect(m_menuButtonRemove, &QPushButton::clicked, this, &CustomJigsawPathCreator::remove);
    QObject::connect(m_testButton, &QPushButton::clicked, this, &CustomJigsawPathCreator::testButtonClicked);

    QObject::connect(m_menuButtonApply, &QPushButton::clicked, this, &CustomJigsawPathCreator::applyButtonClicked);
}

CustomJigsawPathCreator::~CustomJigsawPathCreator()
{
}

void CustomJigsawPathCreator::clicked(const QPoint &position)
{
    for (int i = 0; i < m_customJigsawPath.numberOfPoints(); ++i) {
        if ((position - m_customJigsawPath.pathPoint(i).position).manhattanLength() <= 5) {
            m_selectedPathPoint = i;
            break;
        }
        if (i == 0) continue;
        if ((position - m_customJigsawPath.pathPoint(i).positionControlPoint).manhattanLength() <= 5) {
            m_selectedControlPoint = i;
            break;
        }
    }
}

void CustomJigsawPathCreator::released()
{
    m_selectedPathPoint = -1;
    m_selectedControlPoint = -1;
}

void CustomJigsawPathCreator::dragged(const QPoint &position)
{
    int newPositionX = position.x() < 0 ? 0 : position.x() >= m_canvas->width() ? m_canvas->width() - 1 : position.x();
    int newPositionY = position.y() < 0 ? 0 : position.y() >= m_canvas->height() ? m_canvas->height() - 1 : position.y();
    QPoint newPosition(newPositionX, newPositionY);

    if (m_selectedPathPoint != -1) {
        m_customJigsawPath.setPositionPathPoint(m_selectedPathPoint, newPosition);
        repaintCanvas();
        updateWidgetsValue();
    }
    if (m_selectedControlPoint != -1) {
        m_customJigsawPath.setPositionControlPoint(m_selectedControlPoint, newPosition);
        repaintCanvas();
        updateWidgetsValue();
    }
}

void CustomJigsawPathCreator::typeChanged()
{
    m_signalBlocker.blockSignals();
    for (int i = 0; i < m_customJigsawPath.numberOfPoints(); ++i) {

        if (i >= m_pointsLabel.size()) break;

        m_pointsXSpinBox[i]->setEnabled(m_pointsRestrictionType[i]->currentIndex() == 0 || m_pointsRestrictionType[i]->currentIndex() == 2);
        m_pointsYSpinBox[i]->setEnabled(m_pointsRestrictionType[i]->currentIndex() == 0 || m_pointsRestrictionType[i]->currentIndex() == 2);

        m_pointsRandomOffsetValue[i]->setEnabled(m_pointsRandomOffsetType[i]->currentIndex() != 0);
        m_pointsRandomOffsetL1P1[i]->setEnabled(m_pointsRandomOffsetType[i]->currentIndex() == 2);
        m_pointsRandomOffsetL1P2[i]->setEnabled(m_pointsRandomOffsetType[i]->currentIndex() == 2);

        m_pointsRestrictionL1P1[i]->setEnabled(m_pointsRestrictionType[i]->currentIndex() == 2 || m_pointsRestrictionType[i]->currentIndex() == 3);
        m_pointsRestrictionL1P2[i]->setEnabled(m_pointsRestrictionType[i]->currentIndex() == 2 || m_pointsRestrictionType[i]->currentIndex() == 3);
        m_pointsRestrictionL2P1[i]->setEnabled(m_pointsRestrictionType[i]->currentIndex() == 3);
        m_pointsRestrictionL2P2[i]->setEnabled(m_pointsRestrictionType[i]->currentIndex() == 3);
        m_pointsRestrictionDistanceSpinBox[i]->setEnabled(m_pointsRestrictionType[i]->currentIndex() == 2);

        if (i == 0) continue;

        m_controlPointsXSpinBox[i - 1]->setEnabled(m_customJigsawPath.pathPoint(i).typeOfLine == CustomJigsawPath::TypeOfLine::CURVE &&
                                                  (m_controlPointsRestrictionType[i - 1]->currentIndex() == 0 || m_controlPointsRestrictionType[i - 1]->currentIndex() == 2));
        m_controlPointsYSpinBox[i - 1]->setEnabled(m_customJigsawPath.pathPoint(i).typeOfLine == CustomJigsawPath::TypeOfLine::CURVE &&
                                                  (m_controlPointsRestrictionType[i - 1]->currentIndex() == 0 || m_controlPointsRestrictionType[i - 1]->currentIndex() == 2));

        m_controlPointsRandomOffsetValue[i - 1]->setEnabled(m_controlPointsRandomOffsetType[i - 1]->currentIndex() != 0);
        m_controlPointsRandomOffsetL1P1[i - 1]->setEnabled(m_controlPointsRandomOffsetType[i - 1]->currentIndex() == 2);
        m_controlPointsRandomOffsetL1P2[i - 1]->setEnabled(m_controlPointsRandomOffsetType[i - 1]->currentIndex() == 2);

        m_controlPointsRestrictionL1P1[i - 1]->setEnabled(m_controlPointsRestrictionType[i - 1]->currentIndex() == 2 || m_controlPointsRestrictionType[i - 1]->currentIndex() == 3);
        m_controlPointsRestrictionL1P2[i - 1]->setEnabled(m_controlPointsRestrictionType[i - 1]->currentIndex() == 2 || m_controlPointsRestrictionType[i - 1]->currentIndex() == 3);
        m_controlPointsRestrictionL2P1[i - 1]->setEnabled(m_controlPointsRestrictionType[i - 1]->currentIndex() == 3);
        m_controlPointsRestrictionL2P2[i - 1]->setEnabled(m_controlPointsRestrictionType[i - 1]->currentIndex() == 3);
        m_controlPointsRestrictionDistanceSpinBox[i - 1]->setEnabled(m_controlPointsRestrictionType[i - 1]->currentIndex() == 2);
    }

    m_pointsRestrictionType.last()->setEnabled(false);
    m_pointsRestrictionType[0]->setEnabled(false);

    m_signalBlocker.unblockSignals();
}

void CustomJigsawPathCreator::inputFromWidgets()
{
    for (int i = 0; i < m_customJigsawPath.numberOfPoints(); ++i) {

        if (i >= m_pointsLabel.size()) break;

        m_customJigsawPath.setPositionPathPoint(i, QPoint(m_pointsXSpinBox[i]->value(), m_pointsYSpinBox[i]->value()));
        m_customJigsawPath.setRandomOffsetPathPoint(i, static_cast<CustomJigsawPath::TypeOfRandomOffset>(m_pointsRandomOffsetType[i]->currentIndex()),
                                                    m_pointsRandomOffsetValue[i]->value(),
                                                    qStringToQPointFPointer(m_pointsRandomOffsetL1P1[i]->currentText()),
                                                    qStringToQPointFPointer(m_pointsRandomOffsetL1P2[i]->currentText()));
        m_customJigsawPath.setRestrictionPathPoint(i, static_cast<CustomJigsawPath::TypeOfRestriction>(m_pointsRestrictionType[i]->currentIndex()),
                                                   qStringToQPointFPointer(m_pointsRestrictionL1P1[i]->currentText()),
                                                   qStringToQPointFPointer(m_pointsRestrictionL1P2[i]->currentText()),
                                                   qStringToQPointFPointer(m_pointsRestrictionL2P1[i]->currentText()),
                                                   qStringToQPointFPointer(m_pointsRestrictionL2P2[i]->currentText()),
                                                   m_pointsRestrictionDistanceSpinBox[i]->value());
        if (i == 0) continue;

        m_customJigsawPath.setPositionControlPoint(i, QPoint(m_controlPointsXSpinBox[i - 1]->value(), m_controlPointsYSpinBox[i - 1]->value()));
        m_customJigsawPath.setRandomOffsetControlPoint(i, static_cast<CustomJigsawPath::TypeOfRandomOffset>(m_controlPointsRandomOffsetType[i - 1]->currentIndex()),
                                                       m_controlPointsRandomOffsetValue[i - 1]->value(),
                                                       qStringToQPointFPointer(m_controlPointsRandomOffsetL1P1[i - 1]->currentText()),
                                                       qStringToQPointFPointer(m_controlPointsRandomOffsetL1P2[i - 1]->currentText()));
        m_customJigsawPath.setRestrictionControlPoint(i, static_cast<CustomJigsawPath::TypeOfRestriction>(m_controlPointsRestrictionType[i - 1]->currentIndex()),
                                                      qStringToQPointFPointer(m_controlPointsRestrictionL1P1[i - 1]->currentText()),
                                                      qStringToQPointFPointer(m_controlPointsRestrictionL1P2[i - 1]->currentText()),
                                                      qStringToQPointFPointer(m_controlPointsRestrictionL2P1[i - 1]->currentText()),
                                                      qStringToQPointFPointer(m_controlPointsRestrictionL2P2[i - 1]->currentText()),
                                                      m_controlPointsRestrictionDistanceSpinBox[i - 1]->value());

        m_customJigsawPath.setTypeOfLine(i, static_cast<CustomJigsawPath::TypeOfLine>(m_linesType[i - 1]->currentIndex()));

    }
    synchronizeWidgetsWithCustomJigsawPath();
    repaintCanvas();
}

void CustomJigsawPathCreator::insert()
{
    addPathPoint(m_menuComboBoxInsertBefore->currentIndex() + 1);
}

void CustomJigsawPathCreator::remove()
{
    removePathPoint(m_menuComboBoxRemoveAt->currentIndex() + 1);
}

void CustomJigsawPathCreator::testButtonClicked()
{
    m_testResultLabel->setText("Testing...");

    QSize size(80, 80);

    QPixmap pixmap1(size);
    pixmap1.fill();
    QPainter painter1(&pixmap1);
    QPixmap pixmap2(size);
    pixmap2.fill();
    QPainter painter2(&pixmap2);
    QPixmap pixmap3(size);
    pixmap3.fill();
    QPainter painter3(&pixmap3);

    QPoint outerBoundsTopLeft(0, 0);
    QSize outerBoundsSize(size);
    QRect outerBounds(outerBoundsTopLeft, outerBoundsSize);

    JigsawPath jigsawPath(m_customJigsawPath.pathPoint(0).position.toPoint(),
                          m_customJigsawPath.pathPoint(m_customJigsawPath.numberOfPoints() - 1).position.toPoint(),
                          m_canvas->rect(), Jigsaw::TypeOfPiece::CUSTOM, m_customJigsawPath, true);

    double maxHeightPath = jigsawPath.path().boundingRect().height();
    double maxWidthPath = jigsawPath.path().boundingRect().width();
    double ratio = maxHeightPath / maxWidthPath;

    double sizePercentage;

    if (m_testCheckBox->isChecked()) {
        sizePercentage = 1.0 - 2 * ratio;
        sizePercentage = sizePercentage <= 0.4 ? 0.4 : sizePercentage;
        sizePercentage = JigsawPath::calculateRecommendedInnerBoundsPercentage({Jigsaw::TypeOfPiece::CUSTOM}, QSize(100, 100), round(sizePercentage * 100), 4, m_customJigsawPath).at(0);
    }
    else {
        sizePercentage = 1.0 - 3 * ratio;
        sizePercentage = sizePercentage <= 0 ? 0.1 : sizePercentage;
    }

    QPoint innerBoundsTopLeft = outerBoundsTopLeft + QPoint((1.0 - sizePercentage) / 2 * outerBounds.width(), (1.0 - sizePercentage) / 2 * outerBounds.height());
    QSize innerBoundsSize(sizePercentage * outerBounds.width(), sizePercentage * outerBounds.height());
    QRect innerBounds(innerBoundsTopLeft, innerBoundsSize);

    QPainterPath path1 = JigsawPath::singleJigsawPiecePath(outerBounds, innerBounds, Jigsaw::TypeOfPiece::CUSTOM, 4, false, m_customJigsawPath);
    QPainterPath path2 = JigsawPath::singleJigsawPiecePath(outerBounds, innerBounds, Jigsaw::TypeOfPiece::CUSTOM, 4, false, m_customJigsawPath);
    QPainterPath path3 = JigsawPath::singleJigsawPiecePath(outerBounds, innerBounds, Jigsaw::TypeOfPiece::CUSTOM, 4, false, m_customJigsawPath);

    painter1.setPen(Qt::black);
    painter1.setBrush(Qt::gray);
    painter1.drawPath(path1);
    m_testLabel1->setPixmap(pixmap1);

    painter2.setPen(Qt::black);
    painter2.setBrush(Qt::gray);
    painter2.drawPath(path2);
    m_testLabel2->setPixmap(pixmap2);

    painter3.setPen(Qt::black);
    painter3.setBrush(Qt::gray);
    painter3.drawPath(path3);
    m_testLabel3->setPixmap(pixmap3);

    m_testResultLabel->setText(m_testCheckBox->isChecked() ? "Finished! (Quality: " + QString::number(sizePercentage) + ")" : "Finished!");
}

void CustomJigsawPathCreator::applyButtonClicked()
{
    emit apply(m_customJigsawPath);
}

void CustomJigsawPathCreator::repaintCanvas()
{
    int distanceStartEnd = (m_customJigsawPath.pathPoint(m_customJigsawPath.numberOfPoints() - 1).position - m_customJigsawPath.pathPoint(0).position).manhattanLength();

    QPixmap pixmap(m_canvas->size());
    pixmap.fill();

    JigsawPath jigsawPath(m_customJigsawPath.pathPoint(0).position.toPoint(),
                          m_customJigsawPath.pathPoint(m_customJigsawPath.numberOfPoints() - 1).position.toPoint(),
                          m_canvas->rect(), Jigsaw::TypeOfPiece::CUSTOM, m_customJigsawPath, true);

    QPainter painter(&pixmap);
    painter.setBrush(Qt::transparent);

    for (int i = 0; i < m_customJigsawPath.numberOfPoints(); ++i) {
        painter.setPen(QPen(Qt::lightGray, 5));
        painter.drawPoint(m_customJigsawPath.pathPoint(i).position);

        if (i != 0 && i != m_customJigsawPath.numberOfPoints() - 1) {
            painter.setPen(Qt::black);
            painter.drawText(m_customJigsawPath.pathPoint(i).position + QPoint(0, -10), "P" + QString::number(i));
        }

        switch (m_customJigsawPath.pathPoint(i).randomOffset.type) {
        case CustomJigsawPath::TypeOfRandomOffset::NONE: {
            break;
        }
        case CustomJigsawPath::TypeOfRandomOffset::CIRCULAR: {
            double value = 1.0 * m_customJigsawPath.pathPoint(i).randomOffset.value * distanceStartEnd / 100;
            painter.setPen(QPen(Qt::red, 1));
            painter.drawEllipse(QRectF(m_customJigsawPath.pathPoint(i).position - QPointF(value, value), QSizeF(value * 2, value * 2)));
            break;
        }
        case CustomJigsawPath::TypeOfRandomOffset::LINE: {
            double value = 1.0 * m_customJigsawPath.pathPoint(i).randomOffset.value * distanceStartEnd / 100;
            QLineF restrictionLine(*m_customJigsawPath.pathPoint(i).randomOffset.lineP1, *m_customJigsawPath.pathPoint(i).randomOffset.lineP2);
            QLineF offsetLine1;
            offsetLine1.setP1(m_customJigsawPath.pathPoint(i).position);
            offsetLine1.setAngle(restrictionLine.angle());
            offsetLine1.setLength(value);
            QLineF offsetLine2;
            offsetLine2.setP1(m_customJigsawPath.pathPoint(i).position);
            offsetLine2.setAngle(restrictionLine.angle() - 180);
            offsetLine2.setLength(value);
            painter.setPen(QPen(Qt::red, 3));
            painter.drawLine(offsetLine1);
            painter.drawLine(offsetLine2);
            break;
        }
        }

        if (i == 0 || m_customJigsawPath.pathPoint(i).typeOfLine == CustomJigsawPath::TypeOfLine::STRAIGHT) continue;

        painter.setPen(QPen(Qt::lightGray, 5));
        painter.drawPoint(m_customJigsawPath.pathPoint(i).positionControlPoint);
        painter.setPen(Qt::black);
        painter.drawText(m_customJigsawPath.pathPoint(i).positionControlPoint + QPoint(0, -10), "C" + QString::number(i));

        switch (m_customJigsawPath.pathPoint(i).randomOffsetControlPoint.type) {
        case CustomJigsawPath::TypeOfRandomOffset::NONE: {
            break;
        }
        case CustomJigsawPath::TypeOfRandomOffset::CIRCULAR: {
            double value = 1.0 * m_customJigsawPath.pathPoint(i).randomOffsetControlPoint.value * distanceStartEnd / 100;
            painter.setPen(QPen(Qt::red, 1));
            painter.drawEllipse(QRectF(m_customJigsawPath.pathPoint(i).positionControlPoint - QPointF(value, value), QSizeF(value * 2, value * 2)));
            break;
        }
        case CustomJigsawPath::TypeOfRandomOffset::LINE: {
            double value = 1.0 * m_customJigsawPath.pathPoint(i).randomOffsetControlPoint.value * distanceStartEnd / 100;
            QLineF restrictionLine(*m_customJigsawPath.pathPoint(i).randomOffsetControlPoint.lineP1, *m_customJigsawPath.pathPoint(i).randomOffsetControlPoint.lineP2);
            QLineF offsetLine1;
            offsetLine1.setP1(m_customJigsawPath.pathPoint(i).positionControlPoint);
            offsetLine1.setAngle(restrictionLine.angle());
            offsetLine1.setLength(value);
            QLineF offsetLine2;
            offsetLine2.setP1(m_customJigsawPath.pathPoint(i).positionControlPoint);
            offsetLine2.setAngle(restrictionLine.angle() - 180);
            offsetLine2.setLength(value);
            painter.setPen(QPen(Qt::red, 3));
            painter.drawLine(offsetLine1);
            painter.drawLine(offsetLine2);
            break;
        }
        }

        painter.setPen(QPen(Qt::darkGray, 1));
        painter.drawLine(m_customJigsawPath.pathPoint(i).positionControlPoint, m_customJigsawPath.pathPoint(i).position);
        painter.drawLine(m_customJigsawPath.pathPoint(i).positionControlPoint, m_customJigsawPath.pathPoint(i - 1).position);
    }

    painter.setPen(QPen(Qt::black, 3));
    painter.drawPath(jigsawPath.path());
    m_canvas->setPixmap(pixmap);
}

void CustomJigsawPathCreator::updateWidgetsValue()
{
    for (int i = 0; i < m_customJigsawPath.numberOfPoints(); ++i) {
        m_pointsXSpinBox[i]->setValue(m_customJigsawPath.pathPoint(i).position.x());
        m_pointsYSpinBox[i]->setValue(m_customJigsawPath.pathPoint(i).position.y());
        if (i == 0) continue;

        m_controlPointsXSpinBox[i - 1]->setValue(m_customJigsawPath.pathPoint(i).positionControlPoint.x());
        m_controlPointsYSpinBox[i - 1]->setValue(m_customJigsawPath.pathPoint(i).positionControlPoint.y());
    }
}

void CustomJigsawPathCreator::synchronizeWidgetsWithCustomJigsawPath()
{
    m_signalBlocker.blockSignals();

    for (int i = 0; i < m_customJigsawPath.numberOfPoints(); ++i) {

        if (i >= m_pointsLabel.size()) break;

        m_pointsLabel[i]->setText("Point " + QString::number(i) + ":");
        m_pointsXSpinBox[i]->setValue(m_customJigsawPath.pathPoint(i).position.x());
        m_pointsYSpinBox[i]->setValue(m_customJigsawPath.pathPoint(i).position.y());
        m_pointsXSpinBox[i]->setEnabled(m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::NONE ||
                                        m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::LINE);
        m_pointsYSpinBox[i]->setEnabled(m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::NONE ||
                                        m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::LINE);

        m_pointsRandomOffsetType[i]->setCurrentIndex(static_cast<int>(m_customJigsawPath.pathPoint(i).randomOffset.type));
        m_pointsRandomOffsetType[i]->setEnabled(m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::NONE ||
                                                m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::LINE);
        m_pointsRandomOffsetValue[i]->setValue(m_customJigsawPath.pathPoint(i).randomOffset.value);
        m_pointsRandomOffsetValue[i]->setEnabled(m_customJigsawPath.pathPoint(i).randomOffset.type != CustomJigsawPath::TypeOfRandomOffset::NONE);
        m_pointsRandomOffsetL1P1[i]->setCurrentIndex(m_pointsRandomOffsetL1P1[i]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).randomOffset.lineP1)));
        m_pointsRandomOffsetL1P2[i]->setCurrentIndex(m_pointsRandomOffsetL1P1[i]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).randomOffset.lineP2)));
        m_pointsRandomOffsetL1P1[i]->setEnabled(false);
        m_pointsRandomOffsetL1P2[i]->setEnabled(false);

        m_pointsRestrictionType[i]->setCurrentIndex(static_cast<int>(m_customJigsawPath.pathPoint(i).restriction.type));
        m_pointsRestrictionType[i]->setEnabled(true);
        m_pointsRestrictionL1P1[i]->setCurrentIndex(m_pointsRestrictionL1P1[i]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).restriction.line1P1)));
        m_pointsRestrictionL1P2[i]->setCurrentIndex(m_pointsRestrictionL1P2[i]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).restriction.line1P2)));
        m_pointsRestrictionL2P1[i]->setCurrentIndex(m_pointsRestrictionL2P1[i]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).restriction.line2P1)));
        m_pointsRestrictionL2P2[i]->setCurrentIndex(m_pointsRestrictionL2P2[i]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).restriction.line2P2)));
        m_pointsRestrictionL1P1[i]->setEnabled(m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::LINE ||
                                               m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::INTERSECTIONPOINT);
        m_pointsRestrictionL1P2[i]->setEnabled(m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::LINE ||
                                               m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::INTERSECTIONPOINT);
        m_pointsRestrictionL2P1[i]->setEnabled(m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::INTERSECTIONPOINT);
        m_pointsRestrictionL2P2[i]->setEnabled(m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::INTERSECTIONPOINT);
        m_pointsRestrictionDistanceSpinBox[i]->setValue(m_customJigsawPath.pathPoint(i).restriction.distanceFromLine);
        m_pointsRestrictionDistanceSpinBox[i]->setEnabled(m_customJigsawPath.pathPoint(i).restriction.type == CustomJigsawPath::TypeOfRestriction::LINE);

        if (i == 0) continue;

        m_linesLabel[i - 1]->setText("Line " + QString::number(i) + ":");
        m_linesType[i - 1]->setCurrentIndex(static_cast<int>(m_customJigsawPath.pathPoint(i).typeOfLine));

        m_controlPointsLabel[i - 1]->setText("Control Point " + QString::number(i) + ":");
        m_controlPointsXSpinBox[i - 1]->setValue(m_customJigsawPath.pathPoint(i).positionControlPoint.x());
        m_controlPointsYSpinBox[i - 1]->setValue(m_customJigsawPath.pathPoint(i).positionControlPoint.y());
        m_controlPointsXSpinBox[i - 1]->setEnabled(m_customJigsawPath.pathPoint(i).typeOfLine == CustomJigsawPath::TypeOfLine::CURVE &&
                                                  (m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::NONE ||
                                                   m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::LINE));
        m_controlPointsYSpinBox[i - 1]->setEnabled(m_customJigsawPath.pathPoint(i).typeOfLine == CustomJigsawPath::TypeOfLine::CURVE &&
                                                  (m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::NONE ||
                                                   m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::LINE));

        m_controlPointsRandomOffsetType[i - 1]->setCurrentIndex(static_cast<int>(m_customJigsawPath.pathPoint(i).randomOffsetControlPoint.type));
        m_controlPointsRandomOffsetType[i - 1]->setEnabled(m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::NONE ||
                                                           m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::LINE);
        m_controlPointsRandomOffsetValue[i - 1]->setValue(m_customJigsawPath.pathPoint(i).randomOffsetControlPoint.value);
        m_controlPointsRandomOffsetValue[i - 1]->setEnabled(m_customJigsawPath.pathPoint(i).randomOffsetControlPoint.type != CustomJigsawPath::TypeOfRandomOffset::NONE);
        m_controlPointsRandomOffsetL1P1[i - 1]->setCurrentIndex(m_controlPointsRandomOffsetL1P1[i - 1]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).randomOffsetControlPoint.lineP1)));
        m_controlPointsRandomOffsetL1P2[i - 1]->setCurrentIndex(m_controlPointsRandomOffsetL1P1[i - 1]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).randomOffsetControlPoint.lineP2)));
        m_controlPointsRandomOffsetL1P1[i - 1]->setEnabled(false);
        m_controlPointsRandomOffsetL1P2[i - 1]->setEnabled(false);

        m_controlPointsRestrictionType[i - 1]->setCurrentIndex(static_cast<int>(m_customJigsawPath.pathPoint(i).restrictionControlPoint.type));
        m_controlPointsRestrictionType[i - 1]->setEnabled(m_customJigsawPath.pathPoint(i).typeOfLine == CustomJigsawPath::TypeOfLine::CURVE);
        m_controlPointsRestrictionL1P1[i - 1]->setCurrentIndex(m_controlPointsRestrictionL1P1[i - 1]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).restrictionControlPoint.line1P1)));
        m_controlPointsRestrictionL1P2[i - 1]->setCurrentIndex(m_controlPointsRestrictionL1P2[i - 1]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).restrictionControlPoint.line1P2)));
        m_controlPointsRestrictionL2P1[i - 1]->setCurrentIndex(m_controlPointsRestrictionL2P1[i - 1]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).restrictionControlPoint.line2P1)));
        m_controlPointsRestrictionL2P2[i - 1]->setCurrentIndex(m_controlPointsRestrictionL2P2[i - 1]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).restrictionControlPoint.line2P2)));
        m_controlPointsRestrictionL1P1[i - 1]->setEnabled(m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::LINE ||
                                                          m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::INTERSECTIONPOINT);
        m_controlPointsRestrictionL1P2[i - 1]->setEnabled(m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::LINE ||
                                                          m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::INTERSECTIONPOINT);
        m_controlPointsRestrictionL2P1[i - 1]->setEnabled(m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::INTERSECTIONPOINT);
        m_controlPointsRestrictionL2P2[i - 1]->setEnabled(m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::INTERSECTIONPOINT);
        m_controlPointsRestrictionDistanceSpinBox[i - 1]->setValue(m_customJigsawPath.pathPoint(i).restrictionControlPoint.distanceFromLine);
        m_controlPointsRestrictionDistanceSpinBox[i - 1]->setEnabled(m_customJigsawPath.pathPoint(i).restrictionControlPoint.type == CustomJigsawPath::TypeOfRestriction::LINE);
    }

    m_pointsRestrictionType.last()->setEnabled(false);
    m_pointsRestrictionType[0]->setEnabled(false);
    m_pointsLabel.last()->setText("Ending Point:");
    m_pointsLabel[0]->setText("Starting Point:");

    m_signalBlocker.unblockSignals();
}

void CustomJigsawPathCreator::updateInsertRemoveComboBoxes()
{
    QVector<QString> allPointsNames;
    QVector<QString> allControlPointsNames;
    QVector<QString> namesForInsertComboBox;
    QVector<QString> namesForRemoveComboBox;
    QVector<QString> allNames;

    for (const auto &pointLabel : m_pointsLabel) {
        allPointsNames.push_back(pointLabel->text().chopped(1));
    }

    for (const auto &controlPointLabel : m_controlPointsLabel) {
        allControlPointsNames.push_back(controlPointLabel->text().chopped(1));
    }

    namesForInsertComboBox = allPointsNames.size() > 1 ? allPointsNames.mid(1) : QVector<QString>(0);
    namesForRemoveComboBox = namesForInsertComboBox;
    if (!namesForRemoveComboBox.isEmpty()) namesForRemoveComboBox.pop_back();

    allNames = allPointsNames + allControlPointsNames;

    m_menuComboBoxInsertBefore->clear();
    m_menuComboBoxInsertBefore->addItems(namesForInsertComboBox);
    m_menuComboBoxInsertBefore->setCurrentIndex(m_menuComboBoxInsertBefore->count() - 1);

    m_menuComboBoxRemoveAt->clear();
    m_menuComboBoxRemoveAt->addItems(namesForRemoveComboBox);
    m_menuComboBoxRemoveAt->setCurrentIndex(m_menuComboBoxRemoveAt->count() - 1);

    m_signalBlocker.blockSignals();

    for (int i = 0; i < m_pointsLabel.size(); ++i) {

        m_pointsRandomOffsetL1P1[i]->clear();
        m_pointsRandomOffsetL1P2[i]->clear();
        m_pointsRandomOffsetL1P1[i]->addItems(allNames);
        m_pointsRandomOffsetL1P2[i]->addItems(allNames);
        m_pointsRandomOffsetL1P1[i]->removeItem(i);
        m_pointsRandomOffsetL1P2[i]->removeItem(i);
        m_pointsRandomOffsetL1P1[i]->setCurrentIndex(m_pointsRandomOffsetL1P1[i]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).randomOffset.lineP1)));
        m_pointsRandomOffsetL1P2[i]->setCurrentIndex(m_pointsRandomOffsetL1P2[i]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).randomOffset.lineP2)));

        m_pointsRestrictionL1P1[i]->clear();
        m_pointsRestrictionL1P2[i]->clear();
        m_pointsRestrictionL2P1[i]->clear();
        m_pointsRestrictionL2P2[i]->clear();
        m_pointsRestrictionL1P1[i]->addItems(allNames);
        m_pointsRestrictionL1P2[i]->addItems(allNames);
        m_pointsRestrictionL2P1[i]->addItems(allNames);
        m_pointsRestrictionL2P2[i]->addItems(allNames);
        m_pointsRestrictionL1P1[i]->removeItem(i);
        m_pointsRestrictionL1P2[i]->removeItem(i);
        m_pointsRestrictionL2P1[i]->removeItem(i);
        m_pointsRestrictionL2P2[i]->removeItem(i);
        m_pointsRestrictionL1P1[i]->setCurrentIndex(m_pointsRestrictionL1P1[i]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).restriction.line1P1)));
        m_pointsRestrictionL1P2[i]->setCurrentIndex(m_pointsRestrictionL1P2[i]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).restriction.line1P2)));
        m_pointsRestrictionL2P1[i]->setCurrentIndex(m_pointsRestrictionL2P1[i]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).restriction.line2P1)));
        m_pointsRestrictionL2P2[i]->setCurrentIndex(m_pointsRestrictionL2P2[i]->findText(qPointFPointerToQString(m_customJigsawPath.pathPoint(i).restriction.line2P2)));

        if (i == 0) continue;

        m_controlPointsRandomOffsetL1P1[i - 1]->clear();
        m_controlPointsRandomOffsetL1P2[i - 1]->clear();
        m_controlPointsRandomOffsetL1P1[i - 1]->addItems(allNames);
        m_controlPointsRandomOffsetL1P2[i - 1]->addItems(allNames); //numberofpoints + i - 1
        m_controlPointsRandomOffsetL1P1[i - 1]->removeItem(m_pointsLabel.size() + i - 1);
        m_controlPointsRandomOffsetL1P2[i - 1]->removeItem(m_pointsLabel.size() + i - 1);

        m_controlPointsRestrictionL1P1[i - 1]->clear();
        m_controlPointsRestrictionL1P2[i - 1]->clear();
        m_controlPointsRestrictionL2P1[i - 1]->clear();
        m_controlPointsRestrictionL2P2[i - 1]->clear();
        m_controlPointsRestrictionL1P1[i - 1]->addItems(allNames);
        m_controlPointsRestrictionL1P2[i - 1]->addItems(allNames);
        m_controlPointsRestrictionL2P1[i - 1]->addItems(allNames);
        m_controlPointsRestrictionL2P2[i - 1]->addItems(allNames);
        m_controlPointsRestrictionL1P1[i - 1]->removeItem(m_pointsLabel.size() + i - 1);
        m_controlPointsRestrictionL1P2[i - 1]->removeItem(m_pointsLabel.size() + i - 1);
        m_controlPointsRestrictionL2P1[i - 1]->removeItem(m_pointsLabel.size() + i - 1);
        m_controlPointsRestrictionL2P2[i - 1]->removeItem(m_pointsLabel.size() + i - 1);
    }
    m_signalBlocker.unblockSignals();
}

QPointF *CustomJigsawPathCreator::qStringToQPointFPointer(const QString &text)
{
    if (text.isEmpty()) return nullptr;
    else if (text == "Starting Point") return m_customJigsawPath.positionPathPoint(0);
    else if (text == "Ending Point") return m_customJigsawPath.positionPathPoint(m_customJigsawPath.numberOfPoints() - 1);
    else if (text[0] == 'P') return m_customJigsawPath.positionPathPoint(text.last(2).front().isDigit() ? text.last(2).toInt() : text.last(1).toInt());
    else if (text[0] == 'C') return m_customJigsawPath.positionControlPoint(text.last(2).front().isDigit() ? text.last(2).toInt() : text.last(1).toInt());
    return nullptr;
}

QString CustomJigsawPathCreator::qPointFPointerToQString(QPointF *point)
{
    for (int i = 0; i < m_customJigsawPath.numberOfPoints(); ++i) {
        if (m_customJigsawPath.positionPathPoint(i) == point) {
            if (i == 0) return "Starting Point";
            else if (i == m_customJigsawPath.numberOfPoints() - 1) return "Ending Point";
            else return "Point " + QString::number(i);
        }
        if (i == 0) continue;
        if (m_customJigsawPath.positionControlPoint(i) == point) return "Control Point " + QString::number(i);
    }
    return "";
}

void CustomJigsawPathCreator::addPathPoint(int index, bool calculateValues, bool isStartingPoint)
{
    if (index <= 0 || index >= m_customJigsawPath.numberOfPoints()) index = m_customJigsawPath.numberOfPoints() - 1;

    if (m_rightSideLayout->count() != 0) {
        QLayoutItem* item = m_rightSideLayout->takeAt(m_rightSideLayout->count() - 1);
        m_rightSideLayout->removeItem(item);
        delete item;
    }
    if (calculateValues) {
        QPointF positionNextPoint(m_customJigsawPath.pathPoint(index).position);
        QPointF positionPreviousPoint(m_customJigsawPath.pathPoint(index - 1).position);
        QPointF newPosition = positionPreviousPoint + (positionNextPoint - positionPreviousPoint) / 2;

        QPointF newPositionControlPoint = newPosition - QPoint(20, 20);

        CustomJigsawPath::RandomOffset offsetPoint(CustomJigsawPath::TypeOfRandomOffset::CIRCULAR, 0.0,
                                                   m_customJigsawPath.positionPathPoint(index - 1),
                                                   m_customJigsawPath.positionPathPoint(index));
        CustomJigsawPath::Restriction noRestriction(CustomJigsawPath::TypeOfRestriction::NONE,
                                                    m_customJigsawPath.positionPathPoint(index - 1),
                                                    m_customJigsawPath.positionControlPoint(index - 1),
                                                    m_customJigsawPath.positionPathPoint(index),
                                                    m_customJigsawPath.positionControlPoint(index));

        CustomJigsawPath::PathPoint newPathPoint(newPosition, offsetPoint, noRestriction, newPositionControlPoint, offsetPoint, noRestriction);
        m_customJigsawPath.insertNewPoint(newPathPoint, index);
    }

    if (!isStartingPoint) {
        addLineToWidget();
        addControlPointToWidget();
        addControlPointRestrictionToWidget();
    }
    addPointToWidget();
    addPointRestrictionToWidget();

    m_rightSideLayout->addStretch();

    if (calculateValues) {
        synchronizeWidgetsWithCustomJigsawPath();
        updateInsertRemoveComboBoxes();
        repaintCanvas();
    }
}

void CustomJigsawPathCreator::removePathPoint(int index)
{
    if (m_customJigsawPath.numberOfPoints() <= 2) return;

    if (m_rightSideLayout->count() != 0) {
        QLayoutItem* item = m_rightSideLayout->takeAt(m_rightSideLayout->count() - 1);
        m_rightSideLayout->removeItem(item);
        delete item;
    }

    if (index <= 0 || index >= m_customJigsawPath.numberOfPoints() - 1) index = m_customJigsawPath.numberOfPoints() - 2;

    m_customJigsawPath.removePoint(index);

    for (int i = 0; i < WIDGETSPERPATHPOINT; ++i) {
        m_rightSideLayout->removeWidget(m_entriesWidget.last());
        m_entriesWidget.last()->hide();
        m_entriesWidget.pop_back();
    }

    m_pointsLabel.pop_back();
    m_pointsXSpinBox.pop_back();
    m_pointsYSpinBox.pop_back();
    m_pointsRandomOffsetType.pop_back();
    m_pointsRandomOffsetValue.pop_back();
    m_pointsRandomOffsetL1P1.pop_back();
    m_pointsRandomOffsetL1P2.pop_back();
    m_pointsRandomOffsetButton.pop_back();

    m_pointsRestrictionLabel.pop_back();
    m_pointsRestrictionType.pop_back();
    m_pointsRestrictionL1P1.pop_back();
    m_pointsRestrictionL1P2.pop_back();
    m_pointsRestrictionL2P1.pop_back();
    m_pointsRestrictionL2P2.pop_back();
    m_pointsRestrictionDistanceSpinBox.pop_back();
    m_pointsRestrictionButton.pop_back();

    m_linesLabel.pop_back();
    m_linesType.pop_back();

    m_controlPointsLabel.pop_back();
    m_controlPointsXSpinBox.pop_back();
    m_controlPointsYSpinBox.pop_back();
    m_controlPointsRandomOffsetType.pop_back();
    m_controlPointsRandomOffsetValue.pop_back();
    m_controlPointsRandomOffsetL1P1.pop_back();
    m_controlPointsRandomOffsetL1P2.pop_back();
    m_controlPointsRandomOffsetButton.pop_back();

    m_controlPointsRestrictionLabel.pop_back();
    m_controlPointsRestrictionType.pop_back();
    m_controlPointsRestrictionL1P1.pop_back();
    m_controlPointsRestrictionL1P2.pop_back();
    m_controlPointsRestrictionL2P1.pop_back();
    m_controlPointsRestrictionL2P2.pop_back();
    m_controlPointsRestrictionDistanceSpinBox.pop_back();
    m_controlPointsRestrictionButton.pop_back();

    m_rightSideLayout->addStretch();


    synchronizeWidgetsWithCustomJigsawPath();
    updateInsertRemoveComboBoxes();
    repaintCanvas();
}

void CustomJigsawPathCreator::addPointToWidget()
{
    m_entriesWidget.push_back(new QWidget(m_rightSideWidget));
    m_rightSideLayout->addWidget(m_entriesWidget.last());
    m_entriesLayout.push_back(new QHBoxLayout(m_entriesWidget.last()));
    m_entriesLayout.last()->setContentsMargins(MARGINS);

    m_pointsLabel.push_back(new QLabel(m_entriesWidget.last()));
    m_pointsLabel.last()->setFixedWidth(80);
    m_entriesLayout.last()->addWidget(m_pointsLabel.last());

    QLabel* xLabel = new QLabel("X:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(xLabel);
    m_pointsXSpinBox.push_back(new QDoubleSpinBox(m_entriesWidget.last()));
    m_pointsXSpinBox.last()->setMinimum(0);
    m_pointsXSpinBox.last()->setMaximum(m_canvas->width() - 1);
    m_entriesLayout.last()->addWidget(m_pointsXSpinBox.last());
    QObject::connect(m_pointsXSpinBox.last(), &QDoubleSpinBox::valueChanged, this, &CustomJigsawPathCreator::inputFromWidgets);
    m_signalBlocker.add(m_pointsXSpinBox.last());

    QLabel* yLabel = new QLabel("Y:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(yLabel);
    m_pointsYSpinBox.push_back(new QDoubleSpinBox(m_entriesWidget.last()));
    m_pointsYSpinBox.last()->setMinimum(0);
    m_pointsYSpinBox.last()->setMaximum(m_canvas->height() - 1);
    m_entriesLayout.last()->addWidget(m_pointsYSpinBox.last());
    QObject::connect(m_pointsYSpinBox.last(), &QDoubleSpinBox::valueChanged, this, &CustomJigsawPathCreator::inputFromWidgets);
    m_signalBlocker.add(m_pointsYSpinBox.last());

    QLabel* offsetTypeLabel = new QLabel("Offset type:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(offsetTypeLabel);
    m_pointsRandomOffsetType.push_back(new QComboBox(m_entriesWidget.last()));
    m_pointsRandomOffsetType.last()->addItems({"None", "Circular", "Line"});
    m_entriesLayout.last()->addWidget(m_pointsRandomOffsetType.last());
    QObject::connect(m_pointsRandomOffsetType.last(), &QComboBox::currentIndexChanged, this, &CustomJigsawPathCreator::typeChanged);
    m_signalBlocker.add(m_pointsRandomOffsetType.last());

    QLabel* valueOffsetLabel = new QLabel("Value", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(valueOffsetLabel);
    m_pointsRandomOffsetValue.push_back(new QDoubleSpinBox(m_entriesWidget.last()));
    m_pointsRandomOffsetValue.last()->setMinimum(0);
    m_pointsRandomOffsetValue.last()->setMaximum(10);
    m_entriesLayout.last()->addWidget(m_pointsRandomOffsetValue.last());
    QObject::connect(m_pointsRandomOffsetValue.last(), &QDoubleSpinBox::valueChanged, this, &CustomJigsawPathCreator::inputFromWidgets);
    m_signalBlocker.add(m_pointsRandomOffsetValue.last());

    QLabel* offsetLineLabel = new QLabel("Line:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(offsetLineLabel);
    m_pointsRandomOffsetL1P1.push_back(new QComboBox(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_pointsRandomOffsetL1P1.last());
    m_pointsRandomOffsetL1P2.push_back(new QComboBox(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_pointsRandomOffsetL1P2.last());

    m_pointsRandomOffsetButton.push_back(new QPushButton("Apply", m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_pointsRandomOffsetButton.last());
    QObject::connect(m_pointsRandomOffsetButton.last(), &QPushButton::clicked, this, &CustomJigsawPathCreator::inputFromWidgets);
     m_signalBlocker.add(m_pointsRandomOffsetButton.last());
}

void CustomJigsawPathCreator::addPointRestrictionToWidget()
{
    m_entriesWidget.push_back(new QWidget(m_rightSideWidget));
    m_rightSideLayout->addWidget(m_entriesWidget.last());
    m_entriesLayout.push_back(new QHBoxLayout(m_entriesWidget.last()));
    m_entriesLayout.last()->setContentsMargins(MARGINS);

    m_entriesLayout.last()->addSpacing(TABWIDTH);

    m_pointsRestrictionLabel.push_back(new QLabel("Restriction:", m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_pointsRestrictionLabel.last());

    m_pointsRestrictionType.push_back(new QComboBox(m_entriesWidget.last()));
    m_pointsRestrictionType.last()->addItems({"None", "Fixed", "Line", "Intersection"});
    m_entriesLayout.last()->addWidget(m_pointsRestrictionType.last());
    QObject::connect(m_pointsRestrictionType.last(), &QComboBox::currentIndexChanged, this, &CustomJigsawPathCreator::typeChanged);
    m_signalBlocker.add(m_pointsRestrictionType.last());

    QLabel* line1Label = new QLabel("Line 1:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(line1Label);

    m_pointsRestrictionL1P1.push_back(new QComboBox(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_pointsRestrictionL1P1.last());

    m_pointsRestrictionL1P2.push_back(new QComboBox(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_pointsRestrictionL1P2.last());

    QLabel* line2Label = new QLabel("Line 2:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(line2Label);

    m_pointsRestrictionL2P1.push_back(new QComboBox(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_pointsRestrictionL2P1.last());

    m_pointsRestrictionL2P2.push_back(new QComboBox(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_pointsRestrictionL2P2.last());

    QLabel* distanceLabel = new QLabel("Distance:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(distanceLabel);
    m_pointsRestrictionDistanceSpinBox.push_back(new QDoubleSpinBox(m_entriesWidget.last()));
    m_pointsRestrictionDistanceSpinBox.last()->setMinimum(-50);
    m_pointsRestrictionDistanceSpinBox.last()->setMaximum(50);
    m_entriesLayout.last()->addWidget(m_pointsRestrictionDistanceSpinBox.last());
    QObject::connect(m_pointsRestrictionDistanceSpinBox.last(), &QDoubleSpinBox::valueChanged, this, &CustomJigsawPathCreator::inputFromWidgets);
    m_signalBlocker.add(m_pointsRestrictionDistanceSpinBox.last());

    m_pointsRestrictionButton.push_back(new QPushButton("Apply", m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_pointsRestrictionButton.last());
    QObject::connect(m_pointsRestrictionButton.last(), &QPushButton::clicked, this, &CustomJigsawPathCreator::inputFromWidgets);
    m_signalBlocker.add(m_pointsRestrictionButton.last());
}

void CustomJigsawPathCreator::addLineToWidget()
{
    m_entriesWidget.push_back(new QWidget(m_rightSideWidget));
    m_rightSideLayout->addWidget(m_entriesWidget.last());
    m_entriesLayout.push_back(new QHBoxLayout(m_entriesWidget.last()));
    m_entriesLayout.last()->setContentsMargins(MARGINS);

    m_entriesLayout.last()->addSpacing(TABWIDTH * 2);

    m_linesLabel.push_back(new QLabel(m_entriesWidget.last()));
    m_linesLabel.last()->setFixedWidth(50);
    m_entriesLayout.last()->addWidget(m_linesLabel.last());

    QLabel* typeLabel = new QLabel("Type:", m_entriesWidget.last());
    typeLabel->setFixedWidth(30);
    m_entriesLayout.last()->addWidget(typeLabel);

    m_linesType.push_back(new QComboBox(m_entriesWidget.last()));
    m_linesType.last()->addItems({"Straight", "Curve"});
    m_entriesLayout.last()->addWidget(m_linesType.last());
    QObject::connect(m_linesType.last(), &QComboBox::currentIndexChanged, this, &CustomJigsawPathCreator::inputFromWidgets);
    m_signalBlocker.add(m_linesType.last());

    m_entriesLayout.last()->addSpacing(TABWIDTH * 10);
}

void CustomJigsawPathCreator::addControlPointToWidget()
{
    m_entriesWidget.push_back(new QWidget(m_rightSideWidget));
    m_rightSideLayout->addWidget(m_entriesWidget.last());
    m_entriesLayout.push_back(new QHBoxLayout(m_entriesWidget.last()));
    m_entriesLayout.last()->setContentsMargins(MARGINS);

    m_controlPointsLabel.push_back(new QLabel(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_controlPointsLabel.last());

    QLabel* xLabel = new QLabel("X:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(xLabel);
    m_controlPointsXSpinBox.push_back(new QDoubleSpinBox(m_entriesWidget.last()));
    m_controlPointsXSpinBox.last()->setMinimum(0);
    m_controlPointsXSpinBox.last()->setMaximum(m_canvas->width() - 1);
    m_entriesLayout.last()->addWidget(m_controlPointsXSpinBox.last());
    QObject::connect(m_controlPointsXSpinBox.last(), &QDoubleSpinBox::valueChanged, this, &CustomJigsawPathCreator::inputFromWidgets);
    m_signalBlocker.add(m_controlPointsXSpinBox.last());

    QLabel* yLabel = new QLabel("Y:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(yLabel);
    m_controlPointsYSpinBox.push_back((new QDoubleSpinBox(m_entriesWidget.last())));
    m_controlPointsYSpinBox.last()->setMinimum(0);
    m_controlPointsYSpinBox.last()->setMaximum(m_canvas->height() - 1);
    m_entriesLayout.last()->addWidget(m_controlPointsYSpinBox.last());
    QObject::connect(m_controlPointsYSpinBox.last(), &QDoubleSpinBox::valueChanged, this, &CustomJigsawPathCreator::inputFromWidgets);
    m_signalBlocker.add(m_controlPointsYSpinBox.last());

    QLabel* offsetTypeLabel = new QLabel("Offset type:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(offsetTypeLabel);
    m_controlPointsRandomOffsetType.push_back(new QComboBox(m_entriesWidget.last()));
    m_controlPointsRandomOffsetType.last()->addItems({"None", "Circular", "Line"});
    m_entriesLayout.last()->addWidget(m_controlPointsRandomOffsetType.last());
    QObject::connect(m_controlPointsRandomOffsetType.last(), &QComboBox::currentIndexChanged, this, &CustomJigsawPathCreator::typeChanged);
    m_signalBlocker.add(m_controlPointsRandomOffsetType.last());

    QLabel* valueOffsetLabel = new QLabel("Value", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(valueOffsetLabel);
    m_controlPointsRandomOffsetValue.push_back(new QDoubleSpinBox(m_entriesWidget.last()));
    m_controlPointsRandomOffsetValue.last()->setMinimum(0);
    m_controlPointsRandomOffsetValue.last()->setMaximum(10);
    m_entriesLayout.last()->addWidget(m_controlPointsRandomOffsetValue.last());
    QObject::connect(m_controlPointsRandomOffsetValue.last(), &QDoubleSpinBox::valueChanged, this, &CustomJigsawPathCreator::inputFromWidgets);
    m_signalBlocker.add(m_controlPointsRandomOffsetValue.last());

    QLabel* offsetLineLabel = new QLabel("Line:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(offsetLineLabel);
    m_controlPointsRandomOffsetL1P1.push_back(new QComboBox(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_controlPointsRandomOffsetL1P1.last());
    m_controlPointsRandomOffsetL1P2.push_back(new QComboBox(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_controlPointsRandomOffsetL1P2.last());

    m_controlPointsRandomOffsetButton.push_back(new QPushButton("Apply", m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_controlPointsRandomOffsetButton.last());
    QObject::connect(m_controlPointsRandomOffsetButton.last(), &QPushButton::clicked, this, &CustomJigsawPathCreator::inputFromWidgets);
    m_signalBlocker.add(m_controlPointsRandomOffsetButton.last());
}

void CustomJigsawPathCreator::addControlPointRestrictionToWidget()
{
    m_entriesWidget.push_back(new QWidget(m_rightSideWidget));
    m_rightSideLayout->addWidget(m_entriesWidget.last());
    m_entriesLayout.push_back(new QHBoxLayout(m_entriesWidget.last()));
    m_entriesLayout.last()->setContentsMargins(MARGINS);

    m_entriesLayout.last()->addSpacing(TABWIDTH);

    m_controlPointsRestrictionLabel.push_back(new QLabel("Restriction:", m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_controlPointsRestrictionLabel.last());

    m_controlPointsRestrictionType.push_back(new QComboBox(m_entriesWidget.last()));
    m_controlPointsRestrictionType.last()->addItems({"None", "Fixed", "Line", "Intersection"});
    m_entriesLayout.last()->addWidget(m_controlPointsRestrictionType.last());
    QObject::connect(m_controlPointsRestrictionType.last(), &QComboBox::currentIndexChanged, this, &CustomJigsawPathCreator::typeChanged);
    m_signalBlocker.add(m_controlPointsRestrictionType.last());

    QLabel* line1Label = new QLabel("Line 1:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(line1Label);

    m_controlPointsRestrictionL1P1.push_back(new QComboBox(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_controlPointsRestrictionL1P1.last());

    m_controlPointsRestrictionL1P2.push_back(new QComboBox(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_controlPointsRestrictionL1P2.last());

    QLabel* line2Label = new QLabel("Line 2:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(line2Label);

    m_controlPointsRestrictionL2P1.push_back(new QComboBox(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_controlPointsRestrictionL2P1.last());

    m_controlPointsRestrictionL2P2.push_back(new QComboBox(m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_controlPointsRestrictionL2P2.last());

    QLabel* distanceLabel = new QLabel("Distance:", m_entriesWidget.last());
    m_entriesLayout.last()->addWidget(distanceLabel);
    m_controlPointsRestrictionDistanceSpinBox.push_back(new QDoubleSpinBox(m_entriesWidget.last()));
    m_controlPointsRestrictionDistanceSpinBox.last()->setMinimum(-50);
    m_controlPointsRestrictionDistanceSpinBox.last()->setMaximum(50);
    m_entriesLayout.last()->addWidget(m_controlPointsRestrictionDistanceSpinBox.last());
    QObject::connect(m_controlPointsRestrictionDistanceSpinBox.last(), &QDoubleSpinBox::valueChanged, this, &CustomJigsawPathCreator::inputFromWidgets);
    m_signalBlocker.add(m_controlPointsRestrictionDistanceSpinBox.last());

    m_controlPointsRestrictionButton.push_back(new QPushButton("Apply", m_entriesWidget.last()));
    m_entriesLayout.last()->addWidget(m_controlPointsRestrictionButton.last());
    QObject::connect(m_controlPointsRestrictionButton.last(), &QPushButton::clicked, this, &CustomJigsawPathCreator::inputFromWidgets);
    m_signalBlocker.add(m_controlPointsRestrictionButton.last());
}
