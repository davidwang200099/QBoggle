#ifndef CUBE_H
#define CUBE_H

#include <QLabel>
#include <QWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QApplication>
#include <iostream>
#include <vector>

class Board;
class Console;
#include "BoggleWindow.h"
class Cube : public QWidget
{
    friend class Board;
    Q_OBJECT
public:
    explicit Cube(QWidget *parent = nullptr);
    void setLetter(QString l);
    void highlight(Rank r);
    void dehighlight();
    void inputLetter();
    //bool highlighted() const {return chosen;}
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //void keyPressEvent(QKeyEvent *event);
public:
    QLabel *label;
    Board *board;
    bool chosen;
    Console *console;
    Rank index;
    std::vector<Rank> neibor;
};

#endif // CUBE_H
