#include "textures.h"

Texture::Texture(int x, int y, QPixmap image) : Object (x, y) {
    this->image = unique_ptr<QPixmap>(new QPixmap(image));
}

void Texture::draw(QPainter& painter) {
    if (image) {
        painter.drawPixmap(x, 384 - height - y, *image);
    }
}

Animated_Texture::Animated_Texture(int x, int y, vector<QPixmap> images)
    : Animated_Object(x, y)
{
    // last image for destroyed texture
    for (QPixmap image : images) {
        this->images.push_back(unique_ptr<QPixmap>(new QPixmap(image)));
    }
}

unique_ptr<QPixmap> Animated_Texture::destroy() {
    if (images.size() > 0) {
        QPixmap image = *images[images.size() - 1];
        return unique_ptr<QPixmap>(new QPixmap(image));
    }
    else {
        return nullptr;
    }
}

void Animated_Texture::draw(QPainter& painter) {
    painter.drawPixmap(x, 384 - height - y, *images[static_cast<unsigned long long>(current_image)]);
    if (frame_count == 10) {
        frame_count = 0;
        if (current_image == static_cast<int>(images.size()) - 2) {
            current_image = 0;
        }
        else {
            current_image++;
        }
    }
    else {
        frame_count++;
    }
}

Animated_Texture::~Animated_Texture() {

}
