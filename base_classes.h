#ifndef CLASSES_H
#define CLASSES_H

#include <QWidget>
#include <queue>

static std::string source_path = "E:/Developing/C++/Projects/Electroman/Electroman/source_files";


class Object {
protected:
    int width;
    int height;
public:
    int x;
    int y;

    int getWidth();
    int getHeight();
    virtual void draw(QPainter*) = 0;

    Object(int x, int y);
    virtual ~Object();
};

class Texture : public Object {
protected:
    QPixmap* image;

public:

    void draw(QPainter* painter) override;
    Texture(int x, int y, QPixmap* image);
    ~Texture() override;
};

class Ammo : public Object {
protected:
    QPixmap* image;
    int frame_count;

public:
    void draw(QPainter* painter) override;

    Ammo(int x, int y);
    ~Ammo() override;
};


























/*class Animated_Object {
public:
    std::vector<QPixmap*> animations;
    int animation_max;
    int current_animation;
    bool is_active;

    virtual void animate();
    virtual void draw();
    bool addImage(QPixmap* pixmap_ptr);
    void clearImages();

    Animated_Object(int x, int y, int animation_max);

    virtual ~Animated_Object();
};


class Texture {
public:
    int x;
    int y;
    bool is_solid;
    QPixmap* image;

    virtual void draw();
    Texture(int x, int y, QPixmap* image_ptr, bool is_solid);
    virtual ~Texture();
};

class Animated_Texture : public Animated_Object, public Texture {
    void draw() override;
};*/



/*class Object {
public:
    int x, y, width, height;

    virtual void draw();

    Object(int x, int y, int width, int height);
    virtual ~Object();
};

class Animated_Object : virtual public Object {
public:
    std::vector<QPixmap*> animations;
    int animation_max;
    int current_animation;
    bool is_active;

    virtual void animate();
    void draw() override;
    bool addImage(QPixmap* pixmap_ptr);
    void clearImages();

    Animated_Object(int x, int y, int animation_max, int width, int height, bool is_active);

    ~Animated_Object() override;
};

class Texture : public Object {
public:
    bool is_solid;
    QPixmap* image;

    void draw() override;

    Texture(int x, int y, QPixmap* image, int width, int height, bool is_solid);
    Texture(int x, int y, char* path, int width, int height, bool is_solid);

    ~Texture() override;
};

class Animated_Texture : public Animated_Object {
public:
    bool is_solid;
    void draw() override;

    Animated_Texture(int x, int y, int animation_max, int width, int height, bool is_solid, bool is_active);

    ~Animated_Texture() override;
};

class Destroyable_Texture : public Animated_Texture {
public:
    void destroy();

    Destroyable_Texture(int x, int y, int animation_max, int width, int height, bool is_solid, bool is_active);

    ~Destroyable_Texture() override;
};

class Movable_Object : virtual public Object {
public:
    int speed;
    int direction;

    virtual void move();

    Movable_Object(int x, int y, int max_speed, int width, int height);
    ~Movable_Object();
};

class Bullet : public Animated_Object, public Movable_Object {
public:
    bool is_fly_through;
    int max_distance;
    int current_distance;

    void move () override;

    Bullet(int x, int y, int animation_max, int max_speed, int max_distance, int width, int height, bool is_fly_through);
    ~Bullet() override;
};

class Unit : public Animated_Object {
public:
    virtual void shoot() {

    }

    Unit(int x, int y, int animation_max, int width, int height);
    ~Unit() override;
};

class Cannon : public Unit {
    int direction;
    void shoot() override;
    void destroy();

    Cannon(int x, int y, int direction, int animation_max, int width, int height);
    ~Cannon() override;
};

class Turrel : public Cannon, public Movable_Object {

    Turrel(int x, int y, int speed, int animation_max);
    ~Turrel() override;
};*/

#endif // CLASSES_H
