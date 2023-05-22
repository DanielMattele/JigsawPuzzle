#include "puzzlewidget.h"
#include "custommenuwidget.h"
#include "qapplication.h"
#include "qdebug.h"
#include <random>
#include <QPainter>
#include <QGroupBox>
#include <QRadioButton>
#include <QFile>
#include <QFileDialog>

void PuzzleWidget::createNewMergedPiece(JigsawPiece *firstPiece)
{
    m_mergedPieces.push_back(QVector<JigsawPiece*>(1, firstPiece));
    QObject::disconnect(firstPiece, &JigsawPiece::dragStopped, this, &PuzzleWidget::fixPieceIfPossible);
    QObject::connect(firstPiece, &JigsawPiece::dragStopped, this, &PuzzleWidget::fixMergedPieceIfPossible);
    QObject::disconnect(firstPiece, &JigsawPiece::rotateStopped, this, &PuzzleWidget::fixPieceIfPossible);
    QObject::connect(firstPiece, &JigsawPiece::rotateStopped, this, &PuzzleWidget::fixMergedPieceIfPossible);
}

void PuzzleWidget::addPuzzlePieceToMergedPiece(JigsawPiece *piece, int mergedPieceID)
{
    m_mergedPieces[mergedPieceID].push_back(piece);
    QObject::disconnect(piece, &JigsawPiece::dragStopped, this, &PuzzleWidget::fixPieceIfPossible);
    QObject::connect(piece, &JigsawPiece::dragStopped, this, &PuzzleWidget::fixMergedPieceIfPossible);
    QObject::disconnect(piece, &JigsawPiece::rotateStopped, this, &PuzzleWidget::fixPieceIfPossible);
    QObject::connect(piece, &JigsawPiece::rotateStopped, this, &PuzzleWidget::fixMergedPieceIfPossible);
}

void PuzzleWidget::combineTwoMergedPieces(int firstMergedPieceID, int secondMergedPieceID)
{
    m_mergedPieces[firstMergedPieceID] += m_mergedPieces[secondMergedPieceID];
    m_mergedPieces.removeAt(secondMergedPieceID);
    if (secondMergedPieceID < firstMergedPieceID) m_selectedMergedPiece = --firstMergedPieceID;
}

bool PuzzleWidget::isPartOfMergedPiece(JigsawPiece *piece, int &mergedPieceID)
{
    mergedPieceID = -1;
    for (int i = 0; i < m_mergedPieces.size(); ++i) {
        if (m_mergedPieces[i].contains(piece)) {
            mergedPieceID = i;
            return true;
        }
    }
    return false;
}

bool PuzzleWidget::areMerged(JigsawPiece *first, JigsawPiece *second)
{
    int firstMergedPieceID;
    int secondMergedPieceID;
    if (!isPartOfMergedPiece(first, firstMergedPieceID)) return false;
    if (!isPartOfMergedPiece(second, secondMergedPieceID)) return false;
    if (firstMergedPieceID == -1 && secondMergedPieceID == -1) return false;
    if (firstMergedPieceID == secondMergedPieceID) return true;
    return false;
}

void PuzzleWidget::fixPieceIfPossible(int id)
{
    JigsawPiece* piece = m_puzzlePieces[id];
    piece->lower();
    m_background->lower();
    int pieceNorthID = id - m_cols;
    int pieceEastID = (id + 1) % m_cols == 0 ? -1 : id + 1;
    int pieceSouthID = (id + m_cols >= m_numberOfPieces) ? -1 : id + m_cols;
    int pieceWestID = id % m_cols == 0 ? -1 : id - 1;

    QVector<JigsawPiece*> neighbors;
    neighbors.push_back(pieceNorthID >= 0 ? m_puzzlePieces[pieceNorthID] : nullptr);
    neighbors.push_back(pieceEastID >= 0 ? m_puzzlePieces[pieceEastID] : nullptr);
    neighbors.push_back(pieceSouthID >= 0 ? m_puzzlePieces[pieceSouthID] : nullptr);
    neighbors.push_back(pieceWestID >= 0 ? m_puzzlePieces[pieceWestID] : nullptr);

    int mergedPieceID;
    int neighborMergedPieceID;

    for (const auto &neighbor: neighbors) {
        if (isInCorrectPosition(piece, neighbor) && !areMerged(piece, neighbor)) {

            if (isPartOfMergedPiece(piece, mergedPieceID)) {
                for (const auto &pieces : m_mergedPieces[mergedPieceID]) {
                    repositionPiece(pieces, neighbor);
                }
                if (isPartOfMergedPiece(neighbor, neighborMergedPieceID)) {
                    combineTwoMergedPieces(mergedPieceID, neighborMergedPieceID);
                }
                else {
                    addPuzzlePieceToMergedPiece(neighbor, mergedPieceID);
                }
            }
            else
            {
                repositionPiece(piece, neighbor);
                if (isPartOfMergedPiece(neighbor, neighborMergedPieceID)) {
                    addPuzzlePieceToMergedPiece(piece, neighborMergedPieceID);
                }
                else {
                    createNewMergedPiece(piece);
                    isPartOfMergedPiece(piece, mergedPieceID);
                    addPuzzlePieceToMergedPiece(neighbor, mergedPieceID);
                }
            }
        }
    }
    if (!m_mergedPieces.isEmpty() && m_mergedPieces[0].size() == m_numberOfPieces) m_wonWidget->show();
}

