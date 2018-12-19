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

void Player_Animation::teleport() {
    if (m_queue.size() == 0) {
        for (int i = 0; i < 20; i++) {
            m_queue.push(m_images[29]);
        }
    }
}

Electro_man::Electro_man(int x, int y, int speed, shared_ptr<Matrix> matrix, shared_ptr<Bullet_Manager> bullets, shared_ptr<Pressed_Buttons> buttons)
    : Object(x, y),
    Movable_Object(x, y),
    Shooting_Object(x, y, 0)
{
    this->m_matrix = matrix;
    this->m_bullets = bullets;
    this->m_buttons = buttons;
    this->m_width = 48;
    this->m_height = 84;
    this->m_speed = speed;
    this->m_overlay = 10;
    this->m_is_jump = false;
    this->m_frame_delay = 0;
    this->m_animation = unique_ptr<Player_Animation>(new Player_Animation());
    this->m_bullet = unique_ptr<Bullet>(new Player_Bullet(0, 0, 0));
}

void Electro_man::update() {
    handleKeys(*m_buttons);
    m_frame_delay = max(0, m_frame_delay - 1);
}

void Electro_man::handleKeys(Pressed_Buttons& buttons) {
    int temp_dy = m_dy;
    m_dy = 0;
    if (buttons.a && !buttons.d && m_frame_delay == 0) {
        if (!m_is_jump) {
            m_dx = -m_speed;
            if (m_animation->getDirection() == 0) {
                m_animation->rotate();
                m_frame_delay += 4;
            }
            else {
                m_animation->move();
                move();
            }
        }
        else if (m_is_jump) {
            move();
        }
    }
    else if (buttons.d && !buttons.a && m_frame_delay == 0) {
        if (!m_is_jump) {
            m_dx = m_speed;
            if (m_animation->getDirection() == 180) {
                m_animation->rotate();
                m_frame_delay += 4;
            }
            else {
                m_animation->move();
                move();
            }
        }
        else if (m_is_jump) {
            move();
        }
    }
    else if (!m_is_jump) {
        m_animation->stay();
        m_dx = 0;
    }
    else if (m_is_jump) {
        move();
    }

    m_dy = temp_dy;
    int temp_dx = m_dx;
    m_dx = 0;

    if (buttons.w && !m_is_jump && m_frame_delay == 0) {
        m_is_jump = true;
        m_animation->jump();
        m_dy = int(2.5 * m_speed);
        /*if (buttoms.a) {
            temp_dx = -m_speed;
        }
        else if (buttoms.d) {
            temp_dx = m_speed;
        }*/
        move();
    }
    else if (m_is_jump) {
        m_animation->jump();
        m_dy = max(-3 * m_speed, m_dy - 2);
        move();
    }
    m_dx = temp_dx;

    if (m_frame_delay == 0 && checkFalling(*m_matrix)) {
        m_is_jump = true;
    }


    if (buttons.fire) {
        if (m_bullets->getSize() == 0 ||
            abs(m_bullets->getItem(static_cast<int>(m_bullets->getSize() - 1))->getX() - m_x) > 100)
        {
            m_bullets->addItem(shoot());
        }
    }
}

shared_ptr<Bullet> Electro_man::shoot() {
    shared_ptr<Bullet> bullet = Shooting_Object::shoot();
    bullet->setY(m_y + m_height / 2 - 10);
    bullet->setDirection(m_animation->getDirection());
    return bullet;
}

bool Electro_man::checkMapCollision(Matrix& matrix) {
    if (    0 > m_x || m_x + m_width > matrix.getWidth() ||
            0 > m_y || m_y + m_height > matrix.getHeight() ||
            matrix(m_x + m_overlay, m_y) == 1 ||
            matrix(m_x + m_width / 2, m_y) == 1 ||
            matrix(m_x + m_width, m_y) == 1 ||
            matrix(m_x + m_overlay, m_y + m_height / 2) == 1 ||
            matrix(m_x + m_width / 2, m_y + m_height / 2) == 1 ||
            matrix(m_x + m_width - m_overlay, m_y + m_height / 2) == 1 ||
            matrix(m_x + m_overlay, m_y + m_height) == 1 ||
            matrix(m_x + m_width / 2, m_y + m_height) == 1 ||
            matrix(m_x + m_width - m_overlay, m_y + m_height) == 1)
    {
        return true;
    } else {
        return false;
    }
};

bool Electro_man::checkFalling(Matrix& matrix) {
    if (matrix(m_x + 2 * m_overlay, m_y - 1) == 0 ||
        matrix(m_x + m_width - 2 * m_overlay, m_y - 1) == 0)
    {
        return true;
    }
    else {
        return false;
    }
}

void Electro_man::move() {
    int temp_dx = m_dx;
    m_dx = 0;
    Movable_Object::move();
    if (checkMapCollision(*m_matrix)) {
        moveBack();
    }
    m_dx = temp_dx;
    int temp_dy = m_dy;
    m_dy = 0;
    Movable_Object::move();
    if (checkMapCollision(*m_matrix)) {
        moveBack();
    }
    m_dy = temp_dy;
}

void Electro_man::moveBack() {
    if (m_dx > 0) {
        int i = 0;
        while ((*m_matrix)(m_x + m_width - m_overlay - i, m_y) == 1 ||
               (*m_matrix)(m_x + m_width - m_overlay - i, m_y + m_height / 2) == 1 ||
               (*m_matrix)(m_x + m_width - m_overlay - i, m_y + m_height) == 1)
        {
            i++;
        }
        m_x -= i;
    }
    else if (m_dx < 0) {
        int i = 0;
        while ((*m_matrix)(m_x + m_overlay + i, m_y) == 1 ||
               (*m_matrix)(m_x + m_overlay + i, m_y + m_height / 2) == 1 ||
               (*m_matrix)(m_x + m_overlay + i, m_y + m_height) == 1)
        {
            i++;
        }
        m_x += i;
    }

    if (!checkMapCollision(*m_matrix)) {
        return;
    }

    if (m_dy > 0) {
        m_y -= m_dy;
    }

    else if (m_dy < 0) {
        m_dy = 0;
        m_is_jump = false;
        int i = 0;
        while ((*m_matrix)(m_x + m_overlay, m_y + i) == 1 ||
               (*m_matrix)(m_x + m_width / 2, m_y + i) == 1 ||
               (*m_matrix)(m_x - m_overlay + m_width, m_y + i) == 1)
        {
            i++;
        }
        m_y += i;
    }
}

void Electro_man::teleportation(int destination_x, int destination_y) {
    if (m_frame_delay == 0) {
        this->m_x = destination_x;
        this->m_y = destination_y;
        m_animation->teleport();

        m_frame_delay += 20;
    }
}

void Electro_man::die() {
    m_animation->die();
}

void Electro_man::draw(QPainter& painter) {
    shared_ptr<QPixmap> image_ptr = m_animation->getImage();
    if (image_ptr) {
        painter.drawPixmap(m_x, 384 - m_y - m_height, *image_ptr);
    }
}

Electro_man::~Electro_man() {

}
