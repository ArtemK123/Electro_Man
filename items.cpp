#include "items.h"

Ammo::Ammo(int x, int y) : Animated_Object(x, y) {
    this->m_images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/level/energy.png").c_str())));
    this->m_period = 10;
}

Ammo::~Ammo() {

}

Teleport::Teleport(int x, int y, int destination_x, int destination_y)
    : Animated_Object(x, y)
{
    this->m_destination_x = destination_x;
    this->m_destination_y = destination_y;
    this->m_images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/level/portal1.png").c_str())));
    this->m_images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/level/portal2.png").c_str())));
    this->m_period = 10;
};

int Teleport::getDestinationX() {
    return this->m_destination_x;
}

int Teleport::getDestinationY() {
    return this->m_destination_y;
}

void Teleport::setDestionation(int x, int y) {
    if (x >= 0 && x <= 13 * 48 && y >= 0 && y <= 8 * 48) {
        this->m_destination_x = x;
        this->m_destination_y = y;
    }
}

Teleport::~Teleport() {

}
