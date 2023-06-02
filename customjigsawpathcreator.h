#ifndef CUSTOMJIGSAWPATHCREATOR_H
#define CUSTOMJIGSAWPATHCREATOR_H

#include "creatorcanvas.h"
#include "jigsawpath.h"
#include "customjigsawpath.h"
#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QScrollArea>

class CustomJigsawPathCreator : public QWidget
{
    Q_OBJECT

    const int WIDGETSPERPATHPOINT = 5;
    const int TABWIDTH = 30;
    const QMargins MARGINS = QMargins(2, 2, 2, 2);

    const QPoint START = QPoint(10, 150);
    const QPoint END = QPoint(290, 150);
    const QPoint MIDDLE = START + (END - START) / 2;
public:
    explicit CustomJigsawPathCreator(QWidget *parent = nullptr);
    ~CustomJigsawPathCreator();

public slots:
    void clicked(const QPoint &position);
    void released();
    void dragged(const QPoint &position);

    void typeChanged();
    void inputFromWidgets();
    void insert();
    void remove();
    void testButtonClicked();
    void applyButtonClicked();

signals:
    void apply(const CustomJigsawPath &customJigsawPath);

private:
    struct SignalBlocker {
        SignalBlocker() {}
        ~SignalBlocker() {unblockSignals();}
        void add(QObject* object) {m_objects.push_back(object);}
        void removeLast() {if (!m_objects.isEmpty()) m_objects.removeLast();}
        bool removeOne(QObject* object) {return m_objects.removeOne(object);}
        void blockSignals() {for (const auto &object : m_objects) object->blockSignals(true);}
        void unblockSignals() {for (const auto &object : m_objects) object->blockSignals(false);}
    private:
        QVector<QObject*> m_objects;
    };

    SignalBlocker m_signalBlocker;

    CustomJigsawPath m_customJigsawPath;
    int m_selectedPathPoint;
    int m_selectedControlPoint;

    QHBoxLayout* m_mainLayout;
        QWidget* m_leftSideWidget;
        QVBoxLayout* m_leftSideLayout;
            CreatorCanvas* m_canvas;
            QWidget* m_menuButtonsWidget1;
            QHBoxLayout* m_menuButtonsLayout1;
                QPushButton* m_menuButtonInsert;
                QComboBox* m_menuComboBoxInsertBefore;
                QPushButton* m_menuButtonRemove;
                QComboBox* m_menuComboBoxRemoveAt;
            QWidget* m_testWidget;
            QGridLayout* m_testLayout;
                QLabel* m_testLabel1;
                QLabel* m_testLabel2;
                QLabel* m_testLabel3;
                QPushButton* m_testButton;
                QCheckBox* m_testCheckBox;
                QLabel* m_testResultLabel;
            QWidget* m_menuButtonsWidget2;
            QHBoxLayout* m_menuButtonsLayout2;
                QPushButton* m_menuButtonApply;
                QPushButton* m_menuButtonOpen;
                QPushButton* m_menuButtonSave;
        QWidget* m_rightSideWidget;
        QScrollArea* m_rightSideScrollArea;
        QVBoxLayout* m_rightSideLayout;
            QVector<QWidget*> m_entriesWidget;
            QVector<QHBoxLayout*> m_entriesLayout;

                QVector<QLabel*> m_pointsLabel;
                QVector<QDoubleSpinBox*> m_pointsXSpinBox;
                QVector<QDoubleSpinBox*> m_pointsYSpinBox;
                QVector<QComboBox*> m_pointsRandomOffsetType;
                QVector<QDoubleSpinBox*> m_pointsRandomOffsetValue;
                QVector<QComboBox*> m_pointsRandomOffsetL1P1;
                QVector<QComboBox*> m_pointsRandomOffsetL1P2;
                QVector<QPushButton*> m_pointsRandomOffsetButton;

                QVector<QLabel*> m_pointsRestrictionLabel;
                QVector<QComboBox*> m_pointsRestrictionType;
                QVector<QComboBox*> m_pointsRestrictionL1P1;
                QVector<QComboBox*> m_pointsRestrictionL1P2;
                QVector<QComboBox*> m_pointsRestrictionL2P1;
                QVector<QComboBox*> m_pointsRestrictionL2P2;
                QVector<QDoubleSpinBox*> m_pointsRestrictionDistanceSpinBox;
                QVector<QPushButton*> m_pointsRestrictionButton;

                QVector<QLabel*> m_linesLabel;
                QVector<QComboBox*> m_linesType;

                QVector<QLabel*> m_controlPointsLabel;
                QVector<QDoubleSpinBox*> m_controlPointsXSpinBox;
                QVector<QDoubleSpinBox*> m_controlPointsYSpinBox;
                QVector<QComboBox*> m_controlPointsRandomOffsetType;
                QVector<QDoubleSpinBox*> m_controlPointsRandomOffsetValue;
                QVector<QComboBox*> m_controlPointsRandomOffsetL1P1;
                QVector<QComboBox*> m_controlPointsRandomOffsetL1P2;
                QVector<QPushButton*> m_controlPointsRandomOffsetButton;

                QVector<QLabel*> m_controlPointsRestrictionLabel;
                QVector<QComboBox*> m_controlPointsRestrictionType;
                QVector<QComboBox*> m_controlPointsRestrictionL1P1;
                QVector<QComboBox*> m_controlPointsRestrictionL1P2;
                QVector<QComboBox*> m_controlPointsRestrictionL2P1;
                QVector<QComboBox*> m_controlPointsRestrictionL2P2;
                QVector<QDoubleSpinBox*> m_controlPointsRestrictionDistanceSpinBox;
                QVector<QPushButton*> m_controlPointsRestrictionButton;

    void repaintCanvas();
    void updateWidgetsValue();
    void synchronizeWidgetsWithCustomJigsawPath();
    void updateInsertRemoveComboBoxes();
    QPointF* qStringToQPointFPointer(const QString &text);
    QString qPointFPointerToQString(QPointF* point);

    void addPathPoint(int index = -1, bool calculateValues = true, bool isStartingPoint = false);
    void removePathPoint(int index);

    void addPointToWidget();
    void addPointRestrictionToWidget();
    void addLineToWidget();
    void addControlPointToWidget();
    void addControlPointRestrictionToWidget();

};

#endif // CUSTOMJIGSAWPATHCREATOR_H
