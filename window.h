#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "game.h"

namespace Ui {
class Window;
}


class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window() override;

private:
    unique_ptr<QTimer> m_timer;
    Ui::Window *ui;
    Game& game = Game::instance();

    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;
    void paintEvent(QPaintEvent* ) override;

private slots:
    void timerTick();
};

#endif // WINDOW_H
