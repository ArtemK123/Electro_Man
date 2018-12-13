#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include "base_classes.h"
#include "enemies.h"
#include "bullets.h"
#include "player.h"
#include "textures.h"
#include "xml.h"
#include "items.h"

namespace Ui {
class Game;
}


class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game() override;

private:
    unique_ptr<QTimer> m_timer;
    const int m_width = 624;
    const int m_height = 384;
    shared_ptr<unique_ptr<int[]>[]> m_matrix;
    Ui::Game *ui;
    string m_map_path;
    unique_ptr<Electro_man> m_electro_man;
    shared_ptr<Pressed_Buttoms> m_buttoms;
    vector<unique_ptr<Texture>> m_textures;
    vector<unique_ptr<Animated_Texture>> m_animated_textures;
    vector<unique_ptr<Ammo>> m_ammo;
    vector<unique_ptr<Enemy>> m_enemies;
    vector<shared_ptr<Bullet>> m_enemy_bullets;
    vector<unique_ptr<Teleport>> m_teleports;

    void readTxtMap(string path);
    void readMap(string path);
    void handleKeys();
    void moveBullets();
    void checkHits();
    void checkItems();
    void checkTeleports();
    bool is_overlaid(Object* a, Object* b);
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;
    void paintEvent(QPaintEvent* ) override;

private slots:
    void timerTick();
};

#endif // GAME_H
