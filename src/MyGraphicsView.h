#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMouseEvent>
#include <QPaintEvent>

#include "MyCollisionBall.h"

#include <vector>

class MyGraphicsView : public QGraphicsView {

    Q_OBJECT

private:

    // Scene 
    int minX;
    int maxX;
    int minY;
    int maxY;

    // Info
    unsigned long long collisionTimes;

    // Render
    QTimer* clock;

    // Misc
    int mouseIndex;
    bool mouseTracking;
    bool mouseEliminate;
    bool mouseAddBall;

    int newBallVeloX;
    int newBallVeloY;
    int newBallMass;


public:
    MyGraphicsView(QWidget* parent) :
        QGraphicsView{ parent } {

        this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

        this->setFixedSize(1280, 960);
        this->setSceneRect(0, 0, 1280, 960);

        this->mouseIndex = 1;

        this->clock = new QTimer;
        this->clock->setTimerType(Qt::TimerType::PreciseTimer);
        this->clock->setInterval(15);

        this->setInteractive(true);

        connect(this->clock, &QTimer::timeout, this, &MyGraphicsView::updateAll);

    }
    ~MyGraphicsView() {

    }

    inline void setMouseTracking(bool status) {
        this->mouseTracking = status;
    }
    inline bool mouseTrackingEnabled() { return this->mouseTracking; }

    inline void setMouseEliminate(bool status) {
        this->mouseEliminate = status;
    }
    inline bool mouseEliminateEnabeld() { return this->mouseEliminate; }

    inline bool setMouseAddBall(bool status) {
        this->mouseAddBall = status;
    }
    inline bool mouseAddBallEnabled() { return this->mouseAddBall; }

    void initAll() {
        minX = 0;
        minY = 0;
        maxX = this->width();
        maxY = this->height();
        collisionTimes = 0;
    }

protected:
    // Events
    void paintEvent(QPaintEvent* event) override {
        QGraphicsView::paintEvent(event);
        QPainter painter{ this->viewport() };
        QPen outline{};
        outline.setColor(Qt::white);
        outline.setWidth(5);
        painter.setPen(outline);
        painter.drawRect(this->viewport()->rect());
    }

    void mousePressEvent(QMouseEvent* event) override {

    }

private slots:
    void updateAll() {

    }


public slots:
    void startAll() {

    }

};

