#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDebug>
#include <QPushButton>
#include <cmath>

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
    bool m_mouseBallTracking{};
    bool m_mouseEliminate{};
    bool m_mouseAddBall{};

    int newBallVeloX{};
    int newBallVeloY{};
    int newBallMass{};
    int newBallRadius{};

    // Algorithms

    std::vector<MyCollisionBall*> objBalls{};
    int objCnt{};

    struct QuadtreeNode {
        int ls, rs;

    };



protected:

    void addNewBall(double x, double y) {
        MyCollisionBall* add{ new MyCollisionBall(x, y, newBallRadius) };
        objBalls.emplace_back(add);
        objCnt++;
        add->setVeloV(newBallVeloX, newBallVeloY);
        add->setMass(newBallMass);
        this->scene()->addItem(add);
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
        objBalls.reserve(1024);

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

    void setNewBallVeloX(double value) { this->newBallVeloX = value; }
    void setNewBallVeloY(double value) { this->newBallVeloY = value; }
    void setNewBallMass(double value) { this->newBallMass = value; }
    void setNewBallRadius(double value) { this->newBallRadius = value; }

protected:
    // Event handlers
    void paintEvent(QPaintEvent* event) override {
        QGraphicsView::paintEvent(event);
        QPainter painter{ this->viewport() };
        QPen outline{};
        painter.setRenderHint(QPainter::RenderHint::Antialiasing);
        outline.setColor(Qt::white);
        outline.setWidth(5);
        painter.setPen(outline);
        painter.drawRect(this->viewport()->rect());
        painter.setFont(QFont{ "Consolas" });
        painter.drawText(20, 20, QString::number(this->collisionTimes));
    }

    void mousePressEvent(QMouseEvent* event) override {
        qDebug() << "[VIEW] mousePress";
        if (event->buttons() & Qt::MouseButton::RightButton) {
            if (this->m_mouseAddBall) {
                const auto mousePos{ event->position() };
                const auto toScene{
                    mapToScene(
                        std::min(std::max(mousePos.x(), (double)newBallRadius), maxX - (double)newBallRadius),
                    std::min(std::max(mousePos.y(), (double)newBallRadius), maxY - (double)newBallRadius)
                    )
                };
                this->addNewBall(toScene.x(), toScene.y());
            }
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        qDebug() << "[VIEW] mouseMove";
        if (this->m_mouseBallTracking && (event->buttons() & Qt::MouseButton::LeftButton)) {
            const auto subItems{ this->scene()->items() };
            if (subItems.size() == 0) return;
            const auto mouseBall{ static_cast<MyCollisionBall*>(subItems.back()) };
            const auto mousePos{ event->position() };
            const auto toScene{
                mapToScene(
                    std::min(std::max(mousePos.x(), mouseBall->getRadius()), maxX - mouseBall->getRadius()),
                std::min(std::max(mousePos.y(), mouseBall->getRadius()), maxY - mouseBall->getRadius())
                )
            };
            mouseBall->setPosV(toScene.x(), toScene.y());
        }
    }

private slots:
    void updateAll() {
        // not implement yet
        auto items{ this->scene()->items() };
        for (int i = 0; i < items.size(); i++) {
            auto cur{ static_cast<MyCollisionBall*>(items[i]) };
            if (cur->getIsMouse()) continue;
            cur->processBorderCollision(maxX, maxY);
            cur->updatePosByVelo(this->clock->interval());
        }
    }


public slots:

    void startAll() {
        qDebug() << "[VIEW] Simulation started";
        // Init mouse ball
        MyCollisionBall* a{ new MyCollisionBall{200,200,16} };
        a->setIsMouse(true);
        a->setVeloV(2000, 2000);
        objBalls.emplace_back(a);
        objCnt++;
        this->scene()->addItem(objBalls[0]);
        this->clock->start();
        static_cast<QPushButton*>(sender())->setEnabled(false);
    }

};

