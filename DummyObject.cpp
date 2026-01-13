#include "DummyObject.h"

DummyObject::DummyObject(QObject* parent)
    :GameObject(parent),
    counter_(0)
{
}

void DummyObject::update(){
    counter_++;
    qDebug() << "DummyObject update, counter = " << counter_;
}
