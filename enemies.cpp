#include "enemies.h"

Enemy::Enemy(int x, int y) : Animated_Object(x, y) {
    this->solid = true;
}

unique_ptr<QPixmap> Enemy::die() {
    return nullptr;
}

bool Enemy::is_solid() {
    return this->solid;
}

void Enemy::draw(QPainter& painter) {
    painter.drawPixmap(x, 384 - y - height, *images[static_cast<unsigned long long>(current_image)]);
}

Robot::Robot(int x, int y, int speed, shared_ptr<unique_ptr<int[]>[]> matrix, vector<shared_ptr<Bullet>>* bullets)
    : Enemy(x, y)
{
    this->matrix = matrix;
    this->direction = 0;
    this->speed = speed;
    this->dx = 0;
    this->bullets = bullets;
    for (int i = 0; i < 12; i++) {
        QPixmap image = QPixmap((source_path + "/sprites/monsters/monster1.png").c_str());
        QPixmap cutted = image.copy(i * width, 0, width, height);
        images.push_back(unique_ptr<QPixmap>(new QPixmap(cutted)));
    }
    for (int i = 0; i < 12; i++) {
        QPixmap image = QPixmap((source_path + "/sprites/monsters/monster0.png").c_str());
        QPixmap cutted = image.copy(i * width, 0, width, height);
        images.push_back(unique_ptr<QPixmap>(new QPixmap(cutted)));
    }
    // 0 - 11 right
    // 12 - 23 left
}

void Robot::update() {
    move(matrix);
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

bool Robot::move(shared_ptr<unique_ptr<int[]>[]>) {
    if (dx > 0) {
        if (x + dx + width >= 13 * 48) {
            direction = 180;
        }
        else if (matrix[x + width + dx][static_cast<unsigned long long>(y - speed)] == 0) {
            direction = 180;
        }
        else if (matrix[x + width + dx][static_cast<unsigned long long>(y)] != 0 ||
                 matrix[x + dx + width][static_cast<unsigned long long>(y + height)] != 0) {
            direction = 180;
        }
        else {
            x += dx;
        }
    }
    if (dx < 0) {
        if (x + dx <= 0) {
             direction = 0;
        }
        else if (matrix[x + dx][static_cast<unsigned long long>(y - speed)] == 0) {
            direction = 0;
        }
        else if (matrix[x + dx][static_cast<unsigned long long>(y)] != 0 ||
                 matrix[x + dx][static_cast<unsigned long long>(y + height)] != 0) {
            direction = 0;
        }
        else {
            x += dx;
        }
    }
    return true;
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
    bullets->push_back(shared_ptr<Bullet>(new Robot_Bullet(x, y, direction)));
}

Cannon::Cannon(int x, int y, int direction, vector<shared_ptr<Bullet>>* bullets)
    : Enemy(x, y)
{
    this->direction = direction;
    this->bullets = bullets;
    this->solid = false;
    for (int i = 1; i <= 4; i++) {
        images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/eb_pushka" + to_string(i) + ".png").c_str())));
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

void Cannon::animate() {
    //not used in this class
}

unique_ptr<QPixmap> Cannon::die() {
    QPixmap image;
    if (direction == 90) {
        image = *images[2];
        images.erase(images.begin() + 2);
        return unique_ptr<QPixmap>(new QPixmap(image));
    }
    else if (direction == 270) {
        image = *images[3];
        images.erase(images.begin() + 3);
        return unique_ptr<QPixmap>(new QPixmap(image));
    }
    else {
        return nullptr;
    }
}

void Cannon::shoot() {
    bullets->push_back(shared_ptr<Bullet>(new Cannon_Bullet(x, y, direction)));
}

