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
    void addObject(GameObject* obj);        // 添加对象
    void removeObject(GameObject* obj);     // 移除对象
    int getMoney() const {return money_;} // 获取玩家当前金币值
    int getLife() const {return life_;} // 获取玩家当前生命值
    void addMoney(int amount) {money_ += amount;} // 添加金币
    void loseLife(int amount) {
        life_ -= amount; // 失去生命值
        if(life_ <= 0) emit gameOver();
    }

signals:
    void gameOver(); // 游戏结束
    void enemyDead(int reward); // 敌人死亡，奖励金币

public slots:
    void update(); // 调用函数，统一更新所有对象

private:
    QTimer* timer_; // 计时器
    QVector<GameObject*> objects_; // 游戏的对象容器
    QVector<GameObject*> pendingRemove_; // 等待删除的对象
    bool isRunning_; // 游戏是否运行
    int money_; // 玩家金币
    int life_; // 玩家生命值
};

#endif // GAMEMANAGER_H
