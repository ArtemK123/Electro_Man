#ifndef TEXTURES_H
#define TEXTURES_H

#include "base_classes.h"
#include <QPainter>

class Texture : public Object {
protected:
    unique_ptr<QPixmap> image;

public:

    void draw(QPainter& painter) override;

    Texture(int x, int y, QPixmap image);
    ~Texture() override = default;
};

class Animated_Texture : public Animated_Object {
protected:

public:
    unique_ptr<QPixmap> destroy();
    void draw(QPainter& painter) override;
    Animated_Texture(int x, int y, vector<QPixmap> images);
    ~Animated_Texture() override;
};


#endif // TEXTURES_H
