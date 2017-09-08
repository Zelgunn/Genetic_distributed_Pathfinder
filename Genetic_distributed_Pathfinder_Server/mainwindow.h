#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QLabel>

#include "world.h"
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initUI();
    void showGridOnUI();
    void drawVillagersWay(Humain villager);
    World *world();
    void setUIVillageCount(int count);

public slots:
    void setVillageCount(int count);
    void updateWorld();

private slots:
    void on_actionReg_n_rer_triggered();
    void on_actionContinuer_la_g_n_ration_triggered();

private:
    Ui::MainWindow *ui;
    World m_world;
    QGraphicsPixmapItem* m_image;
    QImage m_baseImage;
    int m_worldWidth;
    // Status bar
    QLabel m_villageCount;
    QLabel m_obstaclesCount;
    Server *m_s;
};

#endif // MAINWINDOW_H
