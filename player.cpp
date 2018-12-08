#include "player.h"

Player_Animation::Player_Animation() {
    direction = 0;
    def_image = shared_ptr<QPixmap>(new QPixmap((source_path + "/sprites/eb33.png").c_str()));
    shared_ptr<QPixmap> image;
    for (int i = 1; i <= 5; i++) {
        image = shared_ptr<QPixmap>(new QPixmap((source_path + "/sprites/eb" + to_string(i) + ".png").c_str()));
        images.push_back(image);
    }
    for (int i = 8; i <= 27; i++) {
        image = shared_ptr<QPixmap>(new QPixmap((source_path + "/sprites/eb" + to_string(i) + ".png").c_str()));
        images.push_back(image);
    }
    for (int i = 34; i >= 28; i--) {
        image = shared_ptr<QPixmap>(new QPixmap((source_path + "/sprites/eb" + to_string(i) + ".png").c_str()));
        images.push_back(image);
    }
    // 0 - 4 rotation
    // 0, 5 - 14 move right
    // 4, 15 - 24 move left
    // 25 - 31 dying
}

shared_ptr<QPixmap> Player_Animation::getImage() {
    if (queue.size() > 0) {
        shared_ptr<QPixmap> ptr = queue.front();
        queue.pop();
        return ptr;
    }
    return def_image;
}

int Player_Animation::getDirection() {
    return this->direction;
}

void Player_Animation::move() {
    if (queue.size() == 0)
    {
        if (direction == 0) {
            if (current_image > 4 && current_image < 14) {
                current_image++;
            }
            else if (current_image == 0 || current_image == 14) {
                current_image = 5;
            }
            else {
                current_image = 0;
            }
        }
        else if (direction == 180) {
            if (current_image > 14 && current_image < 24) {
                current_image++;
            }
            else if (current_image == 4 || current_image == 24) {
                current_image = 15;
            }
            else {
                current_image = 4;
            }
        }
        queue.push(images[static_cast<unsigned long long>(current_image)]);
    }
}

void Player_Animation::rotate() {
    if (queue.size() == 0 ) {
        if (direction == 0) {
            //from left to right
            for (int i = 0; i < 5; i++) {
                queue.push(images[static_cast<unsigned long long>(i)]);
            }
            current_image = 4;
        }
        else if (direction == 180) {
            //from right to left
            for (int i = 4; i >= 0; i--) {
                queue.push(images[static_cast<unsigned long long>(i)]);
            }
            current_image = 0;
        }
        direction = (direction == 0) ? 180 : 0;
    }
}

void Player_Animation::jump() {
    if (queue.size() == 0) {
        if (direction == 0) {
            current_image = 7;
        }
        else if (direction == 180) {
            current_image = 17;
        }
        queue.push(images[static_cast<unsigned long long>(current_image)]);
    }
}

void Player_Animation::stay() {
    if (queue.size() == 0) {
        if (direction == 0) {
            current_image = 0;
        }
        else if (direction == 180) {
            current_image = 4;
        }
        queue.push(images[static_cast<unsigned long long>(current_image)]);
    }
}

void Player_Animation::die() {
    if (queue.size() < 5) {
        for (int i = 25; i <= 31; i++) {
            queue.push(images[static_cast<unsigned long long>(i)]);
        }
    }
}

Electro_man::Electro_man(int x, int y, int speed, shared_ptr<unique_ptr<int[]>[]> matrix) :
    Object(x, y)
{
    this->matrix = matrix;
    this->width = 48;
    this->height = 84;
    this->speed = speed;
    this->overlay = 10;
    this->is_jump = false;
    this->dx = 0;
    this->dy = 0;
    this->frame_delay = 0;
    this->bullets = vector<shared_ptr<Bullet>>();
    this->animation = unique_ptr<Player_Animation>(new Player_Animation());
}

void Electro_man::handleKeys(shared_ptr<Pressed_Buttoms> buttoms) {
    if (!is_jump && frame_delay == 0) {
        if (buttoms->a && !buttoms->d) {
            dx = -speed;
            if (animation->getDirection() == 0) {
                animation->rotate();
                frame_delay += 4;
                dx = 0;
            }
            else {
                animation->move();
            }
        }
        else if (buttoms->d && !buttoms->a) {
            dx = speed;
            if (animation->getDirection() == 180) {
                animation->rotate();
                frame_delay += 4;
                dx = 0;
            }
            else {
                animation->move();
            }
        }
        else {
            dx = 0;
            animation->stay();
        }
    }

    else if (frame_delay > 0) {
        frame_delay--;
    }

    if (buttoms->w && !is_jump) {
        is_jump = true;
        animation->jump();
        dy = int(2.5 * speed);
        if (buttoms->a) {
            dx = -speed;
        }
        else if (buttoms->d) {
            dx = speed;
        }
    }

    else if (is_jump) {
        animation->jump();
        dy = max(-3 * speed, dy - 2);
    }

    if (buttoms->fire) {
        shoot();
    }

    move(matrix);
}