void PuzzleWidget::fixMergedPieceIfPossible(int id)
{
    int mergedPieceID;
    isPartOfMergedPiece(m_puzzlePieces[id], mergedPieceID);
    for (const auto &piece: m_mergedPieces[mergedPieceID]) {
        fixPieceIfPossible(piece->id());
    }
}

bool PuzzleWidget::isInCorrectPosition(JigsawPiece *piece, JigsawPiece *neighbor, int tolerance)
{
    if (!neighbor) return false;

    QPoint pieceGridPoint = m_grid->overlayGridPoint(piece->id());
    QPoint neighborGridPoint = m_grid->overlayGridPoint(neighbor->id());
    QPoint gridPointOffset = pieceGridPoint - neighborGridPoint;

    QPointF piecePosition = piece->originalPosition();
    QPointF pieceNorthPosition = neighbor->originalPosition();
    QPointF pieceOffset = piecePosition - pieceNorthPosition;

    QPointF positionDifference = gridPointOffset - pieceOffset;
    double distance = positionDifference.manhattanLength();

    return distance <= tolerance;
}

void PuzzleWidget::repositionPiece(JigsawPiece *piece, JigsawPiece *neighbor)
{
    QPoint pieceGridPoint = m_grid->overlayGridPoint(piece->id());
    QPoint neighborGridPoint = m_grid->overlayGridPoint(neighbor->id());
    QPoint gridPointOffset = pieceGridPoint - neighborGridPoint;
    QPointF correctPosition = neighbor->originalPosition() + gridPointOffset;
    piece->move(correctPosition);
}

void PuzzleWidget::calculateRowsAndCols(int numberOfPieces, const QPixmap &image)
{
    int maxImageWidth = m_parameters.screenWidth * 2 / 3;
    int maxImageHeight = m_parameters.screenHeight * 4 / 5;
    QSize maxImageSize(maxImageWidth, maxImageHeight);
    QSize imageSize(image.size());
    QSize actualImageSize = imageSize.scaled(maxImageSize, Qt::KeepAspectRatio);
    double imageRatio = 1.0 * actualImageSize.width() / actualImageSize.height();

    //cols = rows * imageRatio

    int rows = 0;
    int cols;
    int actualNumberOfPieces;

    do {
        ++rows;
        cols = imageRatio * rows;
        actualNumberOfPieces = cols * rows;
    }
    while (actualNumberOfPieces < numberOfPieces);

    m_rows = rows;
    m_cols = cols;
    m_numberOfPieces = actualNumberOfPieces;

    m_pieceWidth = actualImageSize.width() / cols;
    m_pieceHeight = actualImageSize.height() / rows;
}

int PuzzleWidget::randomOffset(unsigned int range) const
{
    std::random_device rd;
    std::mt19937 g(rd());
    int result = (g() % range) - range / 2;
    return result;
}

void PuzzleWidget::setDefaultValues()
{
    m_rows = 5;
    m_cols = 7;
    m_numberOfPieces = m_rows * m_cols;
    m_typeOfPiece = TypeOfPiece::TRAPEZOID;

    m_pieceWidth = 80;
    m_pieceHeight = 80;

    m_fps = 60;
}

