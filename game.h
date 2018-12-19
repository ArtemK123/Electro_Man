#ifndef GAME_H
#define GAME_H

#include "base_classes.h"
#include "bullets.h"
#include "player.h"
#include "textures.h"
#include "xml.h"
#include "items.h"
#include "cannon.h"
#include "robot.h"
#include "unit.h"

class Game : ILogic, IInput_Handler, IDrawable {
protected:
    Game();
    ~Game() override;
    Game& operator=(Game const&) = delete;
    Game(Game const&) = delete;


public:
    const int board_width = 624;
    const int board_height = 384;
    string m_map_path;
    shared_ptr<Matrix> m_matrix;
    shared_ptr<Electro_man> m_electro_man;
    shared_ptr<Pressed_Buttons> m_buttons;
    shared_ptr<Robot_Manager> m_robot_manager;
    shared_ptr<Cannon_Manager> m_cannon_manager;
    shared_ptr<Bullet_Manager> m_enemy_bullets;
    shared_ptr<Bullet_Manager> m_player_bullets;


    vector<shared_ptr<Texture>> m_textures;
    vector<shared_ptr<Animated_Texture>> m_animated_textures;
    vector<shared_ptr<Ammo>> m_ammo;
    vector<shared_ptr<Teleport>> m_teleports;
    vector<shared_ptr<IDrawable>> m_renders;
    vector<shared_ptr<ILogic>> m_logical;

    // Meyers singleton
    static Game& instance();

    void update() override;
    void readTxtMap(string path);
    void readMap(string path);
    void handleKeys(Pressed_Buttons& buttons) override;

    void makeLogic();
    void checkEnemyBulletsCollisions();
    void checkPlayerBulletsCollisions();
    void checkUnitCollisions();
    void checkItems();


    void draw(QPainter& painter) override;
};

#endif // GAME_H
