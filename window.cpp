#include "window.h"
#include "ui_window.h"
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <fstream>
#include <iostream>
#include <vector>


Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    this->setFixedSize(624, 384);
    m_timer = unique_ptr<QTimer>(new QTimer());
    connect(m_timer.get(), SIGNAL(timeout()), this, SLOT(timerTick()));
    m_timer->start(30);
}

void Window::timerTick() {
    game.update();
    this->update();
}

void Window::paintEvent(QPaintEvent* ) {
    QPainter painter(this);
    game.draw(painter);
}

void Window::keyPressEvent(QKeyEvent* e) {
    switch (e->key())
    {
    case (Qt::Key_W): {
        game.m_buttons->w = true;
        break;
    }
    case (Qt::Key_S): {
        game.m_buttons->s = true;
        break;
    }
    case (Qt::Key_A): {
        game.m_buttons->a = true;
        break;
    }
    case (Qt::Key_D): {
        game.m_buttons->d = true;
        break;
    }
    case (Qt::Key_Space): {
        game.m_buttons->fire = true;
        break;
    }
    }
};

void Window::keyReleaseEvent(QKeyEvent* e) {
    switch (e->key())
    {
    case (Qt::Key_W): {
        game.m_buttons->w = false;
        break;
    }
    case (Qt::Key_S): {
        game.m_buttons->s = false;
        break;
    }
    case (Qt::Key_A): {
        game.m_buttons->a = false;
        break;
    }
    case (Qt::Key_D): {
        game.m_buttons->d = false;
        break;
    }
    case (Qt::Key_Space): {
        game.m_buttons->fire = false;
        break;
    }
    }
}

Window::~Window()
{
    delete ui;
}
