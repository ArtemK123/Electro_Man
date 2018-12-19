#include "game.h"

Game::Game()
{
    this->m_map_path = source_path + "/levels/level1_1.tmx";

    m_textures = vector<shared_ptr<Texture>>();
    m_animated_textures = vector<shared_ptr<Animated_Texture>>();
    m_ammo = vector<shared_ptr<Ammo>>();
    m_teleports = vector<shared_ptr<Teleport>>();

    m_matrix = shared_ptr<Matrix>(new Matrix(board_width, board_height));
    m_renders = vector<shared_ptr<IDrawable>>();
    m_logical = vector<shared_ptr<ILogic>>();
    m_enemy_bullets = shared_ptr<Bullet_Manager>(new Bullet_Manager());
    m_cannon_manager = shared_ptr<Cannon_Manager>(new Cannon_Manager(m_enemy_bullets));
    m_robot_manager = shared_ptr<Robot_Manager>(new Robot_Manager(m_matrix, m_enemy_bullets));
    m_player_bullets = shared_ptr<Bullet_Manager>(new Bullet_Manager());

    m_buttons = shared_ptr<Pressed_Buttons>(new Pressed_Buttons());
    m_electro_man = shared_ptr<Electro_man>(new Electro_man(4 * 48, 2 * 48, 8, m_matrix, m_player_bullets, m_buttons));

    m_teleports.push_back(shared_ptr<Teleport>(new Teleport(9 * 48, 2 * 48, 9 * 48, 6 * 48)));
    m_teleports.push_back(shared_ptr<Teleport>(new Teleport(9 * 48, 6 * 48, 9 * 48, 2 * 48)));
    m_ammo.push_back(shared_ptr<Ammo>(new Ammo(6 * 48, 2 * 48)));


    this->readMap(m_map_path);

    //m_renders.push_back(m_electro_man);
    m_renders.push_back(m_robot_manager);
    m_logical.push_back(m_robot_manager);
    m_logical.push_back(m_cannon_manager);
    m_renders.push_back(m_cannon_manager);
    m_robot_manager->addItem(shared_ptr<Robot>(new Robot(6 * 48, 2 * 48, 5)));
    m_cannon_manager->addItem(shared_ptr<Cannon>(new Cannon(1 * 48, 7 * 48, 270)));
    m_renders.push_back(m_enemy_bullets);
    m_logical.push_back(m_enemy_bullets);
    m_renders.push_back(m_player_bullets);
    m_logical.push_back(m_player_bullets);
}

Game& Game::instance()
{
    static Game instance;
    return instance;
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
                                (*m_matrix)(x * 48 + i, y * 48 + j) = 1;
                            }
                        }
                    }
                    auto texture = shared_ptr<Texture>(new Texture(x * 48, y * 48, sprite_sheet.copy(sprite_x * 48, sprite_y * 48, 48, 48)));
                    m_textures.push_back(texture);
                    m_renders.push_back(texture);
                }
            }
        }
    }
}

void Game::update() {
    handleKeys(*m_buttons);
    makeLogic();
    checkEnemyBulletsCollisions();
    checkPlayerBulletsCollisions();
    checkUnitCollisions();
    checkItems();
}

void Game::handleKeys(Pressed_Buttons& buttons) {
    if (buttons.s) {
        for (auto teleport : m_teleports) {
            if (m_electro_man->checkObjectCollision(*teleport)) {
                m_electro_man->teleportation(teleport->getDestinationX(), teleport->getDestinationY());
                break;
            }
        }
    }
}

void Game::makeLogic() {
    m_electro_man->update();
    m_robot_manager->update();
    m_cannon_manager->update();
    m_enemy_bullets->update();
    m_player_bullets->update();
}

void Game::checkPlayerBulletsCollisions() {
    bool deleted = false;
    for (int i = 0; i < static_cast<int>(m_player_bullets->getSize()); i++) {
        shared_ptr<Bullet> bullet = m_player_bullets->getItem(i);
        if (bullet->checkMapCollision(*m_matrix)) {
            m_player_bullets->deleteItem(i);
            i--;
            deleted = true;
        }
        if (!deleted) {
            for (int j = 0; j < static_cast<int>(m_robot_manager->getSize()); j++) {
                shared_ptr<Unit> robot = m_robot_manager->getItem(j);
                if (bullet->checkObjectCollision(*robot)) {
                    m_robot_manager->deleteItem(j);
                    m_player_bullets->deleteItem(i);
                    i--;
                    deleted = true;
                    break;
                }
            }
        }
        if (!deleted) {
            for (int j = 0; j < static_cast<int>(m_cannon_manager->getSize()); j++) {
                shared_ptr<Unit> cannon = m_cannon_manager->getItem(j);
                if (bullet->checkObjectCollision(*cannon)) {
                    shared_ptr<QPixmap> destroyed_cannon = cannon->getDestroyedImage();
                    auto texture = shared_ptr<Texture>(
                        new Texture(cannon->getX(), cannon->getY(), *destroyed_cannon));
                    m_textures.push_back(texture);
                    m_renders.push_back(texture);

                    m_cannon_manager->deleteItem(j);
                    m_player_bullets->deleteItem(i);
                    i--;
                    deleted = true;
                    break;
                }
            }
        }
        if (!deleted) {
            for (int j = 0; j < static_cast<int>(m_animated_textures.size()); j++) {
                shared_ptr<Animated_Texture> a_texture = m_animated_textures[static_cast<size_t>(j)];
                if (bullet->checkObjectCollision(*a_texture)) {
                    shared_ptr<QPixmap> destroyed_texture = a_texture->getDestroyedImage();
                    auto texture = shared_ptr<Texture>(
                        new Texture(a_texture->getX(), a_texture->getY(), *destroyed_texture));
                    m_textures.push_back(texture);
                    m_renders.push_back(texture);
                }
            }
        }
    }
}

void Game::checkEnemyBulletsCollisions() {
    for (int i = 0; i < static_cast<int>(m_enemy_bullets->getSize()); i++) {
        shared_ptr<Bullet> bullet = m_enemy_bullets->getItem(i);
        if (bullet->checkMapCollision(*m_matrix)) {
            m_enemy_bullets->deleteItem(i);
            continue;
        }
        else if (bullet->checkObjectCollision(*m_electro_man)) {
            m_electro_man->die();
            m_enemy_bullets->deleteItem(i);
        }
    }
}

void Game::checkUnitCollisions() {
    for (int i = 0; i < static_cast<int>(m_robot_manager->getSize()); i++) {
        shared_ptr<Unit> robot = m_robot_manager->getItem(i);
        if (m_electro_man->checkObjectCollision(*robot)) {
            m_electro_man->die();
            break;
        }
    }
}

void Game::checkItems() {
    for (int i = 0; i < static_cast<int>(m_ammo.size()); i++) {
        if (m_electro_man->checkObjectCollision(*m_ammo[static_cast<size_t>(i)])) {
            m_ammo.erase(m_ammo.begin() + i);
        }
    }
}

void Game::draw(QPainter& painter) {
    for (shared_ptr<IDrawable> item : m_renders) {
        item->draw(painter);
    }
    for(auto a_texture : m_animated_textures) {
        a_texture->draw(painter);
    }
    for (auto teleport : m_teleports) {
        teleport->draw(painter);
    }
    for (auto ammo : m_ammo) {
        ammo->draw(painter);
    }
    m_electro_man->draw(painter);
}

Game::~Game()
{

}

