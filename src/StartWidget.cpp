#include "StartWidget.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QFormLayout>

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QPushButton>
#include <QTimer>
#include <QRadioButton>

#include <QSlider>
#include <QLabel>


StartWidget::StartWidget(QWidget* parent)
    : QWidget(parent) {

    // init start
    this->setWindowTitle("Collision Simulator 241106");

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
    this->scene = new QGraphicsScene{ this };
    this->view = new MyGraphicsView{ this };


    this->view->setScene(this->scene);
    rBox->setFixedSize(1340, 1020);
    rLay->addWidget(this->view);


    this->scene->addText("hello, world");
    this->scene->setSceneRect(0, 0, 1280, 960);

    // Control Widgets

    QPushButton* startBtn{ new QPushButton{"Start"} };
    cLay->addRow("", startBtn);

    // Mouse Behaviors
    QGroupBox* mouseBehaviors{ new QGroupBox{"MouseBehaviors", cBox} };
    cLay->addWidget(mouseBehaviors);
    QVBoxLayout* mouseLay{ new QVBoxLayout{mouseBehaviors} };
    mouseBehaviors->setLayout(mouseLay);

    QRadioButton* mouseTrackingBtn{ new QRadioButton{"MouseTracking", cBox} };
    mouseTrackingBtn->setAutoExclusive(false);
    mouseLay->addWidget(mouseTrackingBtn);

    QRadioButton* mouseEliminateBtn{ new QRadioButton{"MouseEliminate", cBox} };
    mouseEliminateBtn->setAutoExclusive(false);
    mouseLay->addWidget(mouseEliminateBtn);

    QRadioButton* mouseAddBallBtn{ new QRadioButton{"MouseAddBall", cBox} };
    mouseEliminateBtn->setAutoExclusive(false);
    mouseLay->addWidget(mouseAddBallBtn);

    // New Ball Property
    QGroupBox* newballProps{ new QGroupBox{"NewBallProperties", cBox} };
    cLay->addWidget(newballProps);
    QGridLayout* nballLay{ new QGridLayout{newballProps} };
    newballProps->setLayout(nballLay);

    QSlider* massSld{ new QSlider{newballProps} };
    QLabel* massDisplay{ new QLabel{newballProps} };
    massSld->setMinimum(1);
    massSld->setMaximum(10);
    massSld->setOrientation(Qt::Orientation::Horizontal);
    nballLay->addWidget(new QLabel{ "Mass", newballProps }, 0, 0);
    nballLay->addWidget(massSld, 0, 1);
    nballLay->addWidget(massDisplay, 0, 2);

    QSlider* veloX{ new QSlider{newballProps} };
    QLabel* veloXDisplay{ new QLabel{newballProps} };
    nballLay->addWidget(new QLabel{ "VeloX", newballProps }, 1, 0);
    nballLay->addWidget(veloX, 1, 1);
    nballLay->addWidget(veloXDisplay, 1, 2);
    veloX->setOrientation(Qt::Orientation::Horizontal);

    QSlider* veloY{ new QSlider{newballProps} };
    QLabel* veloYDisplay{ new QLabel{newballProps} };
    nballLay->addWidget(new QLabel{ "VeloY", newballProps }, 2, 0);
    nballLay->addWidget(veloY, 2, 1);
    nballLay->addWidget(veloYDisplay, 2, 2);
    veloY->setOrientation(Qt::Orientation::Horizontal);

    massDisplay->setText(QString::number(massSld->value()));
    veloXDisplay->setText(QString::number(veloX->value()));
    veloYDisplay->setText(QString::number(veloY->value()));
    connect(massSld, &QSlider::valueChanged, this,
        [massDisplay](int val)-> void {
            massDisplay->setText(QString::number(val));
        }
    );
    connect(veloX, &QSlider::valueChanged, this,
        [veloXDisplay](int val)-> void {
            veloXDisplay->setText(QString::number(val));
        }
    );
    connect(veloY, &QSlider::valueChanged, this,
        [veloYDisplay](int val)-> void {
            veloYDisplay->setText(QString::number(val));
        }
    );

    veloX->setMinimum(0);
    veloX->setMaximum(50);
    veloY->setMinimum(0);
    veloY->setMaximum(50);


    connect(startBtn, &QPushButton::clicked, this->view, &MyGraphicsView::startAll);

}

StartWidget::~StartWidget() {
}