void PuzzleWidget::setupImage()
{
    QSize overlaySize(m_grid->puzzleTotalSize());
    QSize scaledImageSize(m_cols * m_pieceWidth + 1, m_rows * m_pieceHeight + 1);

    QPixmap overlayPixmap(overlaySize);
    overlayPixmap.fill(Qt::transparent);
    QPixmap scaledImage = m_image.scaled(scaledImageSize);

    QPainter painter(&overlayPixmap);
    painter.drawPixmap(m_grid->symmetricGridPoint(0), scaledImage, QRect(QPoint(0, 0), scaledImageSize));

    m_image = overlayPixmap;
}

void PuzzleWidget::generatePuzzlePieces()
{
    for (unsigned int i = 0; i < m_numberOfPieces; ++i) {
        m_puzzlePieces.push_back(new JigsawPiece(i, m_grid->pieceTotalSize(), QBrush(createImageFragment(i)), m_grid->puzzlePath(i), this));
        m_puzzlePieces.last()->setRotationEnabled(false);
        QObject::connect(m_puzzlePieces.last(), &JigsawPiece::dragStarted, this, &PuzzleWidget::raisePieces);
        QObject::connect(m_puzzlePieces.last(), &JigsawPiece::dragged, this, &PuzzleWidget::dragMergedPieces);
        QObject::connect(m_puzzlePieces.last(), &JigsawPiece::dragStopped, this, &PuzzleWidget::fixPieceIfPossible);
        QObject::connect(m_puzzlePieces.last(), &JigsawPiece::rotateStopped, this, &PuzzleWidget::fixPieceIfPossible);
    }
}

void PuzzleWidget::placePuzzlePieces()
{
    for (auto piece: m_puzzlePieces) {
        double ax, ay;
        do {
            ax = PuzzleGrid::randomNumber(0, m_parameters.screenWidth - m_grid->pieceTotalWidth());
            ay = PuzzleGrid::randomNumber(0, m_parameters.screenHeight - m_grid->pieceTotalHeight());
        }
        while (m_parameters.rectFreeArea.contains(QPoint(ax, ay)));
        piece->move(ax, ay);
        piece->raise();
        piece->show();
    }
}

QPixmap PuzzleWidget::createImageFragment(int pieceID)
{
    QSize puzzlePieceSize(m_grid->pieceTotalSize());
    QPixmap imageFragment(puzzlePieceSize);
    QPainter fragmentPainter(&imageFragment);

    fragmentPainter.drawPixmap(QPoint(0, 0), m_image, QRect(m_grid->overlayGridPoint(pieceID),puzzlePieceSize));
    return imageFragment;
}

void PuzzleWidget::setMenuWidget()
{
    QSize menuSize(150, 500);
    m_menuWidget = new CustomMenuWidget(menuSize, this);
    m_menuWidget->setGeometry(QRect(QPoint(-120, 10), menuSize));
    m_menuWidget->raise();

    m_moveMenuInTimer = new QTimer(this);
    m_moveMenuOutTimer = new QTimer(this);

    QObject::connect(m_moveMenuInTimer, &QTimer::timeout, this, &PuzzleWidget::moveMenuInTimerTimeout);
    QObject::connect(m_moveMenuOutTimer, &QTimer::timeout, this, &PuzzleWidget::moveMenuOutTimerTimeout);

    QObject::connect(m_menuWidget, &CustomMenuWidget::enterMenu, this, &PuzzleWidget::enterMenu);
    QObject::connect(m_menuWidget, &CustomMenuWidget::leaveMenu, this, &PuzzleWidget::leaveMenu);

    QObject::connect(m_menuWidget->newPuzzleButton(), &JigsawButton::clicked, this, &PuzzleWidget::menuNewButtonClicked);
    QObject::connect(m_menuWidget->quitButton(), &JigsawButton::clicked, this, &PuzzleWidget::menuQuitButtonClicked);
}

