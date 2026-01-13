#include "include/tower.h"
#include "include/enemy.h"
#include "include/bullet.h"
#include "include/resourcemanager.h"
#include "include/config.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QGraphicsScene>
#include <math.h>
#include <cmath>

Tower::Tower(TowerType type, QPointF position, QObject *parent)
    : GameEntity(TOWER, parent)
    , towerType(type)
    , currentTarget(nullptr)
    , gameScene(nullptr)
{
    // 设置基础属性
    switch(type) {
    case ARROW_TOWER:
        damage = GameConfig::TowerStats::ARROW_DAMAGE;
        range = GameConfig::TowerStats::ARROW_RANGE;
        cost = GameConfig::TowerStats::ARROW_COST;
        fireRate = GameConfig::TowerStats::ARROW_FIRE_RATE;
        break;
    case CANNON_TOWER:
        damage = GameConfig::TowerStats::CANNON_DAMAGE;
        range = GameConfig::TowerStats::CANNON_RANGE;
        cost = GameConfig::TowerStats::CANNON_COST;
        fireRate = GameConfig::TowerStats::CANNON_FIRE_RATE;
        break;
    case MAGIC_TOWER:
        damage = 30;
        range = 100;
        cost = 150;
        fireRate = 1500;
        break;
    }

    // 设置图片
    ResourceManager& rm = ResourceManager::instance();
    setPixmap(rm.getDefaultTowerPixmap());
    setPos(position);

    // 攻击计时器
    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &Tower::onAttackTimer);
    attackTimer->start(fireRate);

    setHealth(100);
}

Tower::~Tower()
{
    attackTimer->stop();
}

void Tower::update()
{
    if (currentTarget && !isInRange(currentTarget)) {
        currentTarget = nullptr;
    }

    if (!currentTarget) {
        findTarget();
    }
}

void Tower::setTarget(Enemy* target)
{
    currentTarget = target;
}

void Tower::fire()
{
    if (currentTarget && gameScene) {
        // 创建子弹对象（不设置parent，以便子弹自己管理生命周期）
        Bullet* bullet = new Bullet(this->pos(), currentTarget, damage, nullptr);
        gameScene->addItem(bullet);
        emit fired();
    }
}

void Tower::setEnemiesInRange(const QList<Enemy*>& enemies)
{
    enemiesInRange = enemies;
    findTarget();
}

void Tower::onAttackTimer()
{
    if (currentTarget && isInRange(currentTarget)) {
        fire();
    } else {
        findTarget();
        if (currentTarget) {
            fire();
        }
    }
}

bool Tower::isInRange(Enemy* enemy) const
{
    if (!enemy) return false;

    qreal dx = enemy->x() - this->x();
    qreal dy = enemy->y() - this->y();
    qreal distance = std::sqrt(dx*dx + dy*dy);  // 使用 std::sqrt

    return distance <= range;
}

void Tower::findTarget()
{
    Enemy* closestEnemy = nullptr;
    qreal minDistance = range + 1; // 初始化为超出范围

    for (Enemy* enemy : enemiesInRange) {
        if (enemy && isInRange(enemy)) {
            qreal dx = enemy->x() - this->x();
            qreal dy = enemy->y() - this->y();
            qreal distance = std::sqrt(dx*dx + dy*dy);  // 使用 std::sqrt

            if (distance < minDistance) {
                minDistance = distance;
                closestEnemy = enemy;
            }
        }
    }

    currentTarget = closestEnemy;
}
