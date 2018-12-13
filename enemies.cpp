#include "enemies.h"

Enemy::Enemy(int x, int y) : Animated_Object(x, y) {
    this->m_solid = true;
}

unique_ptr<QPixmap> Enemy::die() {
    return nullptr;
}

bool Enemy::is_solid() {
    return this->m_solid;
}

void Enemy::draw(QPainter& painter) {
    painter.drawPixmap(m_x, 384 - m_y - m_height, *m_images[static_cast<size_t>(m_current_image)]);
}

Robot::Robot(int x, int y, int speed, shared_ptr<unique_ptr<int[]>[]> matrix, vector<shared_ptr<Bullet>>* bullets)
    : Enemy(x, y)
{
    this->m_matrix = matrix;
    this->m_direction = 0;
    this->m_speed = speed;
    this->m_dx = 0;
    this->m_bullets = bullets;
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

void Robot::update() {
    move(m_matrix);
    if (m_frame_count == 46) {
        shoot();
        m_frame_count = 0;
    }
    else if (m_frame_count == 30) {
        m_dx = 0;
        m_frame_count++;
    }
    else if (m_frame_count < 30){
        m_frame_count++;
        if (m_direction == 0) {
            m_dx = m_speed;
        }
        else if (m_direction == 180) {
            m_dx = -m_speed;
        }
        animate();
    }
    else {
        m_frame_count++;
    }
}

bool Robot::move(shared_ptr<unique_ptr<int[]>[]>) {
    if (m_dx > 0) {
        if (m_x + m_dx + m_width >= 13 * 48) {
            m_direction = 180;
        }
        else if (m_matrix[m_x + m_width + m_dx][static_cast<size_t>(m_y - m_speed)] == 0) {
            m_direction = 180;
        }
        else if (m_matrix[m_x + m_width + m_dx][static_cast<size_t>(m_y)] != 0 ||
                 m_matrix[m_x + m_dx + m_width][static_cast<size_t>(m_y + m_height)] != 0)
        {
            m_direction = 180;
        }
        else {
            m_x += m_dx;
        }
    }
    if (m_dx < 0) {
        if (m_x + m_dx <= 0) {
             m_direction = 0;
        }
        else if (m_matrix[m_x + m_dx][static_cast<size_t>(m_y - m_speed)] == 0) {
            m_direction = 0;
        }
        else if (m_matrix[m_x + m_dx][static_cast<size_t>(m_y)] != 0 ||
                 m_matrix[m_x + m_dx][static_cast<size_t>(m_y + m_height)] != 0) {
            m_direction = 0;
        }
        else {
            m_x += m_dx;
        }
    }
    return true;
}

void Robot::animate() {
    if (m_direction == 0) {
        if (m_current_image < 11) {
            m_current_image++;
        }
        else {
            m_current_image = 0;
        }
    }
    else if (m_direction == 180) {
        if (m_current_image > 11 && m_current_image < 23) {
            m_current_image++;
        }
        else {
            m_current_image = 12;
        }
    }
}


void Robot::shoot() {
    m_bullets->push_back(shared_ptr<Bullet>(new Robot_Bullet(m_x, m_y, m_direction)));
}

Cannon::Cannon(int x, int y, int direction, vector<shared_ptr<Bullet>>* bullets)
    : Enemy(x, y)
{
    this->m_direction = direction;
    this->m_bullets = bullets;
    this->m_solid = false;
    for (int i = 1; i <= 4; i++) {
        m_images.push_back(unique_ptr<QPixmap>(new QPixmap((source_path + "/sprites/monsters/eb_pushka" + to_string(i) + ".png").c_str())));
    }
    if (direction == 90) {
        m_current_image = 1;
    }
    else if (direction == 270) {
        m_current_image = 0;
    }
}

void Cannon::update() {
    if (m_frame_count == 40) {
        shoot();
        m_frame_count = 0;
    }
    else {
        m_frame_count++;
    }
}

void Cannon::animate() {
    //not used in this class
}

unique_ptr<QPixmap> Cannon::die() {
    QPixmap image;
    if (m_direction == 90) {
        image = *m_images[2];
        m_images.erase(m_images.begin() + 2);
        return unique_ptr<QPixmap>(new QPixmap(image));
    }
    else if (m_direction == 270) {
        image = *m_images[3];
        m_images.erase(m_images.begin() + 3);
        return unique_ptr<QPixmap>(new QPixmap(image));
    }
    else {
        return nullptr;
    }
}

void Cannon::shoot() {
    m_bullets->push_back(shared_ptr<Bullet>(new Cannon_Bullet(m_x, m_y, m_direction)));
}

