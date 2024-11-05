#include "StartWidget.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QFormLayout>

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QPushButton>

StartWidget::StartWidget(QWidget* parent)
    : QWidget(parent) {

    // init start
    QGroupBox* rBox{ new QGroupBox{"Render", this} };
    QHBoxLayout* rLay{ new QHBoxLayout{rBox} };
    rBox->setLayout(rLay);

    QGroupBox* cBox{ new QGroupBox{"Control", this} };
    QFormLayout* cLay{ new QFormLayout{cBox} };
    cBox->setLayout(cLay);

    QHBoxLayout* mainLay{ new QHBoxLayout{this} };
    this->setLayout(mainLay);
    mainLay->addWidget(rBox);
    mainLay->addWidget(cBox);

    // render
    this->scene = new MyGraphicsScene{ this };
    this->view = new MyGraphicsView{ this };


    this->view->setScene(this->scene);
    this->view->setFixedSize(1280, 960);
    rBox->setFixedSize(1320, 1000);
    rLay->addWidget(this->view);


    this->scene->addText("hello, world");

    QPushButton* a{ new QPushButton{"testbutton"} };
    cLay->addRow("nihao", a);

}

StartWidget::~StartWidget() {
}

