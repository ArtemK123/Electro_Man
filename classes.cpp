#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <QApplication>
#include <QPainter>

#include "classes.h"
using namespace std;


// gameboard = 624 x 384
// texture_block = 48 x 48

Object::Object(int x, int y) {
    this->x = x;
    this->y = y;
    this->width = 48;
    this->height = 48;
}

int Object::getWidth() {
    return this->width;
}

int Object::getHeight() {
    return this->height;
}

Object::~Object() {

}

Texture::Texture(int x, int y, QPixmap* image) : Object(x, y) {
    this->image = image;
}

void Texture::draw(QPainter* painter) {
    painter->drawPixmap(x, 384 - height - y, *image);
}

Texture::~Texture() {

}


Ammo::Ammo(int x, int y) : Object(x, y) {
    this->frame_count = 0;
    this->image = new QPixmap((source_path + "/sprites/level/energy.png").c_str());
}

void Ammo::draw(QPainter* painter) {
    if (frame_count < 10) {
        painter->drawPixmap(x, 384 - height - y, *image);
    }
    if (frame_count == 20) {
        frame_count = 0;
    }
    else {
        frame_count++;
    }
}

Ammo::~Ammo() {
    delete image;
}




























/*


// Object
 Object::Object (int x, int y, int width, int height) {
   this->x = x;
   this->y = y;
   this->width = width;
   this->height = height;
};

// Texture

Texture::Texture(int x, int y, QPixmap* image, int width = 48, int height = 48, bool is_solid = false) :
       Object(x, y, width, height)
{
   this->image = image;
   this->is_solid = is_solid;
};

Texture::Texture(int x, int y, char* path, int width = 48, int height = 48, bool is_solid = false) :
   Texture::Texture(x, y, new QPixmap(path), width, height, is_solid) {};

void Texture::draw() {

}

Texture::~Texture() {
   // ???? memory didn`t allocated
   delete image;
}

// Animated_Object

Animated_Object::Animated_Object(int x, int y, int animation_max, int width = 48,
                                  int height = 48, bool is_active = true) :
   Object(x, y, width, height)
{
   animations = vector<QPixmap*>();
   this->animation_max = animation_max;
   this->is_active = is_active;
}


void Animated_Object::animate() {
   if (current_animation < 0) {
       return;
   }
   if (is_active) {
       if (current_animation < static_cast<int>(animations.size())) {
           current_animation++;
       }
       else {
           current_animation = 0;
       }
   }
   this->draw();
}

bool Animated_Object::addImage(QPixmap* image_ptr) {
   if (static_cast<int>(animations.size()) == animation_max) {
       return false;
   }
   animations.push_back(image_ptr);
   return true;
};

void Animated_Object::clearImages() {
   animations.clear();
   current_animation = -1;
}

void Animated_Object::draw() {

};

Animated_Object::~Animated_Object() {
   for(auto ptr : animations) {
       delete ptr;
   }
}

// Animated_Texture

Animated_Texture::Animated_Texture(int x, int y, int animation_max, int width = 48,
                                  int height = 48, bool is_solid = false, bool is_active = true) :
   Object(x, y, width, height), Animated_Object(x, y, animation_max, width, height, is_active)
{
   this->is_solid = is_solid;
};

void Animated_Texture::draw() {

};

Animated_Texture::~Animated_Texture() {
   Animated_Object::~Animated_Object(); // they are the same
}

// Destroyable_Texture

Destroyable_Texture::Destroyable_Texture(int x, int y, int animation_max, int width = 48,
                                        int height = 48, bool is_solid = true, bool is_active = true) :
   Object(x, y, width, height), Animated_Texture(x, y, animation_max, width, height, is_solid, is_active) {    };

void Destroyable_Texture::destroy() {
   is_active = false;
}

Destroyable_Texture::~Destroyable_Texture() {
   Animated_Texture::~Animated_Texture(); //the are the same
}

// Movable_Object

Movable_Object::Movable_Object(int x, int y, int speed, int width = 48, int height= 48) :
   Object(x, y, width, height)
{
   this->speed = speed;
}

// Bullet

Bullet::Bullet(int x, int y, int animation_max, int max_speed, int max_distance = 624,
              int width = 48, int height = 48, bool is_fly_through = false) :
   Object(x, y, width, height), Animated_Object(x, y, animation_max, width, height),
   Movable_Object(x, y, max_speed, width, height)
{
   this->max_distance = max_distance;
   this->is_fly_through = is_fly_through;
   this->current_distance = 0;
};

void Bullet::move() {

};

Bullet::~Bullet() {
   Animated_Object::~Animated_Object();
}

// Unit

Unit::Unit(int x, int y, int animation_max, int width = 48, int height = 48) :
   Object(x, y, width, height), Animated_Object (x, y, animation_max, width, height)
{

}


Unit::~Unit() {
   Animated_Object::~Animated_Object();
}

// Cannon

Cannon::Cannon(int x, int y, int direction, int animation_max = 2, int width = 48, int height = 48) :
   Object(x, y, width, height), Unit(x, y, animation_max, width, height)
{
   this->direction = direction;
}

void Cannon::shoot() {

};

void Cannon::destroy() {
   is_active = false;
};

Cannon::~Cannon() {
   Unit::~Unit();
};


class Enemy : public Unit {
public:
   void move() {

   }
   void shoot() override {

   }
};

class Electroman : public Unit {
public:
   int gun;
   int ammo;
   int overloading;
   int cards;
   void* moving_sound;
   void* jump_sound;
   void* landing_sound;
   void jump() {

   }
   void shoot() override {

   }
};

class Respawn : public Texture {
public:
   void* sound;
   Electroman* electroman;
   void respawn() {

   }
};

class Teleport : public Animated_Texture {
public:
   Object* destination;
   void teleport () {

   }
};


class Item : public Object {
public:
   bool is_taken () {
      return false;
   }
};

class Ammo : public Item {

};

class Card : public Item {

};

class Room {
public:
   // gameboard = 650 x 400; 13 x 8 blocks
   // texture_block = 50 x 50
   vector<Object*> objects = vector<Object*>();
   vector<Texture*> textures_0;
   vector<Texture*> textures_1;
   int** matrix;
   const int height = 400;
   const int width = 650;
   const int block_size = 50;
   vector<Room*> available_rooms = vector<Room*>();
   bool object_delete(Object* ptr) {
       return true;
   }

   bool object_add(Object* ptr) {
       return true;
   }

   void draw() {

   }

   string texture_folder = "E:/Developing/C++/Projects/Electroman/electro_man_remake/electro_man_remake/sprites/level";

   // 0 = bricks
   // 1 = column
   // 2 = hole

   Room(string texture_file_path, string object_file_path) {
       ifstream texture_file(texture_file_path);
       int texture_type;
       for(int i = 0; i < 13; i++) {
           for(int j = 0; j < 8; j++) {
              texture_file >> texture_type;
              if (texture_type) {
                  textures_0.push_back(new Texture(
                                           i * block_size, j * block_size,
                                           "E:/Developing/C++/Projects/Electroman/Electroman/source_files/sprites/level/tile2.png",
                                           block_size, block_size, true));
              }
              else {
                  textures_0.push_back(new Texture(
                                           i * block_size, j * block_size,
                                           "E:/Developing/C++/Projects/Electroman/Electroman/source_files/sprites/level/tile1.png",
                                           block_size, block_size, false));
              }
           }
       }

   }

   ~Room() {
       for (Texture* ptr : textures_0) {
           delete ptr;
       }

       for (Object* ptr : objects) {
           delete ptr;
       }
       for (int i = 0; i < width; i++) {
           delete matrix[i];
       }
       delete matrix;
   }

};

class Map {
public:
   vector<Room*> rooms = vector<Room*>();
   Room* current_room;
   Map(string input_path) {

   }
}; */

