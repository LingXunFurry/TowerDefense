#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QTimer>

#include "enemy.h"
#include "tower.h"

class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);
    ~GamePage();

    void startGame();
    void pauseGame();
    void resetGame();

signals:
    void gameOver();
    void returnToMainMenu();

public slots:
    void spawnEnemy();
    void updateGame();
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    void initGameScene();
    void initUI();
    void createPath();
    void checkGameOver();

    // 绘制游戏元素
    void drawBackground();
    void drawGrid();

    // 游戏逻辑
    void updateEnemies();
    void updateTowers();
    void removeDeadEntities();

    // 鼠标事件
    void mousePressEvent(QMouseEvent *event) override;

    // 游戏状态
    QGraphicsScene *gameScene;
    QGraphicsView *gameView;

    // 游戏元素容器
    QList<Enemy*> enemies;
    QList<Tower*> towers;

    // 游戏资源
    int gold;
    int lives;
    int currentWave;
    int enemiesSpawnedThisWave;

    // 定时器
    QTimer *gameTimer;
    QTimer *enemySpawnTimer;

    // UI元素
    QWidget *controlPanel;
    QLabel *goldLabel;
    QLabel *livesLabel;
    QLabel *waveLabel;
    QPushButton *pauseButton;
    QPushButton *returnButton;

    // 布局
    QVBoxLayout *mainLayout;
    QHBoxLayout *infoLayout;
    QHBoxLayout *buttonLayout;

    // 路径点
    QVector<QPointF> pathPoints;

    bool isPaused;
    bool gameRunning;
};

#endif
