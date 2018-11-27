#ifndef UNITS_H
#define UNITS_H

#include "bullets.h"
#include "classes.h"
#include <QPainter>


class Enemy : public Object {
protected:
    int current_image;
    std::vector<QPixmap*> images;
    int direction;
    bool solid;

public:
    bool is_solid();
    virtual QPixmap* die();
    virtual void update() = 0;
    void draw(QPainter* painter) override;

    Enemy(int x, int y);
    ~Enemy() override;
};

class Robot : public Enemy {
protected:
    int frame_count;
    int speed;
    int dx;
    int** matrix;
    std::vector<Bullet*>* bullets;

    void animate();
    void move();
    void shoot();

public:
    void update() override;

    Robot(int x, int y, int speed, int** matrix, std::vector<Bullet*>* bullets);
    ~Robot() override;
};

class Cannon : public Enemy {
protected:
    int frame_count;
    void shoot();
    std::vector<Bullet*>* bullets;

public:
    void update() override;
    QPixmap* die() override;

    Cannon(int x, int y, int direction, std::vector<Bullet*>* bullets);
    ~Cannon() override;
};

#endif // UNITS_H
