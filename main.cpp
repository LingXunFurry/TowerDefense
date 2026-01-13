#include <QApplication>
#include "GameManager.h"
#include "DummyObject.h"

int main(int argc, char* argv[])
{
    QApplication a(argc,argv);

    GameManager manager;
    manager.startGame();

    DummyObject* obj = new DummyObject();
    manager.addObject(obj);

    return a.exec();
}
