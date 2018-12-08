#include "game.h"
#include "ui_game.h"
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <fstream>
#include <iostream>
#include <vector>


Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);

    this->setFixedSize(624, 384);
    timer = unique_ptr<QTimer>(new QTimer());
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(timerTick()));
    timer->start(30);
    buttoms = shared_ptr<Pressed_Buttoms>(new Pressed_Buttoms());
    this->map_path = source_path + "/levels/level1_1.tmx";
    matrix = shared_ptr<unique_ptr<int[]>[]>(new unique_ptr<int[]>[static_cast<unsigned long long>(width)]);
    for (int i = 0; i < width; i++) {
        matrix[i] = unique_ptr<int[]>(new int[static_cast<unsigned long long>(height)]);
        for (int j = 0; j < height; j++) {
            matrix[i][static_cast<unsigned long long>(j)] = 0;
        }
    }

    enemy_bullets = vector<shared_ptr<Bullet>>();
    ammo = vector<unique_ptr<Ammo>>();
    ammo.push_back(unique_ptr<Ammo>(new Ammo(6 * 48, 2 * 48)));
    textures = vector<unique_ptr<Texture>>();
    animated_textures = vector<unique_ptr<Animated_Texture>>();
    vector<QPixmap> vect;
    vect.push_back(QPixmap((source_path + "/sprites/monsters/eb_migalka1a.png").c_str()));
    vect.push_back(QPixmap((source_path + "/sprites/monsters/eb_migalka1b.png").c_str()));
    vect.push_back(QPixmap((source_path + "/sprites/monsters/eb_migalka2a.png").c_str()));
    animated_textures.push_back(unique_ptr<Animated_Texture>(new Animated_Texture(0, 6 * 48, vect)));
    enemies = vector<unique_ptr<Enemy>>();
    enemies.push_back(unique_ptr<Enemy>(new Robot(10 * 48, 6 * 48, 5, matrix, &enemy_bullets)));
    enemies.push_back(unique_ptr<Enemy>(new Cannon(3 * 48, 7 * 48, 270, &enemy_bullets)));
    teleports.push_back(unique_ptr<Teleport>(new Teleport(9 * 48, 2 * 48, 9 * 48, 6 * 48)));
    teleports.push_back(unique_ptr<Teleport>(new Teleport(9 * 48, 6 * 48, 9 * 48, 2 * 48)));

    electro_man = unique_ptr<Electro_man>(new Electro_man(3 * 48, 2 * 48 + 10, 8, matrix));

    this->readMap(map_path);
}

void Game::readMap(string path) {
    vector<shared_ptr<Layer>> layers = Parser::parse_layers(QString(path.c_str()));
    for (int k = 0; k < static_cast<int>(layers.size()); k++) {
        shared_ptr<Layer> layer = layers[static_cast<unsigned long long>(k)];
        string path = map_path + "/../" + layer->path;
        QPixmap sprite_sheet = QPixmap(path.c_str());
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 13; x++) {
                int type = layer->data[static_cast<unsigned long long>((7 - y) * 13 + x)] - layer->firstgid;
                if (type >= 0) {
                    int sprite_x = type % layer->sheet_columns;
                    int sprite_y = type / layer->sheet_columns;
                    if (k == 0 && type < 12) {
                        for (int i = 0; i < 48; i++) {
                            for (int j = 0; j < 48; j++) {
                                matrix[x * 48 + i][static_cast<unsigned long long>(y * 48 + j)] = 1;
                            }
                        }
                    }
                    textures.push_back(unique_ptr<Texture>
                                       (new Texture(x * 48, y * 48, sprite_sheet.copy(sprite_x * 48, sprite_y * 48, 48, 48))));
                }
            }
        }
    }
}

