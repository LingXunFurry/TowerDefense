#include "include/bullet.h"
#include "include/enemy.h"
#include "include/resourcemanager.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QGraphicsScene>
#include <cmath>
#include <QDebug>

Bullet::Bullet(QPointF startPos, Enemy* target, int damage, QObject *parent)
    : GameEntity(BULLET, parent)
    , target(target)
    , damage(damage)
    , speed(5.0f)
{
    // 设置子弹位置
    setPos(startPos);

    // 设置子弹图片
    ResourceManager& rm = ResourceManager::instance();
    setPixmap(rm.getDefaultBulletPixmap());

    // 移动计时器
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Bullet::onMoveTimer);
    moveTimer->start(30); // 每30ms移动一次

    qDebug() << "Bullet created at" << startPos << "targeting" << (target ? "valid" : "invalid");
}

Bullet::~Bullet()
{
    if (moveTimer) {
        moveTimer->stop();
    }
}

void Bullet::update()
{
    onMoveTimer();
}

void Bullet::onMoveTimer()
{
    // 如果目标被销毁或无效，删除子弹
    if (!target) {
        if (scene()) {
            scene()->removeItem(this);
        }
        deleteLater();
        return;
    }

    // 额外的安全检查：确保target仍然有效
    if (target.isNull()) {
        if (scene()) {
            scene()->removeItem(this);
        }
        deleteLater();
        return;
    }

    QPointF currentPos = pos();
    QPointF targetPos = target->pos();
    QPointF direction = targetPos - currentPos;
    qreal distance = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());

    // 如果接近目标，造成伤害
    if (distance < speed + 15) {
        qDebug() << "Bullet hit target! Dealing" << damage << "damage";
        target->setHealth(target->getHealth() - damage);
        if (scene()) {
            scene()->removeItem(this);
        }
        deleteLater();
        return;
    }

    // 向目标移动
    if (distance > 0) {
        direction /= distance;
        setPos(currentPos + direction * speed);
    }
}
