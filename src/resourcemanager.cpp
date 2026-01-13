#include "include/resourcemanager.h"

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

ResourceManager::ResourceManager(QObject *parent)
    : QObject(parent)
{
    loadDefaultPixmaps();
}

ResourceManager& ResourceManager::instance()
{
    static ResourceManager instance;
    return instance;
}

bool ResourceManager::loadResources()
{
    return true; // 使用默认图片
}

QPixmap ResourceManager::getPixmap(const QString& name) const
{
    return pixmapCache.value(name, QPixmap());
}

QPixmap ResourceManager::getDefaultEnemyPixmap() const
{
    QPixmap pixmap(30, 30);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(Qt::red));
    painter.setPen(QPen(Qt::darkRed, 2));
    painter.drawEllipse(2, 2, 26, 26);

    painter.setPen(QPen(Qt::white, 1));
    painter.drawEllipse(8, 8, 4, 4);
    painter.drawEllipse(18, 8, 4, 4);

    return pixmap;
}

QPixmap ResourceManager::getDefaultTowerPixmap() const
{
    QPixmap pixmap(40, 40);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // 塔基
    painter.setBrush(QBrush(QColor(139, 69, 19))); // 棕色
    painter.setPen(QPen(Qt::black, 1));
    painter.drawRect(10, 20, 20, 15);

    // 塔身
    painter.setBrush(QBrush(QColor(192, 192, 192))); // 银色
    painter.drawRect(15, 10, 10, 15);

    // 塔顶
    painter.setBrush(QBrush(Qt::yellow));
    painter.drawEllipse(18, 5, 4, 4);

    return pixmap;
}

QPixmap ResourceManager::getDefaultBackground() const
{
    QPixmap pixmap(800, 600);
    pixmap.fill(QColor(144, 238, 144)); // 浅绿色背景

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // 画网格
    painter.setPen(QPen(QColor(200, 255, 200), 1));
    for (int x = 0; x < 800; x += 40) {
        painter.drawLine(x, 0, x, 600);
    }
    for (int y = 0; y < 600; y += 40) {
        painter.drawLine(0, y, 800, y);
    }

    return pixmap;
}

QPixmap ResourceManager::getGameMap() const
{
    QPixmap mapPixmap(":/image/map.png");
    // mapPixmap.load();

    if (mapPixmap.isNull()) {
        // 如果找不到map.png，返回默认背景
        return getDefaultBackground();
    }

    // 将320x240的地图放大到800x600
    return mapPixmap.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

QPixmap ResourceManager::getDefaultBulletPixmap() const
{
    QPixmap pixmap(10, 10);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setBrush(QBrush(Qt::yellow));
    painter.setPen(QPen(QColor(255, 165, 0), 1));
    painter.drawEllipse(0, 0, 10, 10);
    return pixmap;
}

void ResourceManager::loadDefaultPixmaps()
{
    pixmapCache["enemy_default"] = getDefaultEnemyPixmap();
    pixmapCache["tower_default"] = getDefaultTowerPixmap();
    pixmapCache["background_default"] = getDefaultBackground();

    // 创建子弹图片
    QPixmap bulletPixmap(10, 10);
    bulletPixmap.fill(Qt::transparent);
    QPainter painter(&bulletPixmap);
    painter.setBrush(QBrush(Qt::blue));
    painter.setPen(QPen(Qt::darkBlue, 1));
    painter.drawEllipse(0, 0, 10, 10);
    pixmapCache["bullet_default"] = bulletPixmap;
}
