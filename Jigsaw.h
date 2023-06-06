#ifndef JIGSAW_H
#define JIGSAW_H

#include <QFont>
#include <QPoint>
#include <QSize>
#include <QRect>
#include <random>

namespace Jigsaw {

enum class Direction {
    TOPLEFT,
    TOPRIGHT,
    BOTTOMLEFT,
    BOTTOMRIGHT
};

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

/*
 * The struct Parameters defines some of the default values of the game. In the future, some parameters might be set via
 * the settings button in the menu (doesn't exist yet).
 */

struct Parameters {
    int screenWidth = 1920;
    int screenHeight = 1080;
    int innerOuterBoundsDifference = screenHeight / 5;

    int minCaptionHeight = 50;
    int minLabelExHeight = 100;
    int minLabelExWidth = minLabelExHeight * 16 / 9;
    int minBorderWidth = 10;
    int minRadioButtonHeight = 20;
    int minLabelPuzzlePieceWidth = 100;
    int minLabelPuzzlePieceHeight = minLabelPuzzlePieceWidth;

    int outerWidth = screenWidth;
    int outerHeight = screenHeight;
    int innerWidth = screenWidth - innerOuterBoundsDifference * 2;
    int innerHeight = screenHeight - innerOuterBoundsDifference * 2;
    int freeAreaWidth = screenWidth * 2 / 3;
    int freeAreaHeight = screenHeight;
    int freeAreaAlignment = 1; // 0 = Center, 1 = Left, 2 = Right

    int widthWidgetCaption = innerWidth;
    int widthWidgetEx = innerWidth;
    int widthWidgetOpenFile = innerWidth;
    int widthWidgetPuzzlePiece = innerWidth * 2 / 3;
    int widthWidgetNumberOfPieces = innerWidth / 3;
    int widthWidgetButtons = innerWidth;

    int heightWidgetCaption = innerHeight / 6;
    int heightWidgetEx = innerHeight / 4;
    int heightWidgetOpenFile = innerHeight / 12;
    int heightWidgetPuzzlePiece = innerHeight / 4;
    int heightWidgetNumberOfPieces = heightWidgetPuzzlePiece;
    int heightWidgetButtons = innerHeight / 4;

    QPoint positionWidget = QPoint(screenWidth / 2 - outerWidth / 2, screenHeight / 2 - outerHeight / 2);
    QPoint positionWidgetArea = QPoint(innerOuterBoundsDifference, innerOuterBoundsDifference);
    QPoint positionWidgetCaption = QPoint(0, 0);
    QPoint positionWidgetEx = QPoint(0, heightWidgetCaption);
    QPoint positionWidgetOpenFile = QPoint(0, heightWidgetCaption + heightWidgetEx);
    QPoint positionWidgetPuzzlePiece = QPoint(0, heightWidgetCaption + heightWidgetEx + heightWidgetOpenFile);
    QPoint positionWidgetNumberOfPieces = QPoint(widthWidgetPuzzlePiece, heightWidgetCaption + heightWidgetEx + heightWidgetOpenFile);
    QPoint positionWidgetButtons = QPoint(0, heightWidgetCaption + heightWidgetEx + heightWidgetOpenFile + heightWidgetPuzzlePiece);
    QPoint positionFreeArea = freeAreaAlignment == 0 ? QPoint((screenWidth - freeAreaWidth) / 2, (screenHeight - freeAreaHeight) / 2)
                            : freeAreaAlignment == 1 ? QPoint(0, (screenHeight - freeAreaHeight) / 2)
                                                     : QPoint((screenWidth - freeAreaWidth), (screenHeight - freeAreaHeight) / 2);

    QSize sizeWidget = QSize(outerWidth, outerHeight);
    QSize sizeWidgetArea = QSize(innerWidth, innerHeight);
    QSize sizeWidgetCaption = QSize(widthWidgetCaption, heightWidgetCaption);
    QSize sizeWidgetEx = QSize(widthWidgetEx, heightWidgetEx);
    QSize sizeWidgetOpenFile = QSize(widthWidgetOpenFile, heightWidgetOpenFile);
    QSize sizeWidgetPuzzlePiece = QSize(widthWidgetPuzzlePiece, heightWidgetPuzzlePiece);
    QSize sizeWidgetNumberOfPieces = QSize(widthWidgetNumberOfPieces, heightWidgetNumberOfPieces);
    QSize sizeWidgetButtons = QSize(widthWidgetButtons, heightWidgetButtons);
    QSize sizeFreeArea = QSize(freeAreaWidth, freeAreaHeight);

    int minNumberOfPieces = 1;
    int maxNumberOfPieces = 300;
    QSize sizeButtonOuterBounds = QSize(widthWidgetNumberOfPieces / 4, heightWidgetNumberOfPieces * 3 / 4);
    QSize sizeButtonInnerBounds = QSize(widthWidgetNumberOfPieces / 8, heightWidgetNumberOfPieces / 4);

    QRect rectWidget = QRect(positionWidget, sizeWidget);
    QRect rectWidgetArea = QRect(positionWidgetArea, sizeWidgetArea);
    QRect rectWidgetCaption = QRect(positionWidgetCaption, sizeWidgetCaption);
    QRect rectWidgetEx = QRect(positionWidgetEx, sizeWidgetEx);
    QRect rectWidgetOpenFile = QRect(positionWidgetOpenFile, sizeWidgetOpenFile);
    QRect rectWidgetPuzzlePiece = QRect(positionWidgetPuzzlePiece, sizeWidgetPuzzlePiece);
    QRect rectWidgetNumberOfPieces = QRect(positionWidgetNumberOfPieces, sizeWidgetNumberOfPieces);
    QRect rectWidgetButtons = QRect(positionWidgetButtons, sizeWidgetButtons);
    QRect rectFreeArea = QRect(positionFreeArea, sizeFreeArea);

    QFont mainFont = QFont("Georgia", 16, QFont::Bold);
};

inline int randomNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 g(rd());
    int range = (max - min) + 1;
    int result = (g() % range) + min;
    return result;
}

}



#endif // JIGSAW_H
