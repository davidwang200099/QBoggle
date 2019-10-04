#ifndef BOGGLEWINDOW_H
#define BOGGLEWINDOW_H

#include "Board.h"
#include "Console.h"
//
#include "WordListWidget.h"
#include "lexicon.h"
#include <QMouseEvent>
#include <QMainWindow>

class BoggleWindow : public QMainWindow
{
    Q_OBJECT

public:
    BoggleWindow(QWidget *parent = 0);
    ~BoggleWindow();
    //void mousePressEvent(QMouseEvent *event){}
    //void keyPressEvent(QKeyEvent *event);


    WordListWidget *me;
    WordListWidget *computer;
    Board *board;
    Console *console;
    Lexicon *lex;
    static const int BOGGLE_WINDOW_WIDTH = 800;
    static const int BOGGLE_WINDOW_HEIGHT = 600;
};

#endif // BOGGLEWINDOW_H
