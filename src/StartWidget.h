#pragma once

#include "MyGraphicsScene.h"
#include "MyGraphicsView.h"

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

class StartWidget : public QWidget {
    Q_OBJECT

public:
    // Public functions
    StartWidget(QWidget* parent = nullptr);
    ~StartWidget();

protected:
    // Events handlers


private slots:
    // Slots

private:
    // Member variables
    MyGraphicsView* view;
    MyGraphicsScene* scene;

};
