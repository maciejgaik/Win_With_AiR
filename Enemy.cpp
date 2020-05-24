#include "Enemy.h"
#include "Game.h"

//globalny obiekt
extern Game *game;

Enemy::Enemy(int interval, QString n, int e, bool s) : Course(n, e, s){
    //Ustawienie rozmiaru
    setRect(0,0,160,100);
    int modulo = 450;
    setX(rand()%modulo);
    //setX(300);

    text = new QGraphicsTextItem(n);
    text->setPos(x()+(rect().width() - text->boundingRect().width())/2,y()+(rect().height() - text->boundingRect().height())/2);
    text->setDefaultTextColor(Qt::black);

    t_ects = new QGraphicsTextItem(QString::number(e));
    t_ects->setPos(x()+(rect().width() - t_ects->boundingRect().width())/2,rect().height()-20);
    QColor colorList[] = {Qt::red, Qt::blue, Qt::green, Qt::cyan, Qt::darkMagenta, Qt::darkYellow, Qt::darkRed, Qt::darkBlue, Qt::darkCyan, Qt::darkGreen};
    t_ects->setDefaultTextColor(colorList[rand()%(sizeof(colorList)/sizeof(colorList[0]))]);


    //Tworzenie timera i laczenie go przez timeout z move
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(interval);
}

//Ruch wroga
void Enemy::move(){
    setPos(x(),y()+5);
    text->setPos(text->x(),text->y()+5);
    t_ects->setPos(t_ects->x(),t_ects->y()+5);
    //Znalezienie colidujacych obiektow
    QList<QGraphicsItem *> colliding_item = collidingItems();
    for(int i = 0, n = colliding_item.size(); i < n; ++i){
        if(typeid(*(colliding_item[i])) == typeid(Player)){
            game->score->increase(getEcts());
            if(getStatus()){
                game->deficit->decrease(-getEcts());
            }
            changeStatus(false);
            scene()->removeItem(this->text);
            scene()->removeItem(this->t_ects);
            scene()->removeItem(this);
            game->enemyList.removeAt(game->enemyList.indexOf(this));
            delete this;
            return;
        }
    }

    //Jesli bloczek nie zostal zlapany to naliczany jest deficyt
    if(pos().y() > scene()->height()){
        if(!getStatus()){
            game->deficit->decrease(getEcts());
        }
        game->courseList.insert(game->pEnemy+2, new Course(getName(), getEcts(), true));
        scene()->removeItem(this->text);
        scene()->removeItem(this->t_ects);
        scene()->removeItem(this);
        game->enemyList.removeAt(game->enemyList.indexOf(this));
        delete this;
        return;
    }
}

void Enemy::faster(){
    timer->setInterval(0.9*timer->interval());
}

void Enemy::slower(){
    timer->setInterval(timer->interval()/0.9);
}

