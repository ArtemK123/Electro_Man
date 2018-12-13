#include "player.h"

Player_Animation::Player_Animation() {
    m_direction = 0;
    m_def_image = shared_ptr<QPixmap>(new QPixmap((source_path + "/sprites/eb33.png").c_str()));
    shared_ptr<QPixmap> image;
    for (int i = 1; i <= 5; i++) {
        image = shared_ptr<QPixmap>(new QPixmap((source_path + "/sprites/eb" + to_string(i) + ".png").c_str()));
        m_images.push_back(image);
    }
    for (int i = 8; i <= 27; i++) {
        image = shared_ptr<QPixmap>(new QPixmap((source_path + "/sprites/eb" + to_string(i) + ".png").c_str()));
        m_images.push_back(image);
    }
    for (int i = 34; i >= 28; i--) {
        image = shared_ptr<QPixmap>(new QPixmap((source_path + "/sprites/eb" + to_string(i) + ".png").c_str()));
        m_images.push_back(image);
    }
    // 0 - 4 rotation
    // 0, 5 - 14 move right
    // 4, 15 - 24 move left
    // 25 - 31 dying
}

shared_ptr<QPixmap> Player_Animation::getImage() {
    if (m_queue.size() > 0) {
        shared_ptr<QPixmap> ptr = m_queue.front();
        m_queue.pop();
        return ptr;
    }
    return m_def_image;
}

int Player_Animation::getDirection() {
    return this->m_direction;
}

void Player_Animation::move() {
    if (m_queue.size() == 0)
    {
        if (m_direction == 0) {
            if (m_current_image > 4 && m_current_image < 14) {
                m_current_image++;
            }
            else if (m_current_image == 0 || m_current_image == 14) {
                m_current_image = 5;
            }
            else {
                m_current_image = 0;
            }
        }
        else if (m_direction == 180) {
            if (m_current_image > 14 && m_current_image < 24) {
                m_current_image++;
            }
            else if (m_current_image == 4 || m_current_image == 24) {
                m_current_image = 15;
            }
            else {
                m_current_image = 4;
            }
        }
        m_queue.push(m_images[static_cast<size_t>(m_current_image)]);
    }
}

void Player_Animation::rotate() {
    if (m_queue.size() == 0 ) {
        if (m_direction == 0) {
            //from left to right
            for (int i = 0; i < 5; i++) {
                m_queue.push(m_images[static_cast<size_t>(i)]);
            }
            m_current_image = 4;
        }
        else if (m_direction == 180) {
            //from right to left
            for (int i = 4; i >= 0; i--) {
                m_queue.push(m_images[static_cast<size_t>(i)]);
            }
            m_current_image = 0;
        }
        m_direction = (m_direction == 0) ? 180 : 0;
    }
}

void Player_Animation::jump() {
    if (m_queue.size() == 0) {
        if (m_direction == 0) {
            m_current_image = 7;
        }
        else if (m_direction == 180) {
            m_current_image = 17;
        }
        m_queue.push(m_images[static_cast<size_t>(m_current_image)]);
    }
}

void Player_Animation::stay() {
    if (m_queue.size() == 0) {
        if (m_direction == 0) {
            m_current_image = 0;
        }
        else if (m_direction == 180) {
            m_current_image = 4;
        }
        m_queue.push(m_images[static_cast<size_t>(m_current_image)]);
    }
}

void Player_Animation::die() {
    if (m_queue.size() < 5) {
        for (int i = 25; i <= 31; i++) {
            m_queue.push(m_images[static_cast<size_t>(i)]);
        }
    }
}

Electro_man::Electro_man(int x, int y, int speed, shared_ptr<unique_ptr<int[]>[]> matrix) :
    Object(x, y)
{
    this->m_matrix = matrix;
    this->m_width = 48;
    this->m_height = 84;
    this->m_speed = speed;
    this->m_overlay = 10;
    this->m_is_jump = false;
    this->m_dx = 0;
    this->m_dy = 0;
    this->m_frame_delay = 0;
    this->m_bullets = vector<shared_ptr<Bullet>>();
    this->m_animation = unique_ptr<Player_Animation>(new Player_Animation());
}

void Electro_man::handleKeys(shared_ptr<Pressed_Buttoms> buttoms) {
    if (!m_is_jump && m_frame_delay == 0) {
        if (buttoms->a && !buttoms->d) {
            m_dx = -m_speed;
            if (m_animation->getDirection() == 0) {
                m_animation->rotate();
                m_frame_delay += 4;
                m_dx = 0;
            }
            else {
                m_animation->move();
            }
        }
        else if (buttoms->d && !buttoms->a) {
            m_dx = m_speed;
            if (m_animation->getDirection() == 180) {
                m_animation->rotate();
                m_frame_delay += 4;
                m_dx = 0;
            }
            else {
                m_animation->move();
            }
        }
        else {
            m_dx = 0;
            m_animation->stay();
        }
    }

    else if (m_frame_delay > 0) {
        m_frame_delay--;
    }

    if (buttoms->w && !m_is_jump) {
        m_is_jump = true;
        m_animation->jump();
        m_dy = int(2.5 * m_speed);
        if (buttoms->a) {
            m_dx = -m_speed;
        }
        else if (buttoms->d) {
            m_dx = m_speed;
        }
    }

    else if (m_is_jump) {
        m_animation->jump();
        m_dy = max(-3 * m_speed, m_dy - 2);
    }

    if (buttoms->fire) {
        shoot();
    }

    move(m_matrix);
}

