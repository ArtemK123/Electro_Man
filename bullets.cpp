#include "bullets.h"

#include <vector>
#include <QPainter>

Bullet::Bullet(int x, int y, int direction)
    : Animated_Object (x, y)
{
    this->m_direction = direction;
    this->m_speed = 20;
}

bool Bullet::move(shared_ptr<unique_ptr<int[]>[]> matrix) {
    if (m_direction == 0) {
        if (m_x + m_width + m_speed < 624 && matrix[m_x + m_width + m_speed][static_cast<size_t>(m_y)] == 0) {
            m_x += m_speed;
            return true;
        }
    }
    if (m_direction == 180) {
        if (m_x - m_speed > 0 && matrix[m_x - m_speed][static_cast<size_t>(m_y)] == 0) {
            m_x -= m_speed;
            return true;
        }
    }
    return false;
}

Bullet::~Bullet() {

}

Player_Bullet::Player_Bullet(int x, int y, int direction) :
    Bullet(x, y, direction)
{
    this->m_width = 32;
    this->m_height = 8;
    QPixmap image = QPixmap((source_path + "/sprites/weapon1.png").c_str());
    for (int i = 0; i < 8; i++) {
        QPixmap cutted = image.copy(i * m_width, 0, m_width, m_height);
        this->m_images.push_back(unique_ptr<QPixmap>(new QPixmap(cutted)));
    }
}

Robot_Bullet::Robot_Bullet(int x, int y, int direction) :
    Bullet(x, y, direction)
{
    this->m_width = 26;
    this->m_height = 12;
    this->m_images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/nogi_pulya1.png").c_str())));
    this->m_images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/nogi_pulya2.png").c_str())));
}

void Robot_Bullet::animate() {
    if (m_direction == 0) {
        m_current_image = 0;
    }
    else if (m_direction == 180) {
        m_current_image = 1;
    }
}

void Robot_Bullet::draw(QPainter& painter) {
    painter.drawPixmap(m_x, 384 - m_y - 48, *m_images[static_cast<unsigned long long>(m_current_image)]);
}

Cannon_Bullet::Cannon_Bullet(int x, int y, int direction) :
    Bullet(x, y, direction)
{
    this->m_width = 48;
    this->m_height = 12;
    this->m_images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/eb_pucha1_puli.png").c_str())));
}

bool Cannon_Bullet::move(shared_ptr<unique_ptr<int[]>[]> matrix) {
    if (m_direction == 90) {
        if (matrix[m_x + m_width / 2][static_cast<size_t>(m_y + m_height + m_speed)] == 0) {
            m_y += m_speed;
            return true;
        }
    }
    if (m_direction == 270) {
        if (matrix[m_x + m_width / 2][static_cast<size_t>(m_y - m_speed)] == 0) {
            m_y -= m_speed;
            return true;
        }
    }
    return false;
}
