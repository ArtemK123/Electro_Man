#include "cannon.h"
#include "bullets.h"

Cannon::Cannon(int x, int y, int direction)
    : Object(x, y),
      Drawable_Object(x, y),
      Shooting_Object(x, y, direction)
{
    m_solid = false;
    m_bullet = unique_ptr<Cannon_Bullet>(new Cannon_Bullet(0, 0, 0));
    if (direction == 90) {
        m_image = unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/eb_pushka2.png").c_str()));
        m_destroyed_image = unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/eb_pushka3.png").c_str()));
    }
    else if (direction == 270) {
        m_image = unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/eb_pushka1.png").c_str()));
        m_destroyed_image = unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/eb_pushka4.png").c_str()));
    }
}

void Cannon::draw(QPainter& painter) {
    Drawable_Object::draw(painter);
}

Cannon::~Cannon() {

}

Cannon_Manager::Cannon_Manager(shared_ptr<Bullet_Manager> bullets) {
    m_bullets = bullets;
    m_max_frame = 40;
}

void Cannon_Manager::update() {
    for (size_t i = 0; i < m_units.size(); i++) {
        Cannon* unit = dynamic_cast<Cannon*>(m_units[i].get());
        if (m_frame_counts[i] == m_max_frame) {
            m_frame_counts[i] = 0;
            m_bullets->addItem(unit->shoot());
        }
        else {
            m_frame_counts[i]++;
        }
    }
}

Cannon_Manager::~Cannon_Manager() {

}
