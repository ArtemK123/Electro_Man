#ifndef BULLETS_H
#define BULLETS_H

#include <QWidget>
#include "base_classes.h"

class Bullet : public Animated_Object, public Movable_Object {
public:
    Bullet(int x, int y, int direction, int speed = 20);
    virtual shared_ptr<Bullet> fabric();
    ~Bullet() override;
};

class Player_Bullet : public Bullet {
public:
    Player_Bullet(int x, int y, int direction, int speed = 20);
    shared_ptr<Bullet> fabric() override;
    ~Player_Bullet() override = default;
};

class Robot_Bullet : public Bullet {
public:
    void animate() override;
    shared_ptr<Bullet> fabric() override;
    void draw(QPainter& painter) override;

    Robot_Bullet(int x, int y, int direction, int speed = 20);
    ~Robot_Bullet() override = default;
};

class Cannon_Bullet : public Bullet {
public:    
    Cannon_Bullet(int x, int y, int direction, int speed = 20);
    shared_ptr<Bullet> fabric() override;
    ~Cannon_Bullet() override = default;
};

class Bullet_Manager : public IObject_Manager<Bullet>, public IDrawable {
protected:
    vector<shared_ptr<Bullet>> m_bullets;

public:
    void update() override;
    void draw(QPainter& painter) override;

    size_t getSize() override;
    shared_ptr<Bullet> getItem(int index) override;
    void addItem(shared_ptr<Bullet> bullet) override;
    void deleteItem(int index) override;
    void clear() override;

    Bullet_Manager();
    ~Bullet_Manager() override;
};

#endif // BULLETS_H
