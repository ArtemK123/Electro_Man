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
    m_timer = unique_ptr<QTimer>(new QTimer());
    connect(m_timer.get(), SIGNAL(timeout()), this, SLOT(timerTick()));
    m_timer->start(30);
    m_buttoms = shared_ptr<Pressed_Buttoms>(new Pressed_Buttoms());
    this->m_map_path = source_path + "/levels/level1_1.tmx";
    m_matrix = shared_ptr<unique_ptr<int[]>[]>(new unique_ptr<int[]>[static_cast<size_t>(m_width)]);
    for (int i = 0; i < m_width; i++) {
        m_matrix[i] = unique_ptr<int[]>(new int[static_cast<size_t>(m_height)]);
        for (int j = 0; j < m_height; j++) {
            m_matrix[i][static_cast<size_t>(j)] = 0;
        }
    }

    m_enemy_bullets = vector<shared_ptr<Bullet>>();
    m_ammo = vector<unique_ptr<Ammo>>();
    m_ammo.push_back(unique_ptr<Ammo>(new Ammo(6 * 48, 2 * 48)));
    m_textures = vector<unique_ptr<Texture>>();
    m_animated_textures = vector<unique_ptr<Animated_Texture>>();
    vector<QPixmap> vect;
    vect.push_back(QPixmap((source_path + "/sprites/monsters/eb_migalka1a.png").c_str()));
    vect.push_back(QPixmap((source_path + "/sprites/monsters/eb_migalka1b.png").c_str()));
    vect.push_back(QPixmap((source_path + "/sprites/monsters/eb_migalka2a.png").c_str()));
    m_animated_textures.push_back(unique_ptr<Animated_Texture>(new Animated_Texture(0, 6 * 48, vect)));
    m_enemies = vector<unique_ptr<Enemy>>();
    m_enemies.push_back(unique_ptr<Enemy>(new Robot(10 * 48, 6 * 48, 5, m_matrix, &m_enemy_bullets)));
    m_enemies.push_back(unique_ptr<Enemy>(new Cannon(3 * 48, 7 * 48, 270, &m_enemy_bullets)));
    m_teleports.push_back(unique_ptr<Teleport>(new Teleport(9 * 48, 2 * 48, 9 * 48, 6 * 48)));
    m_teleports.push_back(unique_ptr<Teleport>(new Teleport(9 * 48, 6 * 48, 9 * 48, 2 * 48)));

    m_electro_man = unique_ptr<Electro_man>(new Electro_man(3 * 48, 2 * 48 + 10, 8, m_matrix));

    this->readMap(m_map_path);
}

