#include "Board.h"
#include "Cube.h"
#include "BoggleWindow.h"
#include <QDebug>
const QString Board::STANDARD_CUBES[16]  = {
        "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
        "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
        "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
        "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const QString Board::BIG_BOGGLE_CUBES[25]  = {
        "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
        "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
        "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
        "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
        "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

const int Board::delta_0[5] ={
       -5,-4,
           1,
        5, 6
};

const int Board::delta_4[5] ={
   -6,-5,
   -1,
    4,+5
};

const int Board::delta_default[8]={
   -6,-5,-4,
   -1,    1,
    4, 5, 6
};

Board::Board(QWidget *parent, int size, const QString *cubeLetters) : QWidget(parent)
{
    this->size = size;
    this->inputRank=0;
    this->cubes = new Cube*[size * size];
    this->letters = new QString[size * size];
    for (int i = 0; i < size * size; ++i)
        this->letters[i] = cubeLetters[i];

    QGridLayout *layout = new QGridLayout();
    layout->setMargin(20);
    layout->setSpacing(10);
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
           this->cubes[index(i, j)] = new Cube(this);
           this->cubes[index(i, j)]->chosen=false;
           this->cubes[index(i, j)]->board=this;
           this->cubes[index(i, j)]->index=i*size+j;
            switch (j) {
            case 0:
                for(int k=0;k<5;k++)
                    cubes[index(i,j)]->neibor.push_back(delta_0[k]);
                break;
            case 4:
                for(int k=0;k<5;k++)
                    cubes[index(i,j)]->neibor.push_back(delta_4[k]);
                break;
            default:
                for(int k=0;k<8;k++)
                    cubes[index(i,j)]->neibor.push_back(delta_default[k]);
            }
            layout->addWidget(this->cubes[index(i, j)], i, j, Qt::AlignmentFlag::AlignCenter);
        }
    }
    setLayout(layout);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            this->cubes[index(i, j)]->setLetter(this->letters[index(i, j)].at(rand()%6));
        }
    }
    shake();

    setFocusPolicy(Qt::ClickFocus);
    // this->setStyleSheet("background-color:grey; border: 3px solid");
}

Board::~Board()
{
    if (cubes) delete[] cubes;
    if (letters) delete[] letters;
}

bool Board::legal(int x){
    if(x<0||x>=size*size) return false;
    if(x-size-1>=0)
        {if(cubes[x-size-1]->chosen) return true;}
    if(x-size>=0)
        {if(cubes[x-size]->chosen) return true;}
    if(x-size+1>=0)
        {if(cubes[x-size+1]->chosen) return true;}
    if(x-1>=0)
        {if(cubes[x-1]->chosen) return true;}
    if(x+1<size*size)
        {if(cubes[x+1]->chosen) return true;}
    if(x+size-1<size*size)
        {if(cubes[x+size-1]->chosen) return true;}
    if(x+size<size*size)
        {if(cubes[x+size]->chosen) return true;}
    if(x+size+1<size*size)
        {if(cubes[x+size+1]->chosen) return true;}
    return false;
    /*return ((((x)-size-1>=0)&&cubes[x-size-1]->chosen)||
            (((x)-size>=0)&&cubes[x-size]->chosen)||
            (((x)-size+1>=0)&&cubes[x-size+1]->chosen)||
            (((x)-1>=0)&&cubes[x-1]->chosen)||
            (((x)+1<size*size)&&cubes[x+1]->chosen)||
            (((x)+size-1<size*size)&&cubes[x+size-1]->chosen)||
            (((x)+size<size*size)&&cubes[x+size]->chosen)||
            (((x)+size+1<size*size)&&cubes[x+size+1]->chosen));*/
}

void Board::shake()
{
    srand(time(NULL));
    for(Rank i=0;i<size*size;i++){
        Rank r=rand()%(size*size-i)+i;
        QString s1=cubes[i]->label->text();
        QString s2=cubes[r]->label->text();
        cubes[i]->label->setText(s2);
        cubes[r]->label->setText(s1);
    }
}

void Board::dehighlight_all(){
    for(int i=0;i<size*size;i++){
        cubes[i]->setStyleSheet("background-color: white;  border: 2px solid");
        cubes[i]->chosen=false;
        cubes[i]->label->setText(*(cubes[i]->label->text().begin()));
    }
    inputRank=0;
}

QString Board::toWord(){
    QString word;
    foreach(Cube *c,playerinput)
        word+=*(c->label->text().begin());
    playerinput.clear();
    dehighlight_all();
    return word;
}

QString Board::toPrefix(){
    QString word;
    foreach(Cube *c,playerinput)
        word+=*(c->label->text().begin());
    return word;
}

void Board::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Backspace){
        if(!(playerinput.isEmpty())){
            Cube *c=playerinput.last();
            c->dehighlight();
            inputRank--;
            playerinput.pop_back();
        }
    }
    else if(event->key()== Qt::Key_Return){
        inputRank=0;
        QString word=toWord().toLower();
        if(!isWord(word)) w->console->write("This is not a word.\n");
        else {
            if(!(w->me->words.contains(word))&&isWord(word)){
                w->me->addWord(word);
                w->me->addScore(1);
                w->me->scoreLabel->setText(QString::number(w->me->score));
            }
            else w->console->write("You have found this word before.\n");
            for(int i=0;i<size*size;i++) cubes[i]->chosen=false;
        }
    }
    else if(event->key()==Qt::Key_Space) {
        w->console->write("Playing automatically......\n");
        autoPlay();
    }
    else if(event->key()==Qt::Key_R) {
        dehighlight_all();
        playerinput.clear();
        w->me->words.clear();
        w->me->wordTable->clear();
        w->me->scoreLabel->setText(QString::number(w->me->score=0));
        w->computer->words.clear();
        w->computer->wordTable->clear();
        w->computer->scoreLabel->setText(QString::number(w->computer->score=0));
    }
    QWidget::keyPressEvent(event);
    event->ignore();
}

bool Board::isWord(QString & word){
    return w->lex->contains(word.toStdString());
}

bool Board::inputLetter(Cube *c){
    if(playerinput.isEmpty()) {
        playerinput.push_back(c);
        c->highlight(++inputRank);
        return true;
    }
    //qDebug()<<bool(LEGAL(c->index));
    if(LEGAL(c->index)){
        playerinput.push_back(c);
        c->highlight(++inputRank);
        return true;
    }
    else {
        w->console->write("This cube is near no cubes!\n");
        return false;
    }
}

void Board::autoPlay(){
    connect(&delayTimer,&QTimer::timeout,this,&Board::DFS);
    delayTimer.start(2000);
    //DFS();

}

void Board::DFS(){
    disconnect(&delayTimer,&QTimer::timeout,this,&Board::DFS);
    dehighlight_all();
    playerinput.clear();
    for(Rank r=0;r<size*size;r++) {
        playerinput.push_back(cubes[r]);
        cubes[r]->highlight(++inputRank);
        dfs(r);
        if(!playerinput.isEmpty()) playerinput.clear();
    }
    w->console->write("Auto-play finished......\n");
    //dfs(0);
}

void Board::dfs(Rank r){
    int n=cubes[r]->neibor.size();
    QString word;
    for(int i=0;i<n;i++){
        int delta=cubes[r]->neibor[i];
        if(legal(r+delta)&&!cubes[r+delta]->chosen){
            playerinput.push_back(cubes[r+delta]);
            cubes[r+delta]->highlight(++inputRank);
            //w->console->write(QString::number(cubes[r+delta[i]]->index));
            word=toPrefix().toLower();
            //qDebug()<<word<<"\n";
            if(isWord(word)&&!(w->computer->words.contains(word))) {
                w->computer->addWord(word);
                w->computer->addScore(1);
                w->computer->scoreLabel->setText(QString::number(w->computer->score));
                //qDebug()<<tr("可以递归深入");
                dfs(r+delta);
            }else{
                if(w->lex->containsPrefix(word.toStdString()))
                    //qDebug()<<tr("可以递归深入");
                    dfs(r+delta);
                else {
                    playerinput.last()->dehighlight();
                    playerinput.pop_back();
                    inputRank--;
                }
            }
        }
    }
    /*if(!(w->computer->words.contains(word))&&isWord(word)){
        w->computer->addWord(word);
        w->computer->addScore(1);
        w->computer->scoreLabel->setText(QString::number(w->computer->score));
    }else {*/
        cubes[r]->dehighlight();
        inputRank--;
        playerinput.pop_back();


}



