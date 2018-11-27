#ifndef BULLETS_H
#define BULLETS_H

#include <QWidget>
#include "classes.h"

class Bullet : public Object {
protected:
    int direction;
    int speed;

public:
    virtual bool move(int** matrix);
    void draw(QPainter* painter) override = 0;

    Bullet(int x, int y, int direction);
    ~Bullet() override;
};

class Player_Bullet : public Bullet {
protected:
    int current_image;
    std::vector<QPixmap*> images;

public:
    void draw(QPainter* painter) override;

    Player_Bullet(int x, int y, int direction);
    ~Player_Bullet() override;
};

class Robot_Bullet : public Bullet {
protected:
    std::vector<QPixmap*> images;

public:
    void draw(QPainter* painter) override;

    Robot_Bullet(int x, int y, int direction);
    ~Robot_Bullet() override;
};

class Cannon_Bullet : public Bullet {
protected:
    QPixmap* image;

public:
    void draw(QPainter* painter) override;
    bool move(int** matrix) override;

    Cannon_Bullet(int x, int y, int direction);
    ~Cannon_Bullet() override;
};


#endif // BULLETS_H
