#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <QApplication>
#include <QPainter>
#include "base_classes.h"

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

Animated_Object::~Animated_Object() {}
