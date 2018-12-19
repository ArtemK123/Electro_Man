#include "robot.h"
#include "bullets.h"
#include <QPainter>

Robot::Robot(int x, int y, int speed)
    : Object(x, y),
      Animated_Object(x, y),
      Movable_Object(x, y, speed),
      Shooting_Object(x, y, 0)
{
    m_shooting_direction = 0;
    m_bullet = unique_ptr<Robot_Bullet>(new Robot_Bullet(0, 0, 0));
    for (int i = 0; i < 12; i++) {
        QPixmap image = QPixmap((source_path + "/sprites/monsters/monster1.png").c_str());
        QPixmap cutted = image.copy(i * m_width, 0, m_width, m_height);
        m_images.push_back(unique_ptr<QPixmap>(new QPixmap(cutted)));
    }
    for (int i = 0; i < 12; i++) {
        QPixmap image = QPixmap((source_path + "/sprites/monsters/monster0.png").c_str());
        QPixmap cutted = image.copy(i * m_width, 0, m_width, m_height);
        m_images.push_back(unique_ptr<QPixmap>(new QPixmap(cutted)));
    }
    // 0 - 11 right
    // 12 - 23 left
}

void Robot::setDirection(int angle) {
    Movable_Object::setDirection(angle);
    m_shooting_direction = angle;
}

bool Robot::checkMapCollision(Matrix& matrix) {
    bool res = Movable_Object::checkMapCollision(matrix);
    if (m_dx > 0) {
        if (matrix(m_x + m_width + m_dx, m_y - 1) == 0) {
            res = true;
        }
    }
    else if (m_dx < 0) {
        if (matrix(m_x + m_dx, m_y - 1) == 0) {
            res = true;
        }
    }
    return res;
}

void Robot::animate() {
    if (getDirection() == 0) {
        if (m_current_image < 11) {
            m_current_image++;
        }
        else {
            m_current_image = 0;
        }
    }
    else if (getDirection() == 180) {
        if (m_current_image > 11 && m_current_image < 23) {
            m_current_image++;
        }
        else {
            m_current_image = 12;
        }
    }
}

shared_ptr<Bullet> Robot::shoot() {
    auto bullet = Shooting_Object::shoot();
    bullet->setY(m_y);
    bullet->setDirection(getDirection());
    return bullet;
}

void Robot::draw(QPainter& painter) {
    painter.drawPixmap(m_x, 384 - m_y - m_height, *m_images[static_cast<size_t>(m_current_image)]);
}

Robot::~Robot() {

}

Robot_Manager::Robot_Manager(shared_ptr<Matrix> matrix, shared_ptr<Bullet_Manager> bullets) {
    m_matrix = matrix;
    m_bullets = bullets;
    m_frame_counts = vector<int>();
    m_max_frame = 40;
};

void Robot_Manager::update() {
    for (size_t i = 0; i < m_units.size(); i++) {
        int frame = m_frame_counts[i];
        Robot* unit = dynamic_cast<Robot*>(m_units[i].get());
        if (frame == m_max_frame) {
            m_bullets->addItem(unit->shoot());
            m_frame_counts[i] = 0;
        }
        else if (frame == m_max_frame - 10) {
            m_frame_counts[i]++;
        }
        else if (frame < m_max_frame - 10){
            m_frame_counts[i]++;
            unit->move();
            if (unit->checkMapCollision(*m_matrix)) {
                unit->moveBack();
                int direction = (unit->getDirection() == 0) ? 180 : 0;
                unit->setDirection(direction);
            }
            unit->animate();
        }
        else {
            m_frame_counts[i]++;
        }
    }
}






