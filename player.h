#ifndef PLAYER_H
#define PLAYER_H

#include "bullets.h"
#include "base_classes.h"
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

class Player_Animation {
protected:
    int direction;
    shared_ptr<QPixmap> def_image;
    vector<shared_ptr<QPixmap>> images;
    queue<shared_ptr<QPixmap>> queue;
    int current_image;

public:
    shared_ptr<QPixmap> getImage();
    int getDirection();
    void move();
    void rotate();
    void jump();
    void stay();
    void die();

    Player_Animation();
};


class Electro_man : public Object, public I_Movable {
private:
    int dx;
    int dy;
    int speed;
    int overlay;
    bool is_jump;
    int frame_delay;
    shared_ptr<unique_ptr<int[]>[]> matrix;
    unique_ptr<Player_Animation> animation;
    vector<shared_ptr<Bullet>> bullets;
    void shoot();
    void moveBullets();
    bool move(shared_ptr<unique_ptr<int[]>[]>) override;

public:
    vector<shared_ptr<Bullet>>* getBulletsPtr();
    void draw(QPainter& painter) override;
    void handleKeys(shared_ptr<Pressed_Buttoms> buttoms);
    void die();
    void teleportation(int destination_x, int destination_y);

    Electro_man(int x, int y, int speed, shared_ptr<unique_ptr<int[]>[]> matrix);
    ~Electro_man() override = default;
};


#endif // PLAYER_H

