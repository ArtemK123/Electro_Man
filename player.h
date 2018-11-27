#ifndef PLAYER_H
#define PLAYER_H

#include "bullets.h"
#include "classes.h"
#include <queue>
#include <QPainter>

struct Pressed_Buttoms
{
    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;
    bool fire = false;
};

class Animation_Handler {
public:
    int direction;
    QPixmap* def_image;
    std::vector<QPixmap*> images;
    std::queue<QPixmap*> queue;

    int current_image;

    QPixmap* getImage();
    void move();
    void rotate();
    void jump();
    void stay();
    void die();

    Animation_Handler();
    ~Animation_Handler();
};


class Electro_man : public Object {
private:
    int dx;
    int dy;
    int speed;
    int overlay;
    bool is_jump;
    int rotation_step;
    int** matrix;
    Animation_Handler* animation;
    std::vector<Bullet*> bullets;
    void shoot();
    void move();
    void moveBullets();

public:
    std::vector<Bullet*>* getBulletsPtr();
    void draw(QPainter* painter) override;
    void handleKeys(Pressed_Buttoms* buttoms);
    void die();

    Electro_man(int x, int y, int speed, int** matrix);
    ~Electro_man() override;
};


#endif // PLAYER_H
