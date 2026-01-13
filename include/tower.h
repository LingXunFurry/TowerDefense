#ifndef TOWER_H
#define TOWER_H

#include "gameentity.h"
#include <QTimer>
#include <QList>

class Enemy;
class QGraphicsScene;

class Tower : public GameEntity
{
    Q_OBJECT
public:
    enum TowerType {
        ARROW_TOWER,
        CANNON_TOWER,
        MAGIC_TOWER
    };

    explicit Tower(TowerType type, QPointF position, QObject *parent = nullptr);
    ~Tower();

    void update() override;
    void setTarget(Enemy* target);
    void fire();
    void setGameScene(QGraphicsScene* scene) { gameScene = scene; }

    int getDamage() const { return damage; }
    int getRange() const { return range; }
    int getCost() const { return cost; }
    TowerType getTowerType() const { return towerType; }

    void setEnemiesInRange(const QList<Enemy*>& enemies);

private slots:
    void onAttackTimer();
signals:
    void fired();
    void targetDestroyed();
private:
    TowerType towerType;
    int damage;
    int range;
    int fireRate; // 毫秒
    int cost;
    Enemy* currentTarget;
    QTimer *attackTimer;
    QList<Enemy*> enemiesInRange;
    QGraphicsScene* gameScene;

    bool isInRange(Enemy* enemy) const;
    void findTarget();
};

#endif