void PuzzleWidget::setQuitWidget()
{
    QSize sizeOuterBoundsBackground(1000, 600);
    QSize sizeInnerBoundsBackground(sizeOuterBoundsBackground - QSize(400, 400));
    QRect innerBoundsBackground(QPoint(200, 200), sizeInnerBoundsBackground);
    QSize sizeOuterBoundsButtons(130, 100);
    QSize sizeInnerBoundsButtons(70, 60);
    QPoint positionWidget(1920 / 2 - sizeOuterBoundsBackground.width() / 2, 1080 / 2 - sizeOuterBoundsBackground.height() / 2);

    m_quitWidget = new QWidget(this);
    m_quitWidget->setGeometry(QRect(positionWidget, sizeOuterBoundsBackground));

    QPainterPath backgroundLabelPath = JigsawPath::singleJigsawPiecePath(QRect(QPoint(0, 0), sizeOuterBoundsBackground), innerBoundsBackground, TypeOfPiece::STANDARD);
    JigsawLabel* backgroundLabel = new JigsawLabel(sizeOuterBoundsBackground, QBrush(QPixmap(":/backgrounds/back3")), backgroundLabelPath,
                                                   m_quitWidget, "Are you sure?", innerBoundsBackground);
    backgroundLabel->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
    backgroundLabel->setFont(m_parameters.mainFont);

    QPainterPath backgroundYesButtonPath = JigsawPath::singleJigsawPiecePath(QRect(QPoint(0, 0), sizeOuterBoundsButtons), QRect(), TypeOfPiece::STANDARD, 4, true);
    QPainterPath backgroundNoButtonPath = JigsawPath::singleJigsawPiecePath(QRect(QPoint(0, 0), sizeOuterBoundsButtons), QRect(), TypeOfPiece::STANDARD, 4, true);

    JigsawButton* yesButton = new JigsawButton(sizeOuterBoundsButtons, QBrush(QPixmap(":/backgrounds/back2")), backgroundYesButtonPath, m_quitWidget, "Yes");
    yesButton->move(innerBoundsBackground.left() + 10, innerBoundsBackground.bottom() - 10 - yesButton->height());
    yesButton->animate();
    yesButton->setFont(m_parameters.mainFont);
    QObject::connect(yesButton, &JigsawButton::clicked, this, &PuzzleWidget::quitWidgetYesClicked);

    JigsawButton* noButton = new JigsawButton(sizeOuterBoundsButtons, QBrush(QPixmap(":/backgrounds/back2")), backgroundNoButtonPath, m_quitWidget, "No");
    noButton->move(innerBoundsBackground.right() - 10 - noButton->width(), innerBoundsBackground.bottom() - 10 - noButton->height());
    noButton->animate();
    noButton->setFont(m_parameters.mainFont);
    QObject::connect(noButton, &JigsawButton::clicked, m_quitWidget, &QWidget::hide);

    m_quitWidget->hide();
}

void PuzzleWidget::setNewWidget()
{

    m_newWidget = new QWidget(this);
    m_newWidget->setGeometry(m_parameters.rectWidget);

    QPainterPath backgroundLabelPath = JigsawPath::singleJigsawPiecePath(m_parameters.rectWidget, m_parameters.rectWidgetArea, TypeOfPiece::STANDARD);
    JigsawLabel* backgroundLabel = new JigsawLabel(m_parameters.sizeWidget, QBrush(QPixmap(":/backgrounds/back3")), backgroundLabelPath, m_newWidget,
                                                   "Create a new Jigsaw Puzzle!", m_parameters.rectWidgetArea);
    backgroundLabel->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
    backgroundLabel->setFont(m_parameters.mainFont);

    QWidget* widgetArea = new QWidget(m_newWidget);
    widgetArea->setGeometry(m_parameters.rectWidgetArea);

    QWidget* widgetCaption = new QWidget(widgetArea);
    widgetCaption->setGeometry(m_parameters.rectWidgetCaption);

    setNewWidgetExWidget(widgetArea, m_parameters);
    setNewWidgetPuzzlePieceWidget(widgetArea, m_parameters);
    setNewWidgetNumberOfPiecesWidget(widgetArea, m_parameters);
    setNewWidgetButtonsWidget(widgetArea, m_parameters);

    m_newWidget->hide();
}

