#include "unit.h"

bool Unit::is_solid() {
    return this->m_solid;
}

Unit::~Unit() {

}

Unit_Manager::Unit_Manager() {
    m_units = vector<shared_ptr<Unit>>();
}

shared_ptr<Unit> Unit_Manager::getItem(int index) {
    if (index < static_cast<int>(m_units.size())) {
        return m_units[static_cast<size_t>(index)];
    }
    else {
        return nullptr;
    }
}

void Unit_Manager::addItem(shared_ptr<Unit> ptr) {
    for (auto unit : m_units) {
        if (unit == ptr) {
            return;
        }
    }
    m_frame_counts.push_back((rand() % m_max_frame) - 1);
    m_units.push_back(ptr);
}


void Unit_Manager::addItems(vector<shared_ptr<Unit>> vector) {
    for (auto ptr : vector) {
        addItem(ptr);
    }
}

void Unit_Manager::deleteItem(int index) {
    if (index < static_cast<int>(m_units.size())) {
        m_units.erase(m_units.begin() + index);
        m_frame_counts.erase(m_frame_counts.begin() + index);
    }
}

size_t Unit_Manager::getSize() {
    return m_units.size();
}

void Unit_Manager::clear() {
    m_units.clear();
    m_frame_counts.clear();
}

void Unit_Manager::draw(QPainter& painter) {
    for (shared_ptr<IDrawable> unit : m_units) {
        unit->draw(painter);
    }
}

Unit_Manager::~Unit_Manager() {

}
