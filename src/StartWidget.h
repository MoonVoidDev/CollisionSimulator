#pragma once

#include "MyGraphicsView.h"
#include "MyCollisionBall.h"

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

class StartWidget : public QWidget {
    Q_OBJECT

private:
    // Member variables
    MyGraphicsView* view{};
    QGraphicsScene* scene{};

public:
    // Public functions
    StartWidget(QWidget* parent = nullptr);
    ~StartWidget();

protected:
    // Events handlers


private slots:




};
