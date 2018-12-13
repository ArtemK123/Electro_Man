#include "textures.h"

Texture::Texture(int x, int y, QPixmap image) : Object (x, y) {
    this->m_image = unique_ptr<QPixmap>(new QPixmap(image));
}

void Texture::draw(QPainter& painter) {
    if (m_image) {
        painter.drawPixmap(m_x, 384 - m_height - m_y, *m_image);
    }
}

Animated_Texture::Animated_Texture(int x, int y, vector<QPixmap> images)
    : Animated_Object(x, y)
{
    // last image for destroyed texture
    for (QPixmap image : images) {
        this->m_images.push_back(unique_ptr<QPixmap>(new QPixmap(image)));
    }
}

unique_ptr<QPixmap> Animated_Texture::destroy() {
    if (m_images.size() > 0) {
        QPixmap image = *m_images[m_images.size() - 1];
        return unique_ptr<QPixmap>(new QPixmap(image));
    }
    else {
        return nullptr;
    }
}

void Animated_Texture::draw(QPainter& painter) {
    painter.drawPixmap(m_x, 384 - m_height - m_y, *m_images[static_cast<unsigned long long>(m_current_image)]);
    if (m_frame_count == 10) {
        m_frame_count = 0;
        if (m_current_image == static_cast<int>(m_images.size()) - 2) {
            m_current_image = 0;
        }
        else {
            m_current_image++;
        }
    }
    else {
        m_frame_count++;
    }
}

Animated_Texture::~Animated_Texture() {

}
