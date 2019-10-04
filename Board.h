#ifndef BOARD_H
#define BOARD_H


#include <QWidget>
#include <QGridLayout>
#include <QList>
#include <string>
#include <cstdlib>
#include <ctime>
#include <QTimer>
using namespace std;
#define DEFAULT_SIZE 5

#define LEGAL(x) ((((x)>=0)&&((x)<=size*size))&& \
                 ((((x)-size-1>=0)&&cubes[x-size-1]->chosen)|| \
                 (((x)-size>=0)&&cubes[x-size]->chosen)|| \
                 (((x)-size+1>=0)&&cubes[x-size+1]->chosen)|| \
                 (((x)-1>=0)&&cubes[x-1]->chosen)|| \
                 (((x)+1<size*size)&&cubes[x+1]->chosen)|| \
                 (((x)+size-1<size*size)&&cubes[x+size-1]->chosen)|| \
                 (((x)+size<size*size)&&cubes[x+size]->chosen)|| \
                 (((x)+size+1<size*size)&&cubes[x+size+1]->chosen)))


#ifndef Rank
#define Rank int
#endif



class Cube;
class BoggleWindow;
class Board : public QWidget
{
    friend class Cube;
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr, int size = DEFAULT_SIZE, const QString *cubeLetters = BIG_BOGGLE_CUBES);
    virtual ~Board();
    void shake();
    QString toWord();
    QString toPrefix();
    void keyPressEvent(QKeyEvent *event);
    bool isWord(QString &word);
    bool inputLetter(Cube *c);
    void dehighlight_all();
    void autoPlay();
    void DFS();
    void dfs(Rank r);
    bool legal(Rank r);
    //bool installEventFilter(QWidget *w);

    int size;
    int inputRank;
    Cube **cubes;
    QString *letters;
    inline int index(int i, int j) const { return i * size + j; }
    static const QString STANDARD_CUBES[16];
    static const QString BIG_BOGGLE_CUBES[25];
    static const int delta_0[5];
    static const int delta_4[5];
    static const int delta_default[8];
    QList<Cube *> playerinput;
    BoggleWindow *w;
    QTimer delayTimer;
    //QString inputWord;
};

#endif // BOARD_H
