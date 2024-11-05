#pragma once

#include <QGraphicsScene>
#include <QObject>

class MyGraphicsScene : public QGraphicsScene {

public:
    MyGraphicsScene(QObject* parent) :
        QGraphicsScene{ parent } {

    }
    ~MyGraphicsScene() {

    }

protected:


private slots:


private:


};