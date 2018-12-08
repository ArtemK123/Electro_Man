#ifndef BASE_CLASSES_H
#define BASE_CLASSES_H

#include <QWidget>
#include <queue>

using namespace std;
static string source_path = "./../Electroman/source_files";
//static string source_path = "E:/Developing/C++/Projects/Electroman/Electroman/source_files";

class Object {
protected:
    int x;
    int y;
    int width;
    int height;
public:
    int getX();
    int getY();
    void setX(int val);
    void setY(int val);
    int getWidth();
    int getHeight();

    virtual void draw(QPainter&) = 0;

    Object(int x, int y);
    virtual ~Object();
};

class I_Animated {
public:
    virtual void animate() = 0;
    virtual void draw(QPainter&) = 0;
    virtual ~I_Animated() = default;
};

class I_Movable {
public:
    virtual bool move(shared_ptr<unique_ptr<int[]>[]>) = 0;
    virtual ~I_Movable() = default;
};

class Animated_Object : public Object, public I_Animated {
protected:
    vector<unique_ptr<QPixmap>> images;
    int current_image;
    int frame_count;
    int period; // how many frames must be passed before next image

public:
    void setPeriod(int val);
    int getPeriod();
    void animate() override;
    void draw(QPainter& painter) override;

    Animated_Object(int x, int y);
    ~Animated_Object() override;
};

#endif // BASE_CLASSES_H