void PuzzleWidget::setNewWidgetExWidget(QWidget *parent, const Parameters &par)
{
    QWidget* widgetEx = new QWidget(parent);
    widgetEx->setGeometry(par.rectWidgetEx);
    QGroupBox* radioGroupBoxEx = new QGroupBox(widgetEx);
    radioGroupBoxEx->setGeometry(QRect(QPoint(0, 0), par.sizeWidgetEx));
    QVector<JigsawButton*> labelEx;
    int numberOfEx = 0;
    while (QFile::exists(":/examples/ex" + QString::number(numberOfEx))) {
        ++numberOfEx;
    }
    ++numberOfEx;
    int maxLabelExWidth = (par.widthWidgetEx - (numberOfEx + 1) * par.minBorderWidth) / numberOfEx;
    int maxLabelExHeight = par.heightWidgetEx - par.minBorderWidth * 2 - par.minRadioButtonHeight;
    int labelExWidth, labelExHeight;
    if (maxLabelExHeight * 16 / 9 > maxLabelExWidth) {
        labelExWidth = maxLabelExWidth;
        labelExHeight = labelExWidth * 9 / 16;
    }
    else {
        labelExHeight = maxLabelExHeight;
        labelExWidth = labelExHeight * 16 / 9;
    }
    bool noPreviewEx = labelExWidth < par.minLabelExWidth || labelExHeight < par.minLabelExHeight;
    QSize sizeLabelEx(labelExWidth, labelExHeight);

    int borderExWidth = numberOfEx != 1 ? (par.widthWidgetEx - par.minBorderWidth * 2 - (numberOfEx) * labelExWidth) / (numberOfEx - 1) : 0;
    int borderExHeight = (par.heightWidgetEx - par.minBorderWidth * 2 - labelExHeight - par.minRadioButtonHeight) / 2;

    bool ownImage = false;

    for (int i = 0; i < numberOfEx; ++i) {
        QString filename = ":/examples/ex" + QString::number(i);
        if (!QFile::exists(filename)) ownImage = true;
        labelEx.push_back(new JigsawButton(widgetEx));
        if (noPreviewEx) {
            labelEx[i]->setText(ownImage ? "... or open your own image!" : "Example " + QString::number(i + 1));
        }
        else {
            if (!ownImage) {
                QPixmap preview(filename);
                preview = preview.scaled(sizeLabelEx);
                labelEx[i]->setPixmap(preview);
                labelEx[i]->animate();
            }
            else {
                QPixmap preview(sizeLabelEx);
                QFont font("Georgia", 32, QFont::Bold);
                preview.fill(Qt::darkGray);
                m_ownImageLabel = new JigsawButton(widgetEx);
                m_ownImageLabel->setPixmap(preview);
                m_ownImageLabel->setText("?");
                m_ownImageLabel->setFont(font);
                m_ownImageLabel->animate();
                labelEx[i] = m_ownImageLabel;
                QObject::connect(m_ownImageLabel, &JigsawButton::clicked, this, &PuzzleWidget::newWidgetOwnImageClicked);
            }
        }
        QPoint labelPositionEx(par.minBorderWidth + i * labelExWidth + i * borderExWidth, par.minBorderWidth + borderExHeight);
        labelEx[i]->move(labelPositionEx);

        m_radioButtonEx.push_back(new QRadioButton(radioGroupBoxEx));
        QPoint radioPositionEx = labelPositionEx + QPoint(labelExWidth / 2 - 10, labelExHeight + borderExHeight);
        m_radioButtonEx[i]->move(radioPositionEx);

        QObject::connect(labelEx[i], &JigsawButton::clicked, m_radioButtonEx[i], &QRadioButton::toggle);
    }

    m_radioButtonEx.last()->setCheckable(false);
    m_radioButtonEx[0]->setChecked(true);
}

