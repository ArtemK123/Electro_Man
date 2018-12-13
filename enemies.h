#ifndef UNITS_H
#define UNITS_H

#include "bullets.h"
#include "base_classes.h"
#include <QPainter>


class Enemy : public Animated_Object {
protected:
    int m_direction;
    bool m_solid;

public:
    bool is_solid();
    virtual unique_ptr<QPixmap> die();
    virtual void update() = 0;
    virtual void shoot() = 0;
    void draw(QPainter& painter) override;

    Enemy(int x, int y);
    ~Enemy() override = default;
};

class Robot : public Enemy, public I_Movable {
protected:
    int m_speed;
    int m_dx;
    shared_ptr<unique_ptr<int[]>[]> m_matrix;
    vector<shared_ptr<Bullet>>* m_bullets;

    void animate() override;
    bool move(shared_ptr<unique_ptr<int[]>[]>) override;
    void shoot() override;

public:
    void update() override;

    Robot(int x, int y, int speed, shared_ptr<unique_ptr<int[]>[]> matrix, vector<shared_ptr<Bullet>>* bullets);
    ~Robot() override = default;
};

class Cannon : public Enemy {
protected:
    vector<shared_ptr<Bullet>>* m_bullets;
    void shoot() override;
    void animate() override;

public:
    void update() override;
    unique_ptr<QPixmap> die() override;


    Cannon(int x, int y, int direction, vector<shared_ptr<Bullet>>* bullets);
    ~Cannon() override = default;
};

#endif // UNITS_H
