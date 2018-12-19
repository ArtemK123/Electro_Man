#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <QApplication>
#include <QPainter>
#include "base_classes.h"
#include "bullets.h"

// gameboard = 624 x 384
// texture_block = 48 x 48

Object::Object(int x, int y) {
    this->m_x = x;
    this->m_y = y;
    this->m_width = 48;
    this->m_height = 48;
}

int Object::getX() {
    return this->m_x;
}

void Object::setX(int val) {
    this->m_x = val;
}

int Object::getY() {
    return this->m_y;
}

void Object::setY(int val) {
    this->m_y = val;
}

int Object::getWidth() {
    return this->m_width;
}

int Object::getHeight() {
    return this->m_height;
}

Object::~Object() {

}

Drawable_Object::Drawable_Object(int x, int y) :
    Object(x, y)
{
    m_image = nullptr;
}

void Drawable_Object::draw(QPainter& painter) {
    painter.drawPixmap(m_x, 384 - m_y - m_height, *m_image);
}

Drawable_Object::~Drawable_Object() {

}

Animated_Object::Animated_Object(int x, int y)
    : Object(x, y)
{
    this->m_images = vector<unique_ptr<QPixmap>>();
    this->m_current_image = 0;
    this->m_frame_count = 0;
    this->m_period = 0;
}

void Animated_Object::setPeriod(int val) {
    this->m_period = val;
}

int Animated_Object::getPeriod() {
    return this->m_period;
}

void Animated_Object::animate() {
    if (m_frame_count == m_period) {
        m_current_image = (m_current_image == static_cast<int>(m_images.size()) - 1) ? 0 : m_current_image + 1;
        m_frame_count = 0;
    } else {
        m_frame_count++;
    }
}

void Animated_Object::draw(QPainter& painter) {
    painter.drawPixmap(m_x, 384 - m_height - m_y, *m_images[static_cast<unsigned long long>(m_current_image)]);
    animate();
}

Animated_Object::~Animated_Object() {

}

Movable_Object::Movable_Object(int x, int y, int dx, int dy)
    : Object(x, y)
{
    m_dx = dx;
    m_dy = dy;
}

bool Movable_Object::checkMapCollision(Matrix& matrix) {
    if (    0 > m_x || m_x > matrix.getWidth() ||
            0 > m_y || m_y > matrix.getHeight() ||
            matrix(m_x, m_y) == 1 ||
            matrix(m_x + m_width / 2, m_y) == 1 ||
            matrix(m_x + m_width, m_y) == 1 ||
            matrix(m_x, m_y + m_height / 2) == 1 ||
            matrix(m_x + m_width / 2, m_y + m_height / 2) == 1 ||
            matrix(m_x + m_width, m_y + m_height / 2) == 1 ||
            matrix(m_x, m_y + m_height) == 1 ||
            matrix(m_x + m_width / 2, m_y + m_height) == 1 ||
            matrix(m_x + m_width, m_y + m_height) == 1)
    {
        return true;
    }
    else {
        return false;
    }
}

bool Movable_Object::checkObjectCollision(Object& object) {
    if (    m_x > object.getX() + object.getWidth() ||
            m_x + m_width < object.getX() ||
            m_y > object.getY() + object.getHeight() ||
            m_y + m_height < object.getY())
    {
        return false;
    }
    else {
        return true;
    }
}

void Movable_Object::move() {
    m_x += m_dx;
    m_y += m_dy;
}

void Movable_Object::moveBack() {
    m_x -= m_dx;
    m_y -= m_dy;
}

int Movable_Object::getDx() {
    return m_dx;
}

int Movable_Object::getDy() {
    return m_dy;
}

void Movable_Object::setDx(int value) {
    m_dx = value;
}

void Movable_Object::setDy(int value) {
    m_dy = value;
}

void Movable_Object::setDirection(int angle) {
    int speed = getSpeed();
    switch(angle) {
    case 0: {
        m_dx = speed;
        m_dy = 0;
        break;
    }
    case 90: {
        m_dx = 0;
        m_dy = speed;
        break;
    }
    case 180: {
        m_dx = -speed;
        m_dy = 0;
        break;
    }
    case 270: {
        m_dx = 0;
        m_dy = -speed;
        break;
    }
    }
}

int Movable_Object::getDirection() {
    if (m_dx > 0 && m_dy == 0) {
        return 0;
    }
    else if (m_dx == 0 && m_dy > 0) {
        return 90;
    }
    else if (m_dx < 0 && m_dy == 0) {
        return 180;
    }
    else if (m_dx == 0 && m_dy < 0) {
        return 270;
    }
    else {
        throw string("wrong angle in Movable_Object::getDirection()");
    }
}

int Movable_Object::getSpeed() {
    return max(abs(m_dx), abs(m_dy));
}

Movable_Object::~Movable_Object() {

}

Shooting_Object::Shooting_Object(int x, int y, int direction)
    : Object(x, y)
{
    m_shooting_direction = direction;
}

shared_ptr<Bullet> Shooting_Object::shoot() {
    shared_ptr<Bullet> bullet = m_bullet->fabric();
    bullet->setX(m_x);
    bullet->setY(m_y + m_height / 2);
    bullet->setDirection(m_shooting_direction);
    return bullet;
}

int Shooting_Object::getShootingDirection() {
    return m_shooting_direction;
}

void Shooting_Object::setShootingDirection(int value) {
    if (0 <= value && value < 360) {
        m_shooting_direction = value;
    }
    else {
        throw string("Bad angle for shooting direction");
    }
}

Shooting_Object::~Shooting_Object() {

}

shared_ptr<QPixmap> Destroyable_Object::getDestroyedImage() {
    return shared_ptr<QPixmap>(new QPixmap(*m_destroyed_image));
}

Destroyable_Object::~Destroyable_Object() {

}
