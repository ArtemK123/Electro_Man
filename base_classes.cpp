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
    this->x = x;
    this->y = y;
    this->width = 48;
    this->height = 48;
}

int Object::getX() {
    return this->x;
}

void Object::setX(int val) {
    this->x = val;
}

int Object::getY() {
    return this->y;
}

void Object::setY(int val) {
    this->y = val;
}

int Object::getWidth() {
    return this->width;
}

int Object::getHeight() {
    return this->height;
}

Object::~Object() {

}

Animated_Object::Animated_Object(int x, int y)
    : Object(x, y)
{
    this->images = vector<unique_ptr<QPixmap>>();
    this->current_image = 0;
    this->frame_count = 0;
    this->period = 0;
}

void Animated_Object::setPeriod(int val) {
    this->period = val;
}

int Animated_Object::getPeriod() {
    return this->period;
}

void Animated_Object::animate() {
    if (frame_count == period) {
        current_image = (current_image == static_cast<int>(images.size()) - 1) ? 0 : current_image + 1;
        frame_count = 0;
    } else {
        frame_count++;
    }
}

void Animated_Object::draw(QPainter& painter) {
    painter.drawPixmap(x, 384 - height - y, *images[static_cast<unsigned long long>(current_image)]);
    animate();
}

Animated_Object::~Animated_Object() {}
