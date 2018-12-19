#ifndef TEXTURES_H
#define TEXTURES_H

#include "base_classes.h"
#include <QPainter>

class Texture : public Drawable_Object {
public:
    Texture(int x, int y, QPixmap image);
    void draw(QPainter& painter) override;
    ~Texture() override;
};

class Animated_Texture : public Animated_Object, public Destroyable_Object {
public:
    Animated_Texture(int x, int y, vector<QPixmap> images, QPixmap destroyed_image);
    ~Animated_Texture() override;
};

#endif // TEXTURES_H
