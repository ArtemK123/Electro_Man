#ifndef UNIT_H
#define UNIT_H

#include "base_classes.h"

class Unit : public virtual Object, public IDrawable, public Destroyable_Object {
protected:
    bool m_solid;

public:
    virtual bool is_solid();
    virtual void draw(QPainter&) override = 0;
    ~Unit() override;
};

class Unit_Manager : public IObject_Manager<Unit>, public IDrawable {
protected:
    vector<shared_ptr<Unit>> m_units;
    vector<int> m_frame_counts;
    int m_max_frame;

public:
    shared_ptr<Unit> getItem(int index) override;
    size_t getSize() override;
    void addItem(shared_ptr<Unit> ptr) override;
    void addItems(vector<shared_ptr<Unit>> vect);
    void deleteItem(int index) override;
    void clear() override;
    void update() override = 0;
    void draw(QPainter& painter) override;

    Unit_Manager();
    ~Unit_Manager() override;
};

#endif // UNIT_H