bool Electro_man::move(shared_ptr<unique_ptr<int[]>[]>) {
    if (m_dx > 0) {
        if (m_x + m_dx + m_width < 13 * 48 &&
            m_matrix[m_x + m_dx + m_width - m_overlay][static_cast<size_t>(m_y)] == 0 &&
            m_matrix[m_x + m_dx + m_width - m_overlay][static_cast<size_t>(m_y + m_height)] == 0 &&
            m_matrix[m_x + m_dx + m_width - m_overlay][static_cast<size_t>(m_y + m_height/2)] == 0)
        {
            m_x += m_dx;
        }
        else {
            int i = 0;
            while (m_x + i + m_width < 13 * 48 &&
                   m_matrix[m_x + i + m_width - m_overlay][static_cast<size_t>(m_y)] == 0 &&
                   m_matrix[m_x + i + m_width - m_overlay][static_cast<size_t>(m_y + m_height)] == 0 &&
                   m_matrix[m_x + i + m_width - m_overlay][static_cast<size_t>(m_y + m_height/2)] == 0)
            {
                i++;
            }
            m_x += i - 1; // (i - 1) to avoid collisions
        }
    }
    else if (m_dx < 0) {
        // dx < 0, it means x + dx < x
        if (m_x + m_dx > 0 &&
            m_matrix[m_x + m_dx + m_overlay][static_cast<size_t>(m_y)] == 0 &&
            m_matrix[m_x + m_dx + m_overlay][static_cast<size_t>(m_y + m_height)] == 0 &&
            m_matrix[m_x + m_dx + m_overlay][static_cast<size_t>(m_y + m_height/2)] == 0)
        {
            m_x += m_dx;
        }
        else {
            int i = 0;
            while (m_x + i > 0 &&
                   m_matrix[m_x + i + m_overlay][static_cast<size_t>(m_y)] == 0 &&
                   m_matrix[m_x + i + m_overlay][static_cast<size_t>(m_y + m_height)] == 0 &&
                   m_matrix[m_x + i + m_overlay][static_cast<size_t>(m_y + m_height/2)] == 0)
            {
                i--;
            }
            m_x += i + 1; // i will be negative
        }
    }

    if (m_dy >= 0) {
        if (//y + height + dy < 8 * 48 &&
            m_matrix[m_x + m_overlay][static_cast<size_t>(m_y + m_height + m_dy)] == 0 &&
            m_matrix[m_x + m_width - m_overlay][static_cast<size_t>(m_y + m_height + m_dy)] == 0)
        {
            m_y += m_dy;
        }
        else {
            m_dy = 0;
            int i = 0;
            while (//y + height + i < 8 * 48 &&
                   m_matrix[m_x + m_overlay][static_cast<size_t>(m_y + m_height + i)] == 0 &&
                   m_matrix[m_x + m_width - m_overlay][static_cast<size_t>(m_y + m_height + i)] == 0)
            {
                i++;
            }
            m_y += i - 1;
        }
    }

    else if (m_dy < 0) {
        //dy < 0, it means y + dy < y
        if (m_y + m_dy > 0 &&
            m_matrix[m_x + 10][static_cast<size_t>(m_y + m_dy)] == 0 &&
            m_matrix[m_x + 38][static_cast<size_t>(m_y + m_dy)] == 0)
        {
            m_y += m_dy;
        }
        else {
            m_dy = 0;
            m_is_jump = false;
            int i = 0;
            while (m_y + i > 0 &&
                   m_matrix[m_x + 10][static_cast<size_t>(m_y + i)] == 0 &&
                   m_matrix[m_x + 38][static_cast<size_t>(m_y + i)] == 0)
            {
                i--;
            }
            m_y += i + 1; // i will be negative
        }
    }
    if (!m_is_jump && m_matrix[m_x + 15][static_cast<size_t>(m_y - m_speed)] == 0 &&
         m_matrix[m_x + m_width - 33][static_cast<size_t>(m_y - m_speed)] == 0)
    {
        m_is_jump = true;
        m_animation->jump();
    }

    moveBullets();
    return true;
}

void Electro_man::moveBullets() {
    for (int i = 0; i < static_cast<int>(m_bullets.size()); i++) {
        if (!m_bullets[static_cast<unsigned long long>(i)]->move(m_matrix)) {
            m_bullets.erase(m_bullets.begin() + i);
            i--;
        }
    }
}

void Electro_man::shoot() {
    if (m_bullets.size() == 0 || abs(m_bullets.back()->getX() - m_x) > 100) {
        m_bullets.push_back(unique_ptr<Bullet>(new Player_Bullet(m_x, m_y + 40, m_animation->getDirection())));
    }
}

void Electro_man::teleportation(int destination_x, int destination_y) {
    if (m_frame_delay == 0) {
        this->m_x = destination_x;
        this->m_y = destination_y;
        m_dx = 0;

        m_frame_delay += 20;
    }
}

void Electro_man::die() {
    m_animation->die();
}

vector<shared_ptr<Bullet>>* Electro_man::getBulletsPtr() {
    return &m_bullets;
}

void Electro_man::draw(QPainter& painter) {
    shared_ptr<QPixmap> image_ptr = m_animation->getImage();
    if (image_ptr) {
        painter.drawPixmap(m_x, 384 - m_y - m_height, *image_ptr);
    }
    for (auto bullet : m_bullets) {
        bullet->draw(painter);
    }
}
