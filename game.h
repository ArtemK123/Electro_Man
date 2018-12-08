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
    unique_ptr<QTimer> timer;
    const int width = 624;
    const int height = 384;
    shared_ptr<unique_ptr<int[]>[]> matrix;
    Ui::Game *ui;
    string map_path;
    unique_ptr<Electro_man> electro_man;
    shared_ptr<Pressed_Buttoms> buttoms;
    vector<unique_ptr<Texture>> textures;
    vector<unique_ptr<Animated_Texture>> animated_textures;
    vector<unique_ptr<Ammo>> ammo;
    vector<unique_ptr<Enemy>> enemies;
    vector<shared_ptr<Bullet>> enemy_bullets;
    vector<unique_ptr<Teleport>> teleports;

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
