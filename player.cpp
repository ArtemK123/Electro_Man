#include "player.h"

using namespace std;


Animation_Handler::Animation_Handler() {
    images = vector<QPixmap*>();
    current_image = 0;
    direction = 0;
    def_image = new QPixmap((source_path + "/sprites/eb33.png").c_str());
    QPixmap* image;
    for (int i = 1; i <= 5; i++) {
        image = new QPixmap((source_path + "/sprites/eb" + to_string(i) + ".png").c_str());
        images.push_back(image);
    }
    for (int i = 8; i <= 27; i++) {
        image = new QPixmap((source_path + "/sprites/eb" + to_string(i) + ".png").c_str());
        images.push_back(image);
    }
    for (int i = 34; i >= 28; i--) {
        image = new QPixmap((source_path + "/sprites/eb" + to_string(i) + ".png").c_str());
        images.push_back(image);
    }
    // 0 - 4 rotation
    // 0, 5 - 14 move right
    // 4, 15 - 24 move left
    // 25 - 31 dying
}

QPixmap* Animation_Handler::getImage() {
    if (queue.size() > 0) {
        QPixmap* ptr = queue.front();
        queue.pop();
        return ptr;
    }
    return def_image;
}

void Animation_Handler::move() {
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

void Animation_Handler::rotate() {
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

void Animation_Handler::jump() {
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

void Animation_Handler::stay() {
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

void Animation_Handler::die() {
    if (queue.size() < 5) {
        for (int i = 25; i <= 31; i++) {
            queue.push(images[static_cast<unsigned long long>(i)]);
        }

    current_image = 0;
    }
}

Animation_Handler::~Animation_Handler() {
    for (QPixmap* ptr : images) {
        delete ptr;
    }
    delete def_image;
}

Electro_man::Electro_man(int x, int y, int speed, int** matrix) :
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
    this->rotation_step = 0;
    this->bullets = vector<Bullet*>();
    this->animation = new Animation_Handler();
}

void Electro_man::handleKeys(Pressed_Buttoms* buttoms) {
    if (!is_jump && rotation_step == 0) {
        if (buttoms->a && !buttoms->d) {
            dx = -speed;
            if (animation->direction == 0) {
                animation->rotate();
                rotation_step = 4;
                dx = 0;
            }
            else {
                animation->move();
            }
        }
        else if (buttoms->d && !buttoms->a) {
            dx = speed;
            if (animation->direction == 180) {
                animation->rotate();
                rotation_step = 4;
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

    else if (rotation_step > 0) {
        rotation_step--;
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

    move();
}

void Electro_man::move() {
    if (dx > 0) {
        if (matrix[x + dx + width - overlay][y] == 0 &&
            matrix[x + dx + width - overlay][y + height ] == 0 &&
            matrix[x + dx + width - overlay][y + height/2 ] == 0)
        {
            x += dx;
        }
        else {
            int i = 0;
            while (matrix[x + i + width - overlay][y] == 0 &&
                   matrix[x + i + width - overlay][y + height ] == 0 &&
                   matrix[x + i + width - overlay][y + height/2 ] == 0)
            {
                i++;
            }
            x += i - 1; // (i - 1) to avoid collisions
        }
    }
    else if (dx < 0) {
        if (matrix[x + dx + overlay][y] == 0 &&
            matrix[x + dx + overlay][y + height] == 0 &&
            matrix[x + dx + overlay][y + height/2] == 0)
        {
            x += dx;
        }
        else {
            int i = 0;
            while (matrix[x + i + overlay][y] == 0 &&
                   matrix[x + i + overlay][y + height] == 0 &&
                   matrix[x + i + overlay][y + height/2] == 0)
            {
                i--;
            }
            x += i + 1; // i will be negative
        }
    }

    if (dy >= 0) {
        if (matrix[x + overlay][y + height + dy] == 0 && matrix[x + width - overlay][y + height + dy ] == 0) {
            y += dy;
        }
        else {
            dy = 0;
            int i = 0;
            while (matrix[x + overlay][y + height + i] == 0 && matrix[x + width - overlay][y + height + i] == 0) {
                i++;
            }
            y += i - 1;
        }
    }

    else if (dy < 0) {
        if (matrix[x + 10][y + dy] == 0 && matrix[x + 38][y + dy] == 0) {
            y += dy;
        }
        else {
            dy = 0;
            is_jump = false;
            int i = 0;
            while (matrix[x + 10][y + i] == 0 && matrix[x + 38][y + i] == 0) {
                i--;
            }
            y += i + 1; // i will be negative
        }
    }
    if (!is_jump && matrix[x + 15][y - speed] == 0 && matrix[x + width - 33][y - speed] == 0) {
        is_jump = true;
        animation->jump();
    }

    moveBullets();
}

void Electro_man::moveBullets() {
    for (int i = 0; i < static_cast<int>(bullets.size()); i++) {
        if (!bullets[static_cast<unsigned long long>(i)]->move(matrix)) {
            delete (bullets[static_cast<unsigned long long>(i)]);
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
}

void Electro_man::shoot() {
    if (bullets.size() == 0 || abs(bullets.back()->x - x) > 100) {
        bullets.push_back(new Player_Bullet(x, y + 40, animation->direction));
    }
}

void Electro_man::die() {
    animation->die();
}

vector<Bullet*>* Electro_man::getBulletsPtr() {
    return &bullets;
}

void Electro_man::draw(QPainter* painter) {
    QPixmap* image_ptr = animation->getImage();
    if (image_ptr) {
        painter->drawPixmap(x, 384 - y - height, *image_ptr);
    }
    for (Bullet* bullet : bullets) {
        bullet->draw(painter);
    }
}

Electro_man::~Electro_man() {
    delete animation;
    for (Bullet* bullet : bullets) {
        delete bullet;
    }
}

