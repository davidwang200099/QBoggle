#include "Cube.h"
#include <QHBoxLayout>
#include "Board.h"
#include "Console.h"
#include <QDebug>
Cube::Cube(QWidget *parent) : QWidget(parent)
{
    label = new QLabel();
    label->setText("");
    label->setAlignment(Qt::AlignCenter);
    QFont font = label->font();
    font.setPointSize(20);
    label->setFont(font);

    //chosen=false;

    this->setFixedSize(77,77 );
    this->setStyleSheet("background-color: white;  border: 2px solid");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);
    setLayout(layout);

    //connect(this,SIGNAL(clicked),this,SLOT(inputLetter));
    //setFocus();
}

void Cube::setLetter(QString l)
{
    label->setText(l);
}

void Cube::highlight(Rank r){
    setStyleSheet("background-color: yellow;border: 2px solid");
    label->setText(label->text()+QString::number(r));
    chosen=true;
}

void Cube::dehighlight(){
    setStyleSheet("background-color: white;border: 2px solid");
    label->setText(*(label->text().begin()));
    chosen=false;
}

void Cube::inputLetter(){
    //if(board->inputLetter(this)) qDebug()<<this->label->text()<<" "<<"is chosen.";
    //else  qDebug()<<this->label->text()<<" "<<"is not chosen.";
    board->inputLetter(this);
}

void Cube::mousePressEvent(QMouseEvent *event){
    if(chosen) return;
    qDebug()<<this->label->text()<<" "<<"is clicked.";
    QCursor cursor;
    cursor.setShape(Qt::PointingHandCursor);
    QApplication::setOverrideCursor(cursor);
    if(!board->playerinput.contains(this)) inputLetter();
    QWidget::mousePressEvent(event);
    event->ignore();
}

void Cube::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event);
    QApplication::restoreOverrideCursor();
    QWidget::mouseReleaseEvent(event);
    event->ignore();
}
