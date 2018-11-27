#include "enemies.h"

using namespace std;

Enemy::Enemy(int x, int y) : Object(x, y) {
    this->x = x;
    this->y = y;
    this->width = 48;
    this->height = 48;
    this->solid = true;
    this->images = vector<QPixmap*>();
    this->current_image = 0;
}

void Enemy::draw(QPainter* painter) {
    painter->drawPixmap(x, 384 - height - y, *images[static_cast<unsigned long long>(current_image)]);
}

QPixmap* Enemy::die() {
    return nullptr;
}


Enemy::~Enemy() {
    for (QPixmap* image : images) {
        delete image;
    }
}

bool Enemy::is_solid() {
    return this->solid;
}


Robot::Robot(int x, int y, int speed, int** matrix, vector<Bullet*>* bullets) :
    Enemy(x, y)
{
    this->matrix = matrix;
    this->frame_count = 0;
    this->direction = 0;
    this->speed = speed;
    this->dx = 0;
    this->bullets = bullets;
    QPixmap* image;
    for (int i = 0; i < 12; i++) {
        image = new QPixmap((source_path + "/sprites/monsters/monster1.png").c_str());
        QPixmap cutted = image->copy(i * width, 0, width, height);
        images.push_back(new QPixmap(cutted));
        delete image;
    }
    for (int i = 0; i < 12; i++) {
        image = new QPixmap((source_path + "/sprites/monsters/monster0.png").c_str());
        QPixmap cutted = image->copy(i * width, 0, width, height);
        images.push_back(new QPixmap(cutted));
        delete image;
    }
    // 0 - 11 right
    // 12 - 23 left
}

void Robot::update() {
    move();
    if (frame_count == 46) {
        shoot();
        frame_count = 0;
    }
    else if (frame_count == 30) {
        dx = 0;
        frame_count++;
    }
    else if (frame_count < 30){
        frame_count++;
        if (direction == 0) {
            dx = speed;
        }
        else if (direction == 180) {
            dx = -speed;
        }
        animate();
    }
    else {
        frame_count++;
    }
}

void Robot::move() {
    if (dx > 0) {
        if (matrix[x + width + dx][y - speed] == 0) {
            direction = 180;
        }
        else if (matrix[x + width + dx][y] != 0 || matrix[x + dx + width][y + height] != 0) {
            direction = 180;
        }
        else {
            x += dx;
        }
    }
    if (dx < 0) {
        if (matrix[x + dx][y - speed] == 0) {
            direction = 0;
        }
        else if (matrix[x + dx][y] != 0 || matrix[x + dx][y + height] != 0) {
            direction = 0;
        }
        else {
            x += dx;
        }
    }
}

void Robot::animate() {
    if (direction == 0) {
        if (current_image < 11) {
            current_image++;
        }
        else {
            current_image = 0;
        }
    }
    else if (direction == 180) {
        if (current_image > 11 && current_image < 23) {
            current_image++;
        }
        else {
            current_image = 12;
        }
    }
}

void Robot::shoot() {
    bullets->push_back(new Robot_Bullet(x, y, direction));
}

Robot::~Robot() {
    Enemy::~Enemy();
}

Cannon::Cannon(int x, int y, int direction, vector<Bullet*>* bullets) :
    Enemy(x, y)
{
    this->direction = direction;
    this->bullets = bullets;
    this->frame_count = 0;
    this->solid = false;
    for (int i = 1; i <= 4; i++) {
        images.push_back(new QPixmap((source_path + "/sprites/monsters/eb_pushka" + to_string(i) + ".png").c_str()));
    }
    if (direction == 90) {
        current_image = 1;
    }
    else if (direction == 270) {
        current_image = 0;
    }
}

void Cannon::update() {
    if (frame_count == 40) {
        shoot();
        frame_count = 0;
    }
    else {
        frame_count++;
    }
}

QPixmap* Cannon::die() {

    if (direction == 90) {
        return new QPixmap(*images[2]);
    }
    else if (direction == 270) {
        return new QPixmap(*images[3]);
    }
    else {
        return nullptr;
    }
}

void Cannon::shoot() {
    bullets->push_back(new Cannon_Bullet(x, y, direction));
}

Cannon::~Cannon() {
    Enemy::~Enemy();
}