void PuzzleWidget::setNewWidgetPuzzlePieceWidget(QWidget *parent, const Parameters &par)
{
    QWidget* widgetPuzzlePiece = new QWidget(parent);
    widgetPuzzlePiece->setGeometry(par.rectWidgetPuzzlePiece);
    QGroupBox* radioGroupBoxPuzzlePiece = new QGroupBox(widgetPuzzlePiece);
    radioGroupBoxPuzzlePiece->setGeometry(QRect(QPoint(0, 0), par.sizeWidgetPuzzlePiece));
    QVector<JigsawButton*> labelPuzzlePiece;
    int numberOfTypes = static_cast<int>(TypeOfPiece::count);
    int maxLabelPuzzlePieceWidth = numberOfTypes != 0 ? (par.widthWidgetPuzzlePiece - (numberOfTypes + 1) * par.minBorderWidth) / numberOfTypes : 0;
    int maxLabelPuzzlePieceHeight = par.heightWidgetPuzzlePiece - par.minBorderWidth * 2 - par.minRadioButtonHeight;
    int labelPuzzlePieceWidth, labelPuzzlePieceHeight;
    if (maxLabelPuzzlePieceHeight > maxLabelPuzzlePieceWidth) {
        labelPuzzlePieceHeight = maxLabelPuzzlePieceWidth;
        labelPuzzlePieceWidth = maxLabelPuzzlePieceWidth;
    }
    else {
        labelPuzzlePieceHeight = maxLabelPuzzlePieceHeight;
        labelPuzzlePieceWidth = maxLabelPuzzlePieceHeight;
    }
    bool noPreviewPuzzlePiece = labelPuzzlePieceWidth < par.minLabelPuzzlePieceWidth || labelPuzzlePieceHeight < par.minLabelPuzzlePieceHeight;
    QSize sizeLabelPuzzlePiece(labelPuzzlePieceWidth, labelPuzzlePieceHeight);

    int borderPuzzlePieceWidth = numberOfTypes != 1 ? (par.widthWidgetPuzzlePiece - par.minBorderWidth * 2 - (numberOfTypes) * labelPuzzlePieceWidth) / (numberOfTypes - 1) : 0;
    int borderPuzzlePieceHeight = (par.heightWidgetPuzzlePiece - par.minBorderWidth * 2 - labelPuzzlePieceHeight - par.minRadioButtonHeight) / 2;

    for (int i = 0; i < numberOfTypes; ++i) {
        labelPuzzlePiece.push_back(new JigsawButton(widgetPuzzlePiece));
        if (noPreviewPuzzlePiece) {
            labelPuzzlePiece[i]->setText("Type " + QString::number(i + 1));
        }
        else {
            QPainterPath path = JigsawPath::singleJigsawPiecePath(QRect(QPoint(0, 0), sizeLabelPuzzlePiece), QRect(), JigsawPath::intToTypeOfPiece(i == numberOfTypes - 1 ? 0 : i), 4, true);
            labelPuzzlePiece[i]->setJigsawPath(path, sizeLabelPuzzlePiece, QBrush(Qt::lightGray));
            labelPuzzlePiece[i]->animate();
        }
        QPoint labelPositionPuzzlePiece(par.minBorderWidth + i * labelPuzzlePieceWidth + i * borderPuzzlePieceWidth, par.minBorderWidth + borderPuzzlePieceHeight);
        labelPuzzlePiece[i]->move(labelPositionPuzzlePiece);
        labelPuzzlePiece[i]->setToolTip(JigsawPath::tooltip(JigsawPath::intToTypeOfPiece(i)));

        m_radioButtonPuzzlePiece.push_back(new QRadioButton(radioGroupBoxPuzzlePiece));
        QPoint radioPositionPuzzlePiece = labelPositionPuzzlePiece + QPoint(labelPuzzlePieceWidth / 2 - 10, labelPuzzlePieceHeight + borderPuzzlePieceHeight);
        m_radioButtonPuzzlePiece[i]->move(radioPositionPuzzlePiece);

        QObject::connect(labelPuzzlePiece[i], &JigsawButton::clicked, m_radioButtonPuzzlePiece[i], &QRadioButton::toggle);
    }
    m_radioButtonPuzzlePiece[JigsawPath::typeOfPieceToInt(TypeOfPiece::STANDARD)]->setChecked(true);

    m_radioButtonPuzzlePiece.last()->setCheckable(false);
    QObject::connect(m_radioButtonPuzzlePiece.last(), &QRadioButton::toggled, m_createOwnShapeWidget, &QWidget::show);
    QFont font("Georgia", 32, QFont::Bold);
    labelPuzzlePiece.last()->setText("?");
    labelPuzzlePiece.last()->setTextArea(labelPuzzlePiece.last()->rect());
    labelPuzzlePiece.last()->setFont(font);
}

void PuzzleWidget::setNewWidgetNumberOfPiecesWidget(QWidget *parent, const Parameters &par)
{
    QWidget* widgetNumberOfPieces = new QWidget(parent);
    widgetNumberOfPieces->setGeometry(par.rectWidgetNumberOfPieces);

    QLabel* caption = new QLabel("Number of pieces:", widgetNumberOfPieces);
    caption->setFont(m_parameters.mainFont);
    caption->setGeometry(QRect(QPoint(par.minBorderWidth, par.minBorderWidth), QSize(par.widthWidgetNumberOfPieces - par.minBorderWidth * 2, par.heightWidgetNumberOfPieces / 4 - par.minBorderWidth * 2)));

    QPainterPath path = JigsawPath::singleJigsawPiecePath(QRect(QPoint(0, 0), par.sizeButtonOuterBounds), QRect(), TypeOfPiece::STANDARD, 4, true);

    m_sliderButton = new JigsawSlider(par.sizeButtonOuterBounds, QBrush(QPixmap(":/backgrounds/back2")), path, QBrush(QPixmap(":/backgrounds/back1")), widgetNumberOfPieces, 30, 10, 300);
    m_sliderButton->setGeometry(QRect(QPoint(0, caption->height() + par.minBorderWidth), QSize(par.widthWidgetNumberOfPieces - par.minBorderWidth * 4, par.heightWidgetNumberOfPieces / 8)));
    m_sliderButton->setFont(m_parameters.mainFont);
    m_sliderButton->animate();
}

