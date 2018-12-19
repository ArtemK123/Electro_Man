#include "textures.h"

Texture::Texture(int x, int y, QPixmap image)
    : Object(x, y),
      Drawable_Object (x, y)
{
    this->m_image = unique_ptr<QPixmap>(new QPixmap(image));
}

void Texture::draw(QPainter& painter) {
    painter.drawPixmap(m_x, 384 - m_y - m_width, *m_image);
}

Texture::~Texture() {

}

Animated_Texture::Animated_Texture(int x, int y, vector<QPixmap> images, QPixmap destroyed_image)
    : Object(x, y),
      Animated_Object(x, y),
      Destroyable_Object()
{
    for (QPixmap image : images) {
        m_images.push_back(unique_ptr<QPixmap>(new QPixmap(image)));
    }
    m_destroyed_image = unique_ptr<QPixmap>(new QPixmap(destroyed_image));
}

Animated_Texture::~Animated_Texture() {

}
