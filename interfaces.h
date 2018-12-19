#ifndef INTERFACES_H
#define INTERFACES_H

#include <QWidget>

using namespace std;

class Matrix;
class Object;
class Bullet;
class Unit;
struct Pressed_Buttons;

class IMovable {
public:
    virtual void move() = 0;
    virtual void moveBack() = 0;
    virtual int getDx() = 0;
    virtual int getDy() = 0;
    virtual void setDx(int val) = 0;
    virtual void setDy(int val) = 0;
    virtual bool checkMapCollision(Matrix&) = 0;
    virtual bool checkObjectCollision(Object&) = 0;
    virtual ~IMovable() = default;
};

class IDrawable {
public:
    virtual void draw(QPainter&) = 0;
    virtual ~IDrawable() = default;
};

class IAnimated {
public:
    virtual void animate() = 0;
    virtual ~IAnimated() = default;
};

class IDestroyable {
public:
    virtual shared_ptr<QPixmap> getDestroyedImage() = 0;
    virtual ~IDestroyable() = default;
};

class IShooting {
public:
    virtual shared_ptr<Bullet> shoot() = 0;
    virtual ~IShooting() = default;
};

class ILogic {
public:
    virtual void update() = 0;
    virtual ~ILogic() = default;
};

template<class T> class IObject_Manager : public ILogic {
public:
    virtual shared_ptr<T> getItem(int index) = 0;
    virtual void addItem(shared_ptr<T> ptr) = 0;
    virtual void deleteItem(int index) = 0;
    virtual size_t getSize() = 0;
    virtual void clear() = 0;
    virtual void update() = 0;
};

class IInput_Handler {
public:
    virtual void handleKeys(Pressed_Buttons& buttoms) = 0;
    virtual ~IInput_Handler() = default;
};

#endif // INTERFACES_H
