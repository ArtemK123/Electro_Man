#include "bullets.h"

#include <vector>
#include <QPainter>

using namespace std;

Bullet::Bullet(int x, int y, int direction) : Object (x, y) {

    this->direction = direction;
    this->speed = 20;
}

bool Bullet::move(int** matrix) {
    if (direction == 0) {
        if (matrix[x + width + speed][y] == 0) {
            x += speed;
            return true;
        }
    }
    if (direction == 180) {
        if (matrix[x - speed][y] == 0) {
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
    this->current_image = 0;
    images = vector<QPixmap*>();
    QPixmap* image;
    for (int i = 0; i < 8; i++) {
        image = new QPixmap((source_path + "/sprites/weapon1.png").c_str());
        QPixmap cutted = image->copy(i * width, 0, width, height);
        images.push_back(new QPixmap(cutted));
        delete image;
    }
}

void Player_Bullet::draw(QPainter* painter) {
    painter->drawPixmap(x, 384 - y - height, *images[static_cast<unsigned long long>(current_image)]);
    if (current_image == static_cast<int>(images.size()) - 1) {
        current_image = 0;
    }
    else {
        current_image++;
    }
}

Player_Bullet::~Player_Bullet() {
    for (QPixmap* image : images) {
        delete image;
    }
    Bullet::~Bullet();
}

Robot_Bullet::Robot_Bullet(int x, int y, int direction) :
    Bullet(x, y, direction)
{
    this->width = 48;
    this->height = 48;
    this->images = vector<QPixmap*>();
    this->images.push_back(new QPixmap((source_path + "/sprites/monsters/nogi_pulya1.png").c_str()));
    this->images.push_back(new QPixmap((source_path + "/sprites/monsters/nogi_pulya2.png").c_str()));
}

void Robot_Bullet::draw(QPainter* painter) {
    if (direction == 0) {
        painter->drawPixmap(x, 384 - y - height, *images[0]);
    }
    else if (direction == 180) {
        painter->drawPixmap(x, 384 - y - height, *images[1]);
    }
}

Robot_Bullet::~Robot_Bullet() {
    for (QPixmap* image : images) {
        delete image;
    }
    Bullet::~Bullet();
}

Cannon_Bullet::Cannon_Bullet(int x, int y, int direction) :
    Bullet(x, y, direction)
{
    this->width = 48;
    this->height = 12;
    image = new QPixmap((source_path + "/sprites/monsters/eb_pucha1_puli.png").c_str());
}

bool Cannon_Bullet::move(int** matrix) {
    if (direction == 90) {
        if (matrix[x + width / 2][y + height + speed] == 0) {
            y += speed;
            return true;
        }
    }
    if (direction == 270) {
        if (matrix[x + width / 2][y - speed] == 0) {
            y -= speed;
            return true;
        }
    }
    return false;
}

void Cannon_Bullet::draw(QPainter* painter) {
    painter->drawPixmap(x, 384 - y - height, *image);
}

Cannon_Bullet::~Cannon_Bullet() {
    delete image;
    Bullet::~Bullet();
}
