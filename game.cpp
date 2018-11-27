#include "game.h"
#include "ui_game.h"
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);

    this->setFixedSize(624, 384);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
    timer->start(30);
    buttoms = Pressed_Buttoms();
    string map_path = source_path + "/levels/x1y1.txt";

    matrix = new int*[static_cast<unsigned long long>(width)];
    for (int i = 0; i < width; i++) {
        matrix[i] = new int[static_cast<unsigned long long>(height)];
    }

    enemy_bullets = vector<Bullet*>();
    ammo = vector<Ammo*>();
    ammo.push_back(new Ammo(300, 48 * 5));
    electro_man = new Electro_man(200, 48, 8, matrix);
    enemies = vector<Enemy*>();
    enemies.push_back(new Robot(300, 48, 6, matrix, &enemy_bullets));
    enemies.push_back(new Cannon(624 - 2 * 48, 288, 270, &enemy_bullets));

    texture_samples = vector<QPixmap*>();
    texture_samples.push_back(new QPixmap((source_path + "/sprites/level/tile2.png").c_str()));
    texture_samples.push_back(new QPixmap((source_path + "/sprites/level/tile1.png").c_str()));

    textures = vector<Texture*>();

    this->readMap(map_path);
}

void Game::readMap(string path) {
    ifstream file(path);
    int type;
    if (file.is_open()) {
        for (int y = 7; y >= 0; y--) {
            for (int x = 0; x < 13; x++) {
                file >> type;
                for (int i = 0; i < 48; i++) {
                    for (int j = 0; j < 48; j++) {
                        matrix[x * 48 + i][y * 48 + j] = type;
                    }
                }
                textures.push_back(new Texture(x * 48, y * 48, texture_samples[static_cast<unsigned long long>(type)]));
            }
        }
    }
    file.close();
}

void Game::paintEvent(QPaintEvent* ) {
    QPainter painter(this);
    for (Texture* texture : textures) {
        texture->draw(&painter);
    }
    electro_man->draw(&painter);
    for (auto enemy: enemies) {
        enemy->draw(&painter);
    }
    for (auto bullet : enemy_bullets) {
        bullet->draw(&painter);
    }
    for (auto item : ammo) {
        item->draw(&painter);
    }
}

bool Game::is_overlaid(Object* a, Object* b) {
    if (    a->x > b->x + b->getWidth() ||
            a->x + a->getWidth() < b->x ||
            a->y > b->y + b->getHeight() ||
            a->y + a->getHeight() < b->y)
    {
        return false;
    }
    else {
        return true;
    }
}


void Game::checkHits() {
    for (int i = 0; i < static_cast<int>(enemy_bullets.size()); i++)
    {
        Bullet* bullet = enemy_bullets[static_cast<unsigned long long>(i)];
        if (is_overlaid(bullet, electro_man))
        {
            delete bullet;
            enemy_bullets.erase(enemy_bullets.begin() + i);
            electro_man->die();
            i--;
        }
    }
    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        Enemy* enemy = enemies[static_cast<unsigned long long>(i)];
        std::vector<Bullet*>* bullets = electro_man->getBulletsPtr();
        for (int j = 0; j < static_cast<int>(bullets->size()); j++)
        {
            Bullet* bullet = (*bullets)[static_cast<unsigned long long>(j)];
            if (is_overlaid(bullet, enemy))
            {
                delete bullet;
                QPixmap* dead_texture = enemy->die();
               // dead texture can be nullptr
                if (dead_texture != nullptr) {
                    texture_samples.push_back(dead_texture);
                    textures.push_back(new Texture(enemy->x, enemy->y, dead_texture));
                }
                delete enemy;
                bullets->erase(bullets->begin() + j);
                enemies.erase(enemies.begin() + static_cast<long long>(i));
                i--;
                break;
            }
        }
    }
    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        Enemy* enemy = enemies[static_cast<unsigned long long>(i)];
        if (is_overlaid(enemy, electro_man) && enemy->is_solid())
        {
            electro_man->die();
        }
    }
}

void Game::moveBullets() {
    for (int i = 0; i < static_cast<int>(enemy_bullets.size()); i++) {
        if (!enemy_bullets[static_cast<unsigned long long>(i)]->move(matrix)) {
            delete (enemy_bullets[static_cast<unsigned long long>(i)]);
            enemy_bullets.erase(enemy_bullets.begin() + i);
            i--;
        }
    }
}

void Game::checkItems() {
    for (int i = 0; i < static_cast<int>(ammo.size()); i++)
    {
        Ammo* item = ammo[static_cast<unsigned long long>(i)];
        if (is_overlaid(item, electro_man)) {
            delete item;
            ammo.erase(ammo.begin() + i);
            i--;
        }
    }
}

void Game::timerTick() {
    electro_man->handleKeys(&buttoms);
    for (auto enemy : enemies) {
        enemy->update();
    }
    moveBullets();
    checkHits();
    checkItems();
    this->update();
}

void Game::keyPressEvent(QKeyEvent* e) {
    switch (e->key())
    {
    case (Qt::Key_W): {
        buttoms.w = true;
        break;
    }
    case (Qt::Key_S): {
        buttoms.s = true;
        break;
    }
    case (Qt::Key_A): {
        buttoms.a = true;
        break;
    }
    case (Qt::Key_D): {
        buttoms.d = true;
        break;
    }
    case (Qt::Key_Space): {
        buttoms.fire = true;
        break;
    }
    }
};

void Game::keyReleaseEvent(QKeyEvent* e) {
    switch (e->key())
    {
    case (Qt::Key_W): {
        buttoms.w = false;
        break;
    }
    case (Qt::Key_S): {
        buttoms.s = false;
        break;
    }
    case (Qt::Key_A): {
        buttoms.a = false;
        break;
    }
    case (Qt::Key_D): {
        buttoms.d = false;
        break;
    }
    case (Qt::Key_Space): {
        buttoms.fire = false;
        break;
    }
    case (Qt::Key_H): {
        enemies.push_back(new Robot(300, 48, 6, matrix, &enemy_bullets));
        break;
    }
    }
}

Game::~Game()
{
    delete ui;
    delete electro_man;
    for (int i = 0; i < width; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    for (auto ptr : texture_samples) {
        delete ptr;
    }
    for (auto ptr : textures) {
        delete ptr;
    }
    for (auto enemy : enemies) {
        delete enemy;
    }
    for (auto bullet : enemy_bullets) {
        delete bullet;
    }
}
