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
    int m_direction;
    shared_ptr<QPixmap> m_def_image;
    vector<shared_ptr<QPixmap>> m_images;
    queue<shared_ptr<QPixmap>> m_queue;
    int m_current_image;

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
    int m_dx;
    int m_dy;
    int m_speed;
    int m_overlay;
    bool m_is_jump;
    int m_frame_delay;
    shared_ptr<unique_ptr<int[]>[]> m_matrix;
    unique_ptr<Player_Animation> m_animation;
    vector<shared_ptr<Bullet>> m_bullets;
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