void PuzzleWidget::setNewWidgetButtonsWidget(QWidget *parent, const Parameters &par)
{
    QWidget* widgetButtons = new QWidget(parent);
    widgetButtons->setGeometry(par.rectWidgetButtons);

    QSize sizeButtonOuterBounds(120, 120);

    QPainterPath pathOkButton = JigsawPath::singleJigsawPiecePath(QRect(QPoint(0, 0), sizeButtonOuterBounds), QRect(), TypeOfPiece::STANDARD, 4, true);
    QPainterPath pathCancelButton = JigsawPath::singleJigsawPiecePath(QRect(QPoint(0, 0), sizeButtonOuterBounds), QRect(), TypeOfPiece::STANDARD, 4, true);

    JigsawButton* okButton = new JigsawButton(sizeButtonOuterBounds, QBrush(QPixmap(":/backgrounds/back2")), pathOkButton, widgetButtons, "Ok");
    okButton->animate();
    okButton->setFont(m_parameters.mainFont);
    okButton->move(par.minBorderWidth, par.minBorderWidth);

    JigsawButton* cancelButton = new JigsawButton(sizeButtonOuterBounds, QBrush(QPixmap(":/backgrounds/back2")), pathCancelButton, widgetButtons, "Cancel");
    cancelButton->animate();
    cancelButton->setFont(m_parameters.mainFont);
    cancelButton->move(par.rectWidgetButtons.right() - par.minBorderWidth - cancelButton->width(), par.minBorderWidth);

    QObject::connect(cancelButton, &JigsawButton::clicked, m_newWidget, &QWidget::hide);
    QObject::connect(okButton, &JigsawButton::clicked, this, &PuzzleWidget::newWidgetOkClicked);
}

void PuzzleWidget::setWonWidget()
{
    m_wonWidget = new QWidget(this);
    QSize sizeOuterBoundsBackground(1000, 600);
    QSize sizeInnerBoundsBackground(sizeOuterBoundsBackground - QSize(400, 400));
    QRect innerBoundsBackground(QPoint(200, 200), sizeInnerBoundsBackground);
    QSize sizeOuterBoundsButtons(130, 100);
    QSize sizeInnerBoundsButtons(70, 60);
    QPoint positionWidget(1920 / 2 - sizeOuterBoundsBackground.width() / 2, 1080 / 2 - sizeOuterBoundsBackground.height() / 2);

    m_wonWidget = new QWidget(this);
    m_wonWidget->setGeometry(QRect(positionWidget, sizeOuterBoundsBackground));

    QPainterPath pathBackgroundLabel = JigsawPath::singleJigsawPiecePath(QRect(QPoint(0, 0), sizeOuterBoundsBackground), QRect(), TypeOfPiece::STANDARD, 4, true);
    JigsawLabel* backgroundLabel = new JigsawLabel(sizeOuterBoundsBackground, QBrush(QPixmap(":/backgrounds/back3")), pathBackgroundLabel, m_wonWidget, "You won!!!", innerBoundsBackground);
    backgroundLabel->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
    backgroundLabel->setFont(m_parameters.mainFont);

    QPainterPath pathOkButton = JigsawPath::singleJigsawPiecePath(QRect(QPoint(0, 0), sizeOuterBoundsButtons), QRect(), TypeOfPiece::STANDARD, 4, true);
    JigsawButton* okButton = new JigsawButton(sizeOuterBoundsButtons, QBrush(QPixmap(":/backgrounds/back2")), pathOkButton, m_wonWidget, "Ok");
    okButton->move(innerBoundsBackground.center() + QPoint(-okButton->width() / 2, okButton->height() / 2));
    okButton->setFont(m_parameters.mainFont);
    okButton->animate();
    QObject::connect(okButton, &JigsawButton::clicked, m_wonWidget, &QWidget::hide);

    m_wonWidget->hide();
}

void PuzzleWidget::setCreateOwnShapeWidget()
{
    m_createOwnShapeWidget = new QWidget(this);
    m_createOwnShapeWidget->hide();
}

