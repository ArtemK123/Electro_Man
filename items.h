#ifndef ITEMS_H
#define ITEMS_H

#include "base_classes.h"

class Ammo : public Animated_Object {
public:
    Ammo(int x, int y);
    ~Ammo() override;
};

class Teleport : public Animated_Object {
protected:
    int m_destination_x;
    int m_destination_y;

public:
    int getDestinationX();
    int getDestinationY();
    void setDestionation(int x, int y);

    Teleport(int x, int y, int destination_x, int destination_y);
    ~Teleport() override;
};

#endif // ITEMS_H
