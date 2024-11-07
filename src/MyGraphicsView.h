#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDebug>
#include <QPushButton>
#include <cmath>
#include <random>

#include "MyCollisionBall.h"

#include <vector>




constexpr static int MAX_MANAGE = 10;
constexpr static int MAX_DEPTH = 6;

// constexpr static int MAX_NODECNT = 1 * (1 - std::pow(4, MAX_DEPTH)) / (1 - 4);
// constexpr static int MAX_LEAFCNT = std::pow(4, MAX_DEPTH - 1);
// constexpr static int MAX_NODEPOOL = MAX_NODECNT;
// constexpr static int MAX_LEAFPOOL = MAX_LEAFCNT;

struct QuadtreeNode {
    double x{}, y{}, w{}, h{};
    QuadtreeNode* s[4]{};
    int depth{ 1 };
    std::vector<MyCollisionBall*> manage{};
    QuadtreeNode(double _x, double _y, double _w, double _h, int _depth) :
        x{ _x },
        y{ _y },
        w{ _w },
        h{ _h },
        depth{ _depth } {
    }
    ~QuadtreeNode() = default;

    void divide() {
        double neww{ w / 2 }, newh{ h / 2 };
        s[0] = new QuadtreeNode{
            x + neww, y, neww, newh, depth + 1
        };
        s[1] = new QuadtreeNode{
            x, y, neww, newh, depth + 1
        };
        s[2] = new QuadtreeNode{
            x, y + newh, neww, newh, depth + 1
        };
        s[3] = new QuadtreeNode{
            x + neww, y + newh, neww, newh, depth + 1
        };
    }

    void clear() {
        manage.clear();
        for (int i = 0; i < 4; i++) {
            if (s[i]) {
                s[i]->clear();
                delete s[i];
                s[i] = nullptr;
            }
        }
    }

    std::vector<int> getManagerIndex(MyCollisionBall* obj) {
        double midx{ x + w / 2 }, midy{ y + h / 2 };
        double objx{ obj->x() }, objy{ obj->y() }, objr{ obj->getRadius() };
        std::vector<int> index{};
        bool up{ objy + objr > midy },
            down{ objy - objr < midy },
            left{ objx - objr < midx },
            right{ objx + objr > midx };
        if (up) {
            if (left) index.emplace_back(0);
            if (right) index.emplace_back(1);
        }
        if (down) {
            if (left) index.emplace_back(2);
            if (right) index.emplace_back(3);
        }
        return index;
    }

    void insert(MyCollisionBall* obj) {
        if (s[0]) {
            std::vector<int> index{ getManagerIndex(obj) };
            for (int i : index) {
                s[i]->insert(obj);
            }
            return;
        }
        manage.emplace_back(obj);
        if (manage.size() > MAX_MANAGE && depth < MAX_DEPTH) {
            divide();
            for (auto mngObj : manage) {
                std::vector<int> index{ getManagerIndex(mngObj) };
                for (int i : index) {
                    s[i]->insert(mngObj);
                }
            }
            manage.clear();
        }
    }

    std::vector<MyCollisionBall*> getObjectsNear(MyCollisionBall* obj) {
        std::vector<int> index{ getManagerIndex(obj) };
        if (s[0]) {
            std::vector<MyCollisionBall*> ret{};
            std::vector<MyCollisionBall*> get{};
            // return s[index]->getObjectsNear(obj);
            for (int i : index) {
                get = s[i]->getObjectsNear(obj);
                ret.insert(ret.end(), get.begin(), get.end());
            }
            return ret;
        }
        return manage;
    }
};


class MyGraphicsView : public QGraphicsView {

    Q_OBJECT

private:

    // Scene 
    int maxX{};
    int maxY{};

    // Info
    unsigned long long collisionTimes{};
    unsigned long long comparedTimes{};  // todo

    // Render
    QTimer* clock{};

    // Misc
    bool m_mouseBallTracking{};
    bool m_mouseEliminate{};
    bool m_mouseAddBall{};

    double newBallVeloX{};
    double newBallVeloY{};
    double newBallMass{};
    double newBallRadius{};

    // Algorithms

    static constexpr int MAX_BALLPOOL = 100000;

    bool useQTree{ true };

    MyCollisionBall* ballPool{};
    int ballPoolTop{};
    MyCollisionBall** ballFreeStack{};
    int ballFreeTop{};



private:



protected:

    MyCollisionBall* addNewBall(double x, double y) {
        MyCollisionBall* addr{};
        if (ballFreeTop && ballFreeTop < MAX_BALLPOOL) {
            addr = ballFreeStack[--ballFreeTop];
            addr->~MyCollisionBall();
        }
        else if (ballPoolTop < MAX_BALLPOOL) {
            addr = &(ballPool[ballPoolTop++]);
        }
        new(addr) MyCollisionBall{ x, y, newBallRadius };
        addr->setVeloV(newBallVeloX, newBallVeloY);
        addr->setMass(newBallMass);
        this->scene()->addItem(addr);
        return addr;
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
        ballPool = static_cast<MyCollisionBall*>(operator new[](sizeof(MyCollisionBall)* MAX_BALLPOOL, std::nothrow));
        ballFreeStack = static_cast<MyCollisionBall**>(operator new[](sizeof(MyCollisionBall*)* MAX_BALLPOOL, std::nothrow));

        // Algorithms
        // nope

        // Connects
        connect(this->clock, &QTimer::timeout, this, &MyGraphicsView::updateAll);

    }
    ~MyGraphicsView() {
        for (int i = 0; i < ballPoolTop; i++) {
            ballPool[i].~MyCollisionBall();
        }
        operator delete[](ballPool, sizeof(MyCollisionBall)* MAX_BALLPOOL);
        operator delete[](ballFreeStack, sizeof(MyCollisionBall*)* MAX_BALLPOOL);
    }

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

