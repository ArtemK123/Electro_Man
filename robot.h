#ifndef ROBOT_H
#define ROBOT_H

#include "unit.h"
#include "base_classes.h"
#include "bullets.h"

class Robot : public Unit, public Animated_Object, public Movable_Object, public Shooting_Object {
public:
    void setDirection(int value) override;
    void animate() override;
    bool checkMapCollision(Matrix& matrix) override;
    void draw(QPainter& painter) override;
    shared_ptr<Bullet> shoot() override;

    Robot(int x, int y, int speed);
    ~Robot() override;
};

class Robot_Manager : public Unit_Manager {
protected:
    shared_ptr<Bullet_Manager> m_bullets;
    shared_ptr<Matrix> m_matrix;

public:
    void update() override;

    Robot_Manager(shared_ptr<Matrix> matrix, shared_ptr<Bullet_Manager> bullets);
};

#endif // ROBOT_H
