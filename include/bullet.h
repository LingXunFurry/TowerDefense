#ifndef BULLET_H
#define BULLET_H

#include "gameentity.h"
#include "enemy.h"
#include <QTimer>
#include <QPointer>

class Bullet : public GameEntity
{
    Q_OBJECT
public:
    explicit Bullet(QPointF startPos, Enemy* target, int damage, QObject *parent = nullptr);
    ~Bullet();

    void update() override;
    Enemy* getTarget() const { return target; }

private slots:
    void onMoveTimer();

private:
    QPointer<Enemy> target;  // 使用QPointer自动跟踪target的生命周期
    int damage;
    float speed;
    QTimer* moveTimer;
};

#endif
