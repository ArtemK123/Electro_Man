#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include "classes.h"
#include "enemies.h"
#include "bullets.h"
#include "player.h"

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
    QTimer* timer;
    const int width = 624;
    const int height = 384;
    int** matrix;
    Electro_man* electro_man;
    Pressed_Buttoms buttoms;
    std::vector<QPixmap*> texture_samples;
    std::vector<Texture*> textures;
    std::vector<Ammo*> ammo;
    std::vector<Enemy*> enemies;
    std::vector<Bullet*> enemy_bullets;

    void readMap(std::string path);
    void handleKeys();
    void moveBullets();
    void checkHits();
    void checkItems();
    bool is_overlaid(Object* a, Object* b);
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;
    void paintEvent(QPaintEvent* ) override;
    Ui::Game *ui;

private slots:
    void timerTick();
};

#endif // GAME_H