    void initMouseBall() {
        auto mouseBall = addNewBall(200, 200);
        mouseBall->setIsMouse(true);
        mouseBall->setRadius(16);
    }

    void batchAdd(long long count) {
        std::random_device rd{};
        std::mt19937 gen{ rd() };
        std::uniform_int_distribution<> distribX{ 20, maxX - 20 };
        std::uniform_int_distribution<> distribY{ 20, maxY - 20 };
        for (int i = 0; i < count; i++) {
            addNewBall(distribX(gen), distribY(gen));
        }
    }

    void batchDelete(long long count) {
        auto items{ this->scene()->items() };
        MyCollisionBall* a{};
        for (int i = 0; i < std::min(count, items.size()); i++) {
            a = static_cast<MyCollisionBall*>(items[i]);
            if (a && a->getIsMouse()) break;
            // place back to pool
            ballFreeStack[ballFreeTop++] = a;
            this->scene()->removeItem(a);
        }
    }

    void useQuadtreeAlgo(bool useQTree) {
        // clear times
        this->collisionTimes = 0;
        this->comparedTimes = 0;
        this->useQTree = useQTree;
    }


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
        painter.drawText(20, 20, "Collision Times " + QString::number(this->collisionTimes));
        painter.drawText(20, 40, "Compare Times " + QString::number(this->comparedTimes));
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
            auto subItems{ this->scene()->items() };
            if (subItems.size() == 0) return;
            auto mouseBall{ static_cast<MyCollisionBall*>(subItems.back()) };
            auto mousePos{ event->position() };
            auto toScene{
                mapToScene(
                    std::min(std::max(mousePos.x(), mouseBall->getRadius()), maxX - mouseBall->getRadius()),
                std::min(std::max(mousePos.y(), mouseBall->getRadius()), maxY - mouseBall->getRadius())
                )
            };

            // Eigen::Vector2d lastPos{ mouseBall->getPosV() };
            // Eigen::Vector2d newPos{ toScene.x(), toScene.y() };
            // Eigen::Vector2d newVelo{ (newPos - lastPos) / (this->clock->interval() / 1000.0) };

            // mouseBall->setVeloV(newVelo[0], newVelo[1]);
            mouseBall->setPosV(toScene.x(), toScene.y());
            mouseBall->setVeloV(0, 0);
        }
    }

    void brutalCollisionCheck() {

        auto items{ this->scene()->items() };

        for (auto i : items) {
            auto a{ static_cast<MyCollisionBall*>(i) };
            for (auto j : items) {
                if (i == j) continue;
                auto b{ static_cast<MyCollisionBall*>(j) };
                bool ifCollide{ processCollision(*a, *b) };
                collisionTimes += ifCollide;
                comparedTimes++;
            }
            bool ifBorderCollide{ a->processBorderCollision(maxX, maxY) };
            collisionTimes += ifBorderCollide;
            comparedTimes++;
        }

        for (auto i : items) {
            (static_cast<MyCollisionBall*>(i))->updatePosByVelo(this->clock->interval());
        }

    }

    void quadTreeCollisionCheck() {

        QuadtreeNode qTree{ 0,0, (double)maxX, (double)maxY, 1 };
        auto items{ this->scene()->items() };
        for (auto i : items) {
            auto cur{ static_cast<MyCollisionBall*>(i) };
            qTree.insert(cur);
        }

        std::vector<MyCollisionBall*> checkDelete{};

        for (auto i : items) {
            auto cur{ static_cast<MyCollisionBall*>(i) };
            std::vector<MyCollisionBall*> possible{ qTree.getObjectsNear(cur) };
            if (cur->getIsMouse() && this->m_mouseEliminate) {
                for (auto other : possible) {
                    if (cur == other) continue;
                    bool ifCollide{ existCollision(*cur, *other) };
                    collisionTimes += ifCollide;
                    comparedTimes++;
                    if (ifCollide) other->setDeleteLater(true);
                }
                checkDelete = std::move(possible);
            }
            else {
                for (auto other : possible) {
                    if (cur == other) continue;
                    bool ifCollide{ processCollision(*cur, *other) };
                    // if ((cur->getIsMouse() || other->getIsMouse()) && ifCollide) {
                    //     qDebug() << "BREAKPOINT";
                    // }
                    collisionTimes += ifCollide;
                    comparedTimes++;
                }
            }
            bool ifBorderCollide{ cur->processBorderCollision(maxX, maxY) };
            collisionTimes += ifBorderCollide;
            cur->updatePosByVelo(this->clock->interval());
        }


        if (this->m_mouseEliminate) {
            for (auto i : checkDelete) {
                if (i->getDeleteLater()) {
                    ballFreeStack[ballFreeTop++] = i;
                    this->scene()->removeItem(i);
                }
            }
        }

        qTree.clear();

    }

private slots:
    void updateAll() {

        if (useQTree) {
            quadTreeCollisionCheck();
        }
        else {
            brutalCollisionCheck();
        }

    }


public slots:

    void startAll() {
        qDebug() << "[VIEW] Simulation started";
        this->clock->start();
        static_cast<QPushButton*>(sender())->setEnabled(false);
    }



};

