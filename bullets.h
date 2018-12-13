#ifndef BULLETS_H
#define BULLETS_H

#include <QWidget>
#include "base_classes.h"

class Bullet : public Animated_Object, public I_Movable {
protected:
    int m_direction;
    int m_speed;

public:
    bool move(shared_ptr<unique_ptr<int[]>[]> matrix) override;

    Bullet(int x, int y, int direction);
    ~Bullet() override;
};

class Player_Bullet : public Bullet {
public:
    Player_Bullet(int x, int y, int direction);
    ~Player_Bullet() override = default;
};

class Robot_Bullet : public Bullet {
public:
    void animate() override;
    void draw(QPainter& painter) override;

    Robot_Bullet(int x, int y, int direction);
    ~Robot_Bullet() override = default;
};

class Cannon_Bullet : public Bullet {
public:
    bool move(shared_ptr<unique_ptr<int[]>[]> matrix) override;

    Cannon_Bullet(int x, int y, int direction);
    ~Cannon_Bullet() override = default;
};

#endif // BULLETS_H
