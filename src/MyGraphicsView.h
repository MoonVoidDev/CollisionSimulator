#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDebug>

#include "MyCollisionBall.h"

#include <vector>

class MyGraphicsView : public QGraphicsView {

    Q_OBJECT

private:

    // Scene 
    int maxX{};
    int maxY{};

    // Info
    unsigned long long collisionTimes{};

    // Render
    QTimer* clock{};

    // Misc
    int mouseIndex{ 0 };
    bool m_mouseBallTracking{};
    bool m_mouseEliminate{};
    bool m_mouseAddBall{};

    int newBallVeloX{};
    int newBallVeloY{};
    int newBallMass{};

    // Algorithms

    std::vector<MyCollisionBall*> objBalls{};
    int objCnt{};

    struct QuadtreeNode {
        int ls, rs;

    };



protected:

    void addNewBall() {

    }

public:
    MyGraphicsView(QWidget* parent) :
        QGraphicsView{ parent } {

        // Widget
        this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        this->setFixedSize(1280, 960);
        this->setSceneRect(0, 0, 1280, 960);

        // Misc
        this->mouseIndex = 0;
        maxX = this->width();
        maxY = this->height();
        collisionTimes = 0;

        // Render
        this->clock = new QTimer;
        this->clock->setTimerType(Qt::TimerType::PreciseTimer);
        this->clock->setInterval(15);

        // Optimization
        this->setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::FullViewportUpdate);

        // Algorithms
        objBalls.resize(1024);
        MyCollisionBall* a{ new MyCollisionBall{200,200,30} };
        objBalls[0] = a;
        a->setIsMouse(true);
        a->setVeloV(2000, 2000);
        objCnt++;

        // Connects
        connect(this->clock, &QTimer::timeout, this, &MyGraphicsView::updateAll);

    }
    ~MyGraphicsView() = default;

    void setMouseBallTracking(bool status) { this->m_mouseBallTracking = status; }
    void setMouseEliminate(bool status) { this->m_mouseEliminate = status; }
    void setMouseAddBall(bool status) { this->m_mouseAddBall = status; }
    inline bool mouseBallTracking(bool status) { return this->m_mouseBallTracking; }
    inline bool mouseEliminate(bool status) { return this->m_mouseEliminate; }
    inline bool mouseAddBall(bool status) { return this->m_mouseAddBall; }

protected:
    // Event handlers
    void paintEvent(QPaintEvent* event) override {
        QGraphicsView::paintEvent(event);
        QPainter painter{ this->viewport() };
        QPen outline{};
        outline.setColor(Qt::white);
        outline.setWidth(5);
        painter.setPen(outline);
        painter.drawRect(this->viewport()->rect());
        painter.setFont(QFont{ "Consolas" });
        painter.drawText(20, 20, QString::number(this->collisionTimes));
    }

    void mousePressEvent(QMouseEvent* event) override {
        qDebug() << "[VIEW] mousePress";
        if (event->buttons() & Qt::MouseButton::LeftButton) {
            if (this->m_mouseBallTracking) {
                const auto subItems{ this->scene()->items() };
                if (subItems.size() == 0) return;
                const auto toScene{ mapToScene(event->pos()) };
                static_cast<MyCollisionBall*>(subItems[0])->setPosV(toScene.x(), toScene.y());
            }
        }
        else if (event->button() == Qt::MouseButton::RightButton) {
            if (this->m_mouseAddBall) {
                this->addNewBall();
            }
        }
        event->accept();
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        qDebug() << "[VIEW] mouseMove";
        if (this->m_mouseBallTracking && (event->buttons() & Qt::MouseButton::LeftButton)) {
            const auto subItems{ this->scene()->items() };
            if (subItems.size() == 0) return;
            const auto toScene{ mapToScene(event->pos()) };
            static_cast<MyCollisionBall*>(subItems[0])->setPosV(toScene.x(), toScene.y());
        }
        event->accept();
    }

private slots:
    void updateAll() {
        // not implement yet
    }


public slots:

    void startAll() {
        qDebug() << "test";
        this->scene()->addItem(objBalls[0]);
    }


};

