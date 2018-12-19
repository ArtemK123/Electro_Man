#include "bullets.h"

#include <vector>
#include <QPainter>

Bullet::Bullet(int x, int y, int direction, int speed)
   : Object(x, y),
     Animated_Object (x, y),
     Movable_Object (x, y, speed)
{
    setDirection(direction);
}

shared_ptr<Bullet> Bullet::fabric() {
    return shared_ptr<Bullet>(new Bullet(m_x, m_y, getDirection(), getSpeed()));
}

Bullet::~Bullet() {

}

Player_Bullet::Player_Bullet(int x, int y, int direction, int speed)
  : Object(x, y),
    Bullet(x, y, direction, speed)
{
    this->m_width = 32;
    this->m_height = 8;
    QPixmap image = QPixmap((source_path + "/sprites/weapon1.png").c_str());
    for (int i = 0; i < 8; i++) {
        QPixmap cutted = image.copy(i * m_width, 0, m_width, m_height);
        this->m_images.push_back(unique_ptr<QPixmap>(new QPixmap(cutted)));
    }
}

shared_ptr<Bullet> Player_Bullet::fabric() {
    return shared_ptr<Bullet>(new Player_Bullet(m_x, m_y, getDirection()));
}

Robot_Bullet::Robot_Bullet(int x, int y, int direction, int speed)
  : Object(x, y),
    Bullet(x, y, direction, speed)
{
    this->m_width = 26;
    this->m_height = 12;
    this->m_images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/nogi_pulya1.png").c_str())));
    this->m_images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/nogi_pulya2.png").c_str())));
}

shared_ptr<Bullet> Robot_Bullet::fabric() {
    return shared_ptr<Bullet>(new Robot_Bullet(m_x, m_y, getDirection()));
}

void Robot_Bullet::animate() {
    if (getDirection() == 0) {
        m_current_image = 0;
    }
    else if (getDirection() == 180) {
        m_current_image = 1;
    }
}

void Robot_Bullet::draw(QPainter& painter) {
    painter.drawPixmap(m_x, 384 - m_y - 48, *m_images[static_cast<unsigned long long>(m_current_image)]);
    animate();
}

Cannon_Bullet::Cannon_Bullet(int x, int y, int direction, int speed)
  : Object(x, y),
    Bullet(x, y, direction, speed)
{
    this->m_width = 48;
    this->m_height = 12;
    this->m_images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/eb_pucha1_puli.png").c_str())));
}

shared_ptr<Bullet> Cannon_Bullet::fabric() {
    return shared_ptr<Bullet>(new Cannon_Bullet(m_x, m_y, getDirection(), getSpeed()));
}

Bullet_Manager::Bullet_Manager() {
    m_bullets = vector<shared_ptr<Bullet>>();
}

void Bullet_Manager::update() {
    for (auto bullet : m_bullets) {
        bullet->move();
    }
}

shared_ptr<Bullet> Bullet_Manager::getItem(int index) {
    if (index < static_cast<int>(m_bullets.size())) {
        return m_bullets[static_cast<size_t>(index)];
    }
    else {
        return nullptr;
    }
}

void Bullet_Manager::addItem(shared_ptr<Bullet> ptr) {
    for (auto unit : m_bullets) {
        if (unit == ptr) {
            return;
        }
    }
    m_bullets.push_back(ptr);
}

void Bullet_Manager::deleteItem(int index) {
    if (index < static_cast<int>(m_bullets.size())) {
        m_bullets.erase(m_bullets.begin() + index);
    }
}

size_t Bullet_Manager::getSize() {
    return m_bullets.size();
}

void Bullet_Manager::clear() {
    m_bullets.clear();
}

void Bullet_Manager::draw(QPainter& painter) {
    for (auto unit : m_bullets) {
        unit->draw(painter);
    }
}

Bullet_Manager::~Bullet_Manager() {

}
