#ifndef CANNON_H
#define CANNON_H

#include "base_classes.h"
#include "unit.h"
#include "bullets.h"

class Cannon : public Unit, public Drawable_Object, public Shooting_Object {
public:
    Cannon(int x, int y, int direction);
    void draw(QPainter& painter) override;
    ~Cannon() override;
};

class Cannon_Manager : public Unit_Manager {
protected:
    shared_ptr<Bullet_Manager> m_bullets;

public:
    void update() override;

    Cannon_Manager(shared_ptr<Bullet_Manager> bullets);
    ~Cannon_Manager() override;
};

#endif // CANNON_H
