#include "include/gameentity.h"

GameEntity::GameEntity(EntityType type, QObject *parent)
    : QObject(parent), QGraphicsPixmapItem()
    , type(type)
    , health(100)
    , maxHealth(100)
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

void GameEntity::setHealth(int newHealth)
{
    health = qMax(0, qMin(newHealth, maxHealth));
}