void Game::readMap(string path) {
    vector<shared_ptr<Layer>> layers = Parser::parse_layers(QString(path.c_str()));
    for (int k = 0; k < static_cast<int>(layers.size()); k++) {
        shared_ptr<Layer> layer = layers[static_cast<size_t>(k)];
        string path = m_map_path + "/../" + layer->path;
        QPixmap sprite_sheet = QPixmap(path.c_str());
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 13; x++) {
                int type = layer->data[static_cast<size_t>((7 - y) * 13 + x)] - layer->firstgid;
                if (type >= 0) {
                    int sprite_x = type % layer->sheet_columns;
                    int sprite_y = type / layer->sheet_columns;
                    if (k == 0 && type < 12) {
                        for (int i = 0; i < 48; i++) {
                            for (int j = 0; j < 48; j++) {
                                m_matrix[x * 48 + i][static_cast<size_t>(y * 48 + j)] = 1;
                            }
                        }
                    }
                    m_textures.push_back(unique_ptr<Texture>
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
    for (int i = 0; i < static_cast<int>(m_enemy_bullets.size()); i++)
    {
        shared_ptr<Bullet>& bullet = m_enemy_bullets[static_cast<size_t>(i)];
        if (is_overlaid(bullet.get(), m_electro_man.get()))
        {
            m_enemy_bullets.erase(m_enemy_bullets.begin() + i);
            m_electro_man->die();
            i--;
        }
    }
    vector<shared_ptr<Bullet>>* player_bullets = m_electro_man->getBulletsPtr();
    for (int i = 0; i < static_cast<int>(player_bullets->size()); i++) {
        shared_ptr<Bullet>& bullet = (*player_bullets)[static_cast<size_t>(i)];
        for (int j = 0; j < static_cast<int>(m_enemies.size()); j++) {
            unique_ptr<Enemy>& enemy = m_enemies[static_cast<size_t>(j)];
            if (is_overlaid(bullet.get(), enemy.get()))
            {
                unique_ptr<QPixmap> dead_texture = enemy->die();
               // dead texture can be nullptr
                if (dead_texture != nullptr) {
                    //textures.push_back(new Texture(enemy->getX(), enemy->getY(), dead_texture, 0, 0));
                    m_textures.push_back(unique_ptr<Texture>(new Texture(enemy->getX(), enemy->getY(), *dead_texture)));
                }
                player_bullets->erase(player_bullets->begin() + i);
                m_enemies.erase(m_enemies.begin() + static_cast<long long>(j));
                i--;
                break;
            }
        }

        for (int j = 0; j < static_cast<int>(m_animated_textures.size()); j++) {
            unique_ptr<Animated_Texture>& animated_texture = m_animated_textures[static_cast<size_t>(j)];
            if (is_overlaid(bullet.get(), animated_texture.get()))
            {
                unique_ptr<QPixmap> dead_texture = animated_texture->destroy();
               // dead texture can be nullptr
                if (dead_texture != nullptr) {
                    m_textures.push_back(unique_ptr<Texture>(new Texture(animated_texture->getX(), animated_texture->getY(), *dead_texture)));
                }
                player_bullets->erase(player_bullets->begin() + i);
                m_animated_textures.erase(m_animated_textures.begin() + static_cast<long long>(j));
                i--;
                break;
            }
        }
    }

    for (int i = 0; i < static_cast<int>(m_enemies.size()); i++)
    {
        unique_ptr<Enemy>& enemy = m_enemies[static_cast<size_t>(i)];
        if (is_overlaid(enemy.get(), m_electro_man.get()) && enemy->is_solid())
        {
            m_electro_man->die();
        }
    }
}

void Game::moveBullets() {
    for (int i = 0; i < static_cast<int>(m_enemy_bullets.size()); i++) {
        if (!m_enemy_bullets[static_cast<size_t>(i)]->move(m_matrix)) {
            m_enemy_bullets.erase(m_enemy_bullets.begin() + i);
            i--;
        }
    }
}

void Game::checkItems() {
    for (int i = 0; i < static_cast<int>(m_ammo.size()); i++)
    {
        unique_ptr<Ammo>& item = m_ammo[static_cast<size_t>(i)];
        if (is_overlaid(item.get(), m_electro_man.get())) {
            m_ammo.erase(m_ammo.begin() + i);
            i--;
        }
    }
}

void Game::checkTeleports() {
    for (auto&& teleport : m_teleports) {
        if (is_overlaid(m_electro_man.get(), teleport.get())) {
            m_electro_man->teleportation(teleport->getDestinationX(), teleport->getDestinationY());
            return;
        }
    }
}

void Game::timerTick() {
    m_electro_man->handleKeys(m_buttoms);
    for (auto&& enemy : m_enemies) {
        enemy->update();
    }
    if (m_buttoms->s) {
        checkTeleports();
    }
    moveBullets();
    checkHits();
    checkItems();
    this->update();
}

void Game::paintEvent(QPaintEvent* ) {
    QPainter painter(this);
    for (auto&& texture : m_textures) {
        texture->draw(painter);
    }
    for (auto&& texture : m_animated_textures) {
        texture->draw(painter);
    }
    for (auto&& teleport : m_teleports) {
        teleport->draw(painter);
    }
    for (auto&& enemy : m_enemies) {
        enemy->draw(painter);
    }
    for (auto&& bullet : m_enemy_bullets) {
        bullet->draw(painter);
    }
    for (auto&& item : m_ammo) {
        item->draw(painter);
    }
    m_electro_man->draw(painter);
}

void Game::keyPressEvent(QKeyEvent* e) {
    switch (e->key())
    {
    case (Qt::Key_W): {
        m_buttoms->w = true;
        break;
    }
    case (Qt::Key_S): {
        m_buttoms->s = true;
        break;
    }
    case (Qt::Key_A): {
        m_buttoms->a = true;
        break;
    }
    case (Qt::Key_D): {
        m_buttoms->d = true;
        break;
    }
    case (Qt::Key_Space): {
        m_buttoms->fire = true;
        break;
    }
    }
};

void Game::keyReleaseEvent(QKeyEvent* e) {
    switch (e->key())
    {
    case (Qt::Key_W): {
        m_buttoms->w = false;
        break;
    }
    case (Qt::Key_S): {
        m_buttoms->s = false;
        break;
    }
    case (Qt::Key_A): {
        m_buttoms->a = false;
        break;
    }
    case (Qt::Key_D): {
        m_buttoms->d = false;
        break;
    }
    case (Qt::Key_Space): {
        m_buttoms->fire = false;
        break;
    }
    }
}

Game::~Game()
{
    delete ui;
}
