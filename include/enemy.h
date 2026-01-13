#ifndef ENEMY_H
#define ENEMY_H

#include "gameentity.h"
#include <QTimer>
#include <QVector>

class Enemy : public GameEntity
{
    Q_OBJECT
public:
    explicit Enemy(int enemyType = 0, QObject *parent = nullptr);
    ~Enemy();

    void update() override;
    void setPath(const QVector<QPointF>& pathPoints);
    void moveAlongPath();

    int getReward() const { return reward; }
    bool isAtEnd() const { return reachedEnd; }
    int getEnemyType() const { return enemyType; }

private slots:
    void onMoveTimer();
signals:
    void reachedEndPoint();
    void enemyKilled(int reward);
private:
    int enemyType;
    int reward;
    float speed;
    QVector<QPointF> pathPoints;
    int currentPathIndex;
    QTimer *moveTimer;
    bool reachedEnd;
};

#endif