PuzzleWidget::PuzzleWidget(QWidget *parent)
    : QWidget{parent}
    , m_background(new QLabel(this))
    , m_movingTimer(new QTimer(this))
    , m_selectedPiece(nullptr)
    , m_selectedMergedPiece(-1)
{
    setDefaultValues();
    m_background->setGeometry(QRect(this->pos(), QSize(1920, 1080)));
    m_background->setScaledContents(true);
    m_background->setPixmap(QPixmap(":/backgrounds/back1"));


    setCreateOwnShapeWidget();
    setMenuWidget();
    setNewWidget();
    setQuitWidget();
    setWonWidget();
}

void PuzzleWidget::loadImage(const QPixmap &image)
{
    m_image = image;
}

void PuzzleWidget::setupPuzzle()
{
    m_grid = new PuzzleGrid(m_rows, m_cols, m_pieceWidth, m_pieceHeight, m_typeOfPiece, this);
    setupImage();
    generatePuzzlePieces();
    placePuzzlePieces();
}

void PuzzleWidget::menuNewButtonClicked()
{
    m_newWidget->show();
    m_newWidget->raise();
}

void PuzzleWidget::menuQuitButtonClicked()
{
    m_quitWidget->show();
    m_quitWidget->raise();
}

void PuzzleWidget::enterMenu()
{
    if (m_menuWidget->pos().x() < 10) {
        m_moveMenuOutTimer->stop();
        m_moveMenuInTimer->start(10);
    }
}

void PuzzleWidget::leaveMenu()
{
    if (m_menuWidget->pos().x() > -120) {
        m_moveMenuOutTimer->start(10);
        m_moveMenuInTimer->stop();
    }
}

void PuzzleWidget::moveMenuInTimerTimeout()
{
    if (m_menuWidget->pos().x() >= 10) {
        m_moveMenuInTimer->stop();
        return;
    }
    m_menuWidget->move(m_menuWidget->pos() + QPoint(2, 0));
}

void PuzzleWidget::moveMenuOutTimerTimeout()
{
    if (m_menuWidget->pos().x() <= -120) {
        m_moveMenuOutTimer->stop();
        return;
    }
    m_menuWidget->move(m_menuWidget->pos() - QPoint(2, 0));
}

void PuzzleWidget::quitWidgetYesClicked()
{
    QApplication::quit();
}

void PuzzleWidget::newWidgetOkClicked()
{
    if (!m_radioButtonEx.last()->isChecked()) {
        QPixmap image;
        for (int i = 0; i < m_radioButtonEx.size(); ++i) {
            if (m_radioButtonEx[i]->isChecked()) image = QPixmap(":/examples/ex" + QString::number(i));
        }
        loadImage(image);
    }
    else {
        loadImage(QPixmap(m_filename));
    }

    for (int i = 0; i < m_radioButtonPuzzlePiece.size(); ++i) {
        if (m_radioButtonPuzzlePiece[i]->isChecked()) m_typeOfPiece = JigsawPath::intToTypeOfPiece(i);
    }

    qDeleteAll(m_puzzlePieces);
    m_puzzlePieces.clear();
    m_mergedPieces.clear();

    m_selectedPiece = nullptr;
    m_selectedMergedPiece = -1;

    calculateRowsAndCols(m_sliderButton->val(), m_image);

    setupPuzzle();
    m_newWidget->lower();
    m_newWidget->hide();
}

void PuzzleWidget::newWidgetOwnImageClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "C:/", "Image Files (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty() && fileName != "" && !QPixmap(fileName).isNull()) {
        m_filename = fileName;
        m_radioButtonEx.last()->setCheckable(true);
        QPixmap preview(fileName);
        preview = preview.scaled(QSize(m_ownImageLabel->width(), m_ownImageLabel->height()));
        m_ownImageLabel->setPixmap(preview);
        m_ownImageLabel->setText("");
        loadImage(QPixmap(fileName));
    }
}

void PuzzleWidget::dragMergedPieces(int id, const QPointF &draggedBy)
{
    int mergedPieceID;
    if (isPartOfMergedPiece(m_puzzlePieces[id], mergedPieceID)) {
        for (const auto &piece : m_mergedPieces[mergedPieceID]) {
            if (piece != m_puzzlePieces[id]) piece->move(piece->originalPosition() + draggedBy);
        }
    }
}

void PuzzleWidget::raisePieces(int id)
{
    int mergedPieceID;
    if (isPartOfMergedPiece(m_puzzlePieces[id], mergedPieceID)) {
        for (const auto &piece : m_mergedPieces[mergedPieceID]) {
            piece->raise();
        }
    }
    else {
        m_puzzlePieces[id]->raise();
    }
}
