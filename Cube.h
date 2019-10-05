#ifndef CUBE_H
#define CUBE_H

#include <QLabel>
#include <QWidget>

class Cube : public QWidget
{
    Q_OBJECT
public:
    explicit Cube(QWidget *parent = nullptr);
    void setLetter(QString l);

signals:

public slots:

private:
    QLabel *label;
};

#endif // CUBE_H
