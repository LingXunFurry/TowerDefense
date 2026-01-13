#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QPointF>
#include <QPainter>

// 基类框架，供子类继承
class GameObject : public QObject{
    Q_OBJECT
public:
    explicit GameObject(QObject* parent = nullptr): QObject(parent){}
    virtual ~GameObject(){}

    // 每一帧调用，子类实现具体逻辑，此处仅提供接口
    virtual void update() = 0;
};

#endif // GAMEOBJECT_H
