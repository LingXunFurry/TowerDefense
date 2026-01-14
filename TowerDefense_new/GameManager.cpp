#include "GameManager.h"
#include <QDebug>

GameManager::GameManager(QObject *parent)
    : QObject(parent),
    timer_(nullptr),
    isRunning_(false),
    money_(100),
    life_(10)
{
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &GameManager::update);
    timer_->start(16); // 60FPS
}

void GameManager::update() {
    if(!isRunning_) return;

    // 更新所有对象
    for (GameObject* &obj : objects_) {
        obj->update();
    }

    processPendingRemove();
}

void GameManager::processPendingRemove(){
    for(GameObject*& obj : pendingRemove_){
        objects_.removeOne(obj);
        delete obj;
    }
    pendingRemove_.clear();
}

void GameManager::startGame(){
    if(isRunning_) return;
    isRunning_ = true;
    timer_->start(16);
}

void GameManager::pauseGame(){
    if(!isRunning_) return;
    isRunning_ = false;
    timer_->stop();
}

// 添加对象函数逻辑
void GameManager::addObject(GameObject *obj) {
    obj->setParent(this);
    objects_.push_back(obj);
}

// 移除对象逻辑
void GameManager::removeObject(GameObject *obj) {
    // 防止多次移除同一目标
    if(!pendingRemove_.contains(obj)){
        pendingRemove_.push_back(obj);
    }
}

// 损失生命值
void GameManager::loseLife(int amount){
    life_ -= amount;
    if(life_ <= 0){
        life_ = 0; // 防止生命值为负数
        emit gameOver();    // 游戏结束
    }
}

// 放置防御塔逻辑
void GameManager::requestPlaceTower(const QPointF& pos, int price){
    if(money_ >= price){
        money_ -= price;
        emit towerPlaceRequested(pos);
    } else {
        qDebug() << "Not enough money to place tower!";
    }
}

// 重启游戏
void GameManager::resetGame(){
    pauseGame();

    for(GameObject*& obj : objects_) delete obj;
    objects_.clear();
    pendingRemove_.clear();

    money_ = 100;
    life_ = 10;

    startGame();
}

// 卖出逻辑（需要tower类配合根据等级计算价格）
void GameManager::sellTower(GameObject* tower) {
    Tower* t = dynamic_cast<Tower*>(tower);
    if(t && objects_.contains(t)) {
        money_ += t->getSellValue();
        removeObject(t);
    }
}

// 升级tower
void GameManager::upgradeTower(GameObject* tower) {
    if(objects_.contains(tower)) {
        qDebug() << "Tower upgraded (logic implemented in Tower module)";
    }
}
