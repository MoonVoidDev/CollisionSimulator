#pragma once

#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <Eigen/Dense>

constexpr double MASS = 1.0;

class MyCollisionBall : public QGraphicsEllipseItem {

private:
    double mass{ 1 };
    double radius{};
    Eigen::Vector2d veloV{};
    Eigen::Vector2d posV{};
    bool isMouse{};


public:
    MyCollisionBall(double x, double y, double r) :
        QGraphicsEllipseItem{},
        mass{ MASS },
        radius{ r },
        veloV{ 0,0 },
        posV{ x, y },
        isMouse{ false } {
        this->setRect(-r, -r, 2 * r, 2 * r);
        this->setPos(x, y);

    };
    ~MyCollisionBall() = default;

    void setVeloV(double x, double y) {
        this->veloV = { x, y };
    }
    void setPosV(double x, double y) {
        this->posV = { x, y };
        this->setPos(x, y);
    }
    void setMass(double mass) {
        this->mass = mass;
    }
    void setIsMouse(bool status) {
        this->isMouse = status;
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        QPen outline{};
        QBrush inside{};
        outline.setWidth(5);
        inside.setStyle(Qt::BrushStyle::SolidPattern);
        if (isMouse) {
            // my mouse
            outline.setColor(QColor::fromRgb(234, 64, 64));
            inside.setColor(QColor::fromRgb(238, 0, 0));
        }
        else {
            outline.setColor(QColor::fromRgb(198, 232, 249));
            inside.setColor(QColor::fromRgb(102, 204, 255));
        }
        painter->setPen(outline);
        painter->setBrush(inside);
        painter->drawEllipse(this->rect());
    }

    // first invoke this
    inline friend bool processCollision(MyCollisionBall& a, MyCollisionBall& b) {
        const Eigen::Vector2d disV = a.posV - b.posV;
        if (disV.norm() <= a.radius + b.radius) {
            Eigen::Vector2d n{ disV / disV.norm() };
            double relV{ (a.veloV - b.veloV).dot(n) };
            if (relV >= 0) return false;
            double massSum{ a.mass + b.mass };
            Eigen::Vector2d aNewVeloV{
                a.veloV - ((2 * b.mass) / massSum * relV * n)
            };
            Eigen::Vector2d bNewVeloV{
                b.veloV + ((2 * a.mass) / massSum * relV * n)
            };

            a.veloV = aNewVeloV;
            b.veloV = bNewVeloV;
            return true;
        }
        return false;
    }

    // second invoke this
    inline bool processBorderCollision(int limitX, int limitY) {
        if (this->posV[0] <= radius || this->posV[0] >= limitX - radius) {
            // left and right 
            Eigen::Vector2d axis{ 1, 0 };
            this->veloV = this->veloV - 2 * (this->veloV.dot(axis)) * axis;
            return true;
        }
        else if (this->posV[1] <= radius || this->posV[1] >= limitY - radius) {
            // up and down
            Eigen::Vector2d axis{ 0, 1 };
            this->veloV = this->veloV - 2 * (this->veloV.dot(axis)) * axis;
            return true;
        }
        return false;
    }

    // last invoke this
    inline void updatePosByVelo(int msec) {
        qDebug() << "updatePosByVelo";
        // this->posV += msec / 1000.0 * this->veloV;
        // this->setPos(this->posV[0], this->posV[1]);
    }

protected:

    // Event handlers
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        qDebug() << "Item mouse press";
        event->ignore();
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        qDebug() << "Item mouse move";
        event->ignore();
    }


private slots:



};