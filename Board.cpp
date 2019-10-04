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

const int Board::delta[8] ={
    -DEFAULT_SIZE-1,-DEFAULT_SIZE,-DEFAULT_SIZE+1,
    -1             ,               1             ,
    DEFAULT_SIZE-1,DEFAULT_SIZE,DEFAULT_SIZE+1
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
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            this->cubes[index(i, j)] = new Cube(this);
            this->cubes[index(i, j)]->chosen=false;
            this->cubes[index(i, j)]->board=this;
            this->cubes[index(i, j)]->index=i*size+j;
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
    else if(event->key()==Qt::Key_Space) autoPlay();
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
     DFS();
}

void Board::DFS(){
    dehighlight_all();
    for(Rank r=0;r<size*size;r++,playerinput.clear()) dfs(r);
}

void Board::dfs(Rank r){
    playerinput.push_back(cubes[r]);
    cubes[r]->highlight(++inputRank);

    QString word;
    for(int i=0;i<8;i++){
        if(LEGAL(r+delta[i])){
            playerinput.push_back(cubes[r+delta[i]]);
            cubes[r+delta[i]]->highlight(++inputRank);

            word=toPrefix().toLower();
            if(!(w->computer->words.contains(word))&&isWord(word)) {
                w->computer->addWord(word);
                w->computer->addScore(1);
                w->computer->scoreLabel->setText(QString::number(w->computer->score));
                dfs(r+delta[i]);
            }else{
                if(w->lex->containsPrefix(word.toStdString()))
                    dfs(r+delta[i]);
            }
        }
    }
    if(!(w->computer->words.contains(word))&&isWord(word)){
        w->computer->addWord(word);
        w->computer->addScore(1);
        w->computer->scoreLabel->setText(QString::number(w->computer->score));
    }else {
        cubes[r]->dehighlight();
        inputRank--;
        playerinput.pop_back();
     }


}



