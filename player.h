#ifndef PLAYER_H
#define PLAYER_H

#include "bullets.h"
#include "base_classes.h"
#include <queue>
#include <QPainter>
#include "unit.h"

struct Pressed_Buttons
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
    void teleport();

    Player_Animation();
};

class Electro_man : public Movable_Object, public Shooting_Object, public IDrawable, public IInput_Handler, public ILogic {
private:
    int m_overlay;
    unique_ptr<Player_Animation> m_animation;
    shared_ptr<Matrix> m_matrix;
    int m_speed;
    shared_ptr<Pressed_Buttons> m_buttons;

    bool m_is_jump;
    int m_frame_delay;
    shared_ptr<Bullet_Manager> m_bullets;
    void moveBack() override;
    bool checkMapCollision(Matrix& matrix) override;
    bool checkFalling(Matrix& matrix);
    shared_ptr<Bullet> shoot() override;


public:
    void draw(QPainter& painter) override;
    void handleKeys(Pressed_Buttons& buttons) override;
    void teleportation(int destination_x, int destination_y);
    void die();
    void move() override;
    void update() override;




    Electro_man(int x, int y, int speed, shared_ptr<Matrix> matrix, shared_ptr<Bullet_Manager> bullets, shared_ptr<Pressed_Buttons> buttons);
    ~Electro_man() override;
};

#endif // PLAYER_H

