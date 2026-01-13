#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QObject>
#include <QPixmap>
#include <QMap>
#include <QString>

class ResourceManager : public QObject
{
    Q_OBJECT

public:
    static ResourceManager& instance();

    bool loadResources();
    QPixmap getPixmap(const QString& name) const;

    // 默认图片
    QPixmap getDefaultEnemyPixmap() const;
    QPixmap getDefaultTowerPixmap() const;
    QPixmap getDefaultBackground() const;
    QPixmap getGameMap() const;
    QPixmap getDefaultBulletPixmap() const;

private:
    explicit ResourceManager(QObject *parent = nullptr);
    ~ResourceManager() = default;

    QMap<QString, QPixmap> pixmapCache;

    void loadDefaultPixmaps();
};

#endif
