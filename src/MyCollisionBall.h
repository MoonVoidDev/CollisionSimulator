#include <QGraphicsEllipseItem>

#include <Eigen/Dense>

constexpr double MASS = 1.0;

class MyCollisionBall : public QGraphicsEllipseItem {

public:
    MyCollisionBall(double x, double y, double r);
    ~MyCollisionBall();

    inline void setVelo(const Eigen::Vector2d& velo) {
        this->veloV = velo;
    }
    inline void setPos(const Eigen::Vector2d& pos) {
        this->posV = pos;
    }
    inline void setMass(double mass) {
        this->mass = mass;
    }

    // first invoke this
    inline friend bool checkCollision(const MyCollisionBall& a, const MyCollisionBall& b) {
        const Eigen::Vector2d disV = b.posV - a.posV;
        if (disV.norm() <= b.radius + a.radius) {
            Eigen::Vector2d n{ disV / disV.norm() };
            Eigen::Vector2d an_newVelo{};
            // todo

        }
    }

    // second invoke this
    inline bool checkBoarder();  // todo

    // last invoke this
    inline void updatePosByVelo(); // todo

protected:


private slots:



private:
    double mass;
    double radius;
    Eigen::Vector2d veloV;
    Eigen::Vector2d posV;

};