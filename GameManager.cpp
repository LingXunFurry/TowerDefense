#include "GameManager.h"

GameManager::GameManager(QObject *parent)
    : QObject(parent),
      timer_(nullptr),
      isRunning_(false),
      money_(0),
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

    // 处理待删除对象
    for (GameObject* &obj : pendingRemove_) {
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


