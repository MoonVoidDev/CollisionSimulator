#pragma once

#include <QGraphicsView>

class MyGraphicsView : public QGraphicsView {

    Q_OBJECT

public:
    MyGraphicsView(QWidget* parent) :
        QGraphicsView{ parent } {

    }
    ~MyGraphicsView() {

    }

protected:
    // Events


private slots:


private:


};

