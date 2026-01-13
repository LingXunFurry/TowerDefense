#ifndef DUMMYOBJECT_H
#define DUMMYOBJECT_H

#include "GameObject.h"
#include <QDebug>

class DummyObject : public GameObject{
    Q_OBJECT
public:
    explicit DummyObject(QObject* parent = nullptr);

    void update() override;

private:
    // 测试计数
    int counter_;
};

#endif // DUMMYOBJECT_H
