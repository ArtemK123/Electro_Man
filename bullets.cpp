#include "bullets.h"

#include <vector>
#include <QPainter>

Bullet::Bullet(int x, int y, int direction)
    : Animated_Object (x, y)
{
    this->direction = direction;
    this->speed = 20;
}

bool Bullet::move(shared_ptr<unique_ptr<int[]>[]> matrix) {
    if (direction == 0) {
        if (x + width + speed < 624 && matrix[x + width + speed][static_cast<unsigned long long>(y)] == 0) {
            x += speed;
            return true;
        }
    }
    if (direction == 180) {
        if (x - speed > 0 && matrix[x - speed][static_cast<unsigned long>(y)] == 0) {
            x -= speed;
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
    this->width = 32;
    this->height = 8;
    QPixmap image = QPixmap((source_path + "/sprites/weapon1.png").c_str());
    for (int i = 0; i < 8; i++) {
        QPixmap cutted = image.copy(i * width, 0, width, height);
        this->images.push_back(unique_ptr<QPixmap>(new QPixmap(cutted)));
    }
}

Robot_Bullet::Robot_Bullet(int x, int y, int direction) :
    Bullet(x, y, direction)
{
    this->width = 26;
    this->height = 12;
    this->images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/nogi_pulya1.png").c_str())));
    this->images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/nogi_pulya2.png").c_str())));
}

void Robot_Bullet::animate() {
    if (direction == 0) {
        current_image = 0;
    }
    else if (direction == 180) {
        current_image = 1;
    }
}

void Robot_Bullet::draw(QPainter& painter) {
    painter.drawPixmap(x, 384 - y - 48, *images[static_cast<unsigned long long>(current_image)]);
}

Cannon_Bullet::Cannon_Bullet(int x, int y, int direction) :
    Bullet(x, y, direction)
{
    this->width = 48;
    this->height = 12;
    this->images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/eb_pucha1_puli.png").c_str())));
}

bool Cannon_Bullet::move(shared_ptr<unique_ptr<int[]>[]> matrix) {
    if (direction == 90) {
        if (matrix[x + width / 2][static_cast<unsigned long long>(y + height + speed)] == 0) {
            y += speed;
            return true;
        }
    }
    if (direction == 270) {
        if (matrix[x + width / 2][static_cast<unsigned long long>(y - speed)] == 0) {
            y -= speed;
            return true;
        }
    }
    return false;
}
