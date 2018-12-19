#ifndef BASE_CLASSES_H
#define BASE_CLASSES_H

#include <QWidget>
#include <queue>
#include "interfaces.h"
#include "matrix.h"

using namespace std;
static string source_path = "./../Electroman/source_files";
//static string source_path = "E:/Developing/C++/Projects/Electroman/Electroman/source_files";

class Bullet;
class Unit;

class Object {
protected:
    int m_x;
    int m_y;
    int m_width;
    int m_height;

public:
    int getX();
    int getY();
    void setX(int val);
    void setY(int val);
    int getWidth();
    int getHeight();

    Object(int x, int y);
    virtual ~Object();
};


class Drawable_Object : public virtual Object, public IDrawable {
protected:
    unique_ptr<QPixmap> m_image;

public:
    void draw(QPainter& painter) override;

    Drawable_Object(int x, int y);
    ~Drawable_Object() override;
};

class Animated_Object : public virtual Object, public IAnimated, public IDrawable {
protected:
    vector<unique_ptr<QPixmap>> m_images;
    int m_current_image;
    int m_frame_count;
    int m_period; // how many frames must be passed before next image

public:
    void setPeriod(int val);
    int getPeriod();
    void animate() override;
    void draw(QPainter& painter) override;

    Animated_Object(int x, int y);
    ~Animated_Object() override;
};

class Movable_Object : public virtual Object, public IMovable {
protected:
    int m_dx;
    int m_dy;

public:
    bool checkMapCollision(Matrix& matrix) override;
    bool checkObjectCollision(Object& object) override;
    void move() override;
    void moveBack() override;
    int getDx() override;
    int getDy() override;
    void setDx(int value) override;
    void setDy(int value) override;
    virtual int getDirection();
    virtual void setDirection(int angle);
    virtual int getSpeed();

    Movable_Object(int x, int y, int dx = 0, int dy = 0);
    ~Movable_Object() override;
};

class Shooting_Object : public virtual Object, public IShooting {
protected:
    unique_ptr<Bullet> m_bullet;
    int m_shooting_direction;

public:
    shared_ptr<Bullet> shoot() override;
    int getShootingDirection();
    void setShootingDirection(int val);

    Shooting_Object(int x, int y, int direction);
    ~Shooting_Object() override;
};

class Destroyable_Object : public IDestroyable {
protected:
    unique_ptr<QPixmap> m_destroyed_image;

public:
    shared_ptr<QPixmap> getDestroyedImage() override;

    Destroyable_Object() = default;
    ~Destroyable_Object() override;
};

#endif // BASE_CLASSES_H