bool Electro_man::move(shared_ptr<unique_ptr<int[]>[]>) {
    if (dx > 0) {
        if (x + dx + width < 13 * 48 &&
            matrix[x + dx + width - overlay][static_cast<size_t>(y)] == 0 &&
            matrix[x + dx + width - overlay][static_cast<size_t>(y + height)] == 0 &&
            matrix[x + dx + width - overlay][static_cast<size_t>(y + height/2)] == 0)
        {
            x += dx;
        }
        else {
            int i = 0;
            while (x + i + width < 13 * 48 &&
                   matrix[x + i + width - overlay][static_cast<size_t>(y)] == 0 &&
                   matrix[x + i + width - overlay][static_cast<size_t>(y + height)] == 0 &&
                   matrix[x + i + width - overlay][static_cast<size_t>(y + height/2)] == 0)
            {
                i++;
            }
            x += i - 1; // (i - 1) to avoid collisions
        }
    }
    else if (dx < 0) {
        // dx < 0, it means x + dx < x
        if (x + dx > 0 &&
            matrix[x + dx + overlay][static_cast<size_t>(y)] == 0 &&
            matrix[x + dx + overlay][static_cast<size_t>(y + height)] == 0 &&
            matrix[x + dx + overlay][static_cast<size_t>(y + height/2)] == 0)
        {
            x += dx;
        }
        else {
            int i = 0;
            while (x + i > 0 &&
                   matrix[x + i + overlay][static_cast<size_t>(y)] == 0 &&
                   matrix[x + i + overlay][static_cast<size_t>(y + height)] == 0 &&
                   matrix[x + i + overlay][static_cast<size_t>(y + height/2)] == 0)
            {
                i--;
            }
            x += i + 1; // i will be negative
        }
    }

    if (dy >= 0) {
        if (//y + height + dy < 8 * 48 &&
            matrix[x + overlay][static_cast<size_t>(y + height + dy)] == 0 &&
            matrix[x + width - overlay][static_cast<size_t>(y + height + dy)] == 0)
        {
            y += dy;
        }
        else {
            dy = 0;
            int i = 0;
            while (//y + height + i < 8 * 48 &&
                   matrix[x + overlay][static_cast<size_t>(y + height + i)] == 0 &&
                   matrix[x + width - overlay][static_cast<size_t>(y + height + i)] == 0)
            {
                i++;
            }
            y += i - 1;
        }
    }

    else if (dy < 0) {
        //dy < 0, it means y + dy < y
        if (y + dy > 0 &&
            matrix[x + 10][static_cast<size_t>(y + dy)] == 0 &&
            matrix[x + 38][static_cast<size_t>(y + dy)] == 0)
        {
            y += dy;
        }
        else {
            dy = 0;
            is_jump = false;
            int i = 0;
            while (y + i > 0 &&
                   matrix[x + 10][static_cast<size_t>(y + i)] == 0 &&
                   matrix[x + 38][static_cast<size_t>(y + i)] == 0)
            {
                i--;
            }
            y += i + 1; // i will be negative
        }
    }
    if (!is_jump && matrix[x + 15][static_cast<size_t>(y - speed)] == 0 &&
         matrix[x + width - 33][static_cast<size_t>(y - speed)] == 0)
    {
        is_jump = true;
        animation->jump();
    }

    moveBullets();
    return true;
}

void Electro_man::moveBullets() {
    for (int i = 0; i < static_cast<int>(bullets.size()); i++) {
        if (!bullets[static_cast<unsigned long long>(i)]->move(matrix)) {
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
}

void Electro_man::shoot() {
    if (bullets.size() == 0 || abs(bullets.back()->getX() - x) > 100) {
        bullets.push_back(unique_ptr<Bullet>(new Player_Bullet(x, y + 40, animation->getDirection())));
    }
}

void Electro_man::teleportation(int destination_x, int destination_y) {
    if (frame_delay == 0) {
        this->x = destination_x;
        this->y = destination_y;
        dx = 0;

        frame_delay += 20;
    }
}

void Electro_man::die() {
    animation->die();
}

vector<shared_ptr<Bullet>>* Electro_man::getBulletsPtr() {
    return &bullets;
}

void Electro_man::draw(QPainter& painter) {
    shared_ptr<QPixmap> image_ptr = animation->getImage();
    if (image_ptr) {
        painter.drawPixmap(x, 384 - y - height, *image_ptr);
    }
    for (auto bullet : bullets) {
        bullet->draw(painter);
    }
}