bool Game::is_overlaid(Object* a, Object* b) {
    if (    a->getX() > b->getX() + b->getWidth() ||
            a->getX() + a->getWidth() < b->getX() ||
            a->getY() > b->getY() + b->getHeight() ||
            a->getY() + a->getHeight() < b->getY())
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
        shared_ptr<Bullet>& bullet = enemy_bullets[static_cast<unsigned long long>(i)];
        if (is_overlaid(bullet.get(), electro_man.get()))
        {
            enemy_bullets.erase(enemy_bullets.begin() + i);
            electro_man->die();
            i--;
        }
    }
    vector<shared_ptr<Bullet>>* player_bullets = electro_man->getBulletsPtr();
    for (int i = 0; i < static_cast<int>(player_bullets->size()); i++) {
        shared_ptr<Bullet>& bullet = (*player_bullets)[static_cast<unsigned long long>(i)];
        for (int j = 0; j < static_cast<int>(enemies.size()); j++) {
            unique_ptr<Enemy>& enemy = enemies[static_cast<unsigned long long>(j)];
            if (is_overlaid(bullet.get(), enemy.get()))
            {
                unique_ptr<QPixmap> dead_texture = enemy->die();
               // dead texture can be nullptr
                if (dead_texture != nullptr) {
                    //textures.push_back(new Texture(enemy->getX(), enemy->getY(), dead_texture, 0, 0));
                    textures.push_back(unique_ptr<Texture>(new Texture(enemy->getX(), enemy->getY(), *dead_texture)));
                }
                player_bullets->erase(player_bullets->begin() + i);
                enemies.erase(enemies.begin() + static_cast<long long>(j));
                i--;
                break;
            }
        }

        for (int j = 0; j < static_cast<int>(animated_textures.size()); j++) {
            unique_ptr<Animated_Texture>& animated_texture = animated_textures[static_cast<unsigned long long>(j)];
            if (is_overlaid(bullet.get(), animated_texture.get()))
            {
                unique_ptr<QPixmap> dead_texture = animated_texture->destroy();
               // dead texture can be nullptr
                if (dead_texture != nullptr) {
                    textures.push_back(unique_ptr<Texture>(new Texture(animated_texture->getX(), animated_texture->getY(), *dead_texture)));
                }
                player_bullets->erase(player_bullets->begin() + i);
                animated_textures.erase(animated_textures.begin() + static_cast<long long>(j));
                i--;
                break;
            }
        }
    }

    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        unique_ptr<Enemy>& enemy = enemies[static_cast<unsigned long long>(i)];
        if (is_overlaid(enemy.get(), electro_man.get()) && enemy->is_solid())
        {
            electro_man->die();
        }
    }
}

void Game::moveBullets() {
    for (int i = 0; i < static_cast<int>(enemy_bullets.size()); i++) {
        if (!enemy_bullets[static_cast<unsigned long long>(i)]->move(matrix)) {
            enemy_bullets.erase(enemy_bullets.begin() + i);
            i--;
        }
    }
}

void Game::checkItems() {
    for (int i = 0; i < static_cast<int>(ammo.size()); i++)
    {
        unique_ptr<Ammo>& item = ammo[static_cast<unsigned long long>(i)];
        if (is_overlaid(item.get(), electro_man.get())) {
            ammo.erase(ammo.begin() + i);
            i--;
        }
    }
}

void Game::checkTeleports() {
    for (auto&& teleport : teleports) {
        if (is_overlaid(electro_man.get(), teleport.get())) {
            electro_man->teleportation(teleport->destination_x, teleport->destination_y);
            return;
        }
    }
}

void Game::timerTick() {
    electro_man->handleKeys(buttoms);
    for (auto&& enemy : enemies) {
        enemy->update();
    }
    if (buttoms->s) {
        checkTeleports();
    }
    moveBullets();
    checkHits();
    checkItems();
    this->update();
}

void Game::paintEvent(QPaintEvent* ) {
    QPainter painter(this);
    for (auto&& texture : textures) {
        texture->draw(painter);
    }
    for (auto&& texture : animated_textures) {
        texture->draw(painter);
    }
    for (auto&& teleport : teleports) {
        teleport->draw(painter);
    }
    for (auto&& enemy : enemies) {
        enemy->draw(painter);
    }
    for (auto&& bullet : enemy_bullets) {
        bullet->draw(painter);
    }
    for (auto&& item : ammo) {
        item->draw(painter);
    }
    electro_man->draw(painter);
}

void Game::keyPressEvent(QKeyEvent* e) {
    switch (e->key())
    {
    case (Qt::Key_W): {
        buttoms->w = true;
        break;
    }
    case (Qt::Key_S): {
        buttoms->s = true;
        break;
    }
    case (Qt::Key_A): {
        buttoms->a = true;
        break;
    }
    case (Qt::Key_D): {
        buttoms->d = true;
        break;
    }
    case (Qt::Key_Space): {
        buttoms->fire = true;
        break;
    }
    }
};

void Game::keyReleaseEvent(QKeyEvent* e) {
    switch (e->key())
    {
    case (Qt::Key_W): {
        buttoms->w = false;
        break;
    }
    case (Qt::Key_S): {
        buttoms->s = false;
        break;
    }
    case (Qt::Key_A): {
        buttoms->a = false;
        break;
    }
    case (Qt::Key_D): {
        buttoms->d = false;
        break;
    }
    case (Qt::Key_Space): {
        buttoms->fire = false;
        break;
    }
    }
}

Game::~Game()
{
    delete ui;
}
