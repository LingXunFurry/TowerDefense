#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

// 必须头文件
#include <QObject>
#include <QTimer>
#include <QVector>
#include "GameObject.h"

// 核心管理类，实现游戏时钟，统一管理所有对象
class GameManager : public QObject {
    Q_OBJECT
public:
    // 创建对象函数，全部挂到GameManager下，这样可以一同销毁
    explicit GameManager(QObject* parent = nullptr);

    // 游戏控制接口
    void startGame();       // 开始游戏
    void pauseGame();       // 暂停游戏
    void resetGame();       // 重置游戏

    // 游戏对象管理
    void addObject(GameObject* obj);        // 添加对象
    void removeObject(GameObject* obj);     // 移除对象

    // 玩家状态
    int getMoney() const {return money_;} // 获取玩家当前金币值
    int getLife() const {return life_;} // 获取玩家当前生命值
    void addMoney(int amount) {money_ += amount;} // 添加金币
    void loseLife(int amount); // 失去生命
    void rewardPlayer(int amount){addMoney(amount);} // 奖励玩家金币

    // 鼠标交互
    void requestPlaceTower(const QPointF& pos, int price);  // 接收放置请求，检查金币发出信号建造防御塔
    bool canPlaceTower(const QPointF& pos) const;

    // 塔操作
    void sellTower(GameObject* tower); // 售卖防御塔
    void upgradeTower(GameObject* tower);

signals:
    void gameOver(); // 游戏结束
    void gameWin(); // 胜利
    void enemyDead(int reward); // 敌人死亡，奖励金币
    void towerPlaceRequested(const QPointF& pos); // 创建防御塔信号

public slots:
    void update(); // 调用函数，统一更新所有对象

private:
    QTimer* timer_; // 计时器
    QVector<GameObject*> objects_; // 游戏的对象容器
    QVector<GameObject*> pendingRemove_; // 待删除对象
    void processPendingRemove(); // 待删除对象函数
    bool isRunning_; // 游戏是否运行
    int money_; // 玩家金币
    int life_; // 玩家生命值
};

#endif // GAMEMANAGER_H
