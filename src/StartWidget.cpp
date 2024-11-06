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
#include <QCheckBox>

#include <QSlider>
#include <QLabel>


StartWidget::StartWidget(QWidget* parent)
    : QWidget(parent) {

    // Start init
    this->setWindowTitle("Collision Simulator 241106");

    QHBoxLayout* mainLay{ new QHBoxLayout{this} };
    QGroupBox* renderBox{ new QGroupBox{"Render", this} };
    QHBoxLayout* renderLay{ new QHBoxLayout{renderBox} };
    QGroupBox* controlBox{ new QGroupBox{"Control", this} };
    QFormLayout* controlLay{ new QFormLayout{controlBox} };
    this->setLayout(mainLay);
    renderBox->setLayout(renderLay);
    controlBox->setLayout(controlLay);
    mainLay->addWidget(renderBox);
    mainLay->addWidget(controlBox);
    renderBox->setFixedSize(1340, 1020);


    // Main Widgets
    this->scene = new QGraphicsScene{ this };
    this->view = new MyGraphicsView{ this };
    this->view->setScene(this->scene);
    renderLay->addWidget(this->view);
    this->scene->setSceneRect(0, 0, 1280, 960);

    // Control Widgets
    QPushButton* startBtn{ new QPushButton{"Start"} };
    controlLay->addRow("", startBtn);

    // Mouse Behaviors
    QGroupBox* mouseBehaviors{ new QGroupBox{"MouseBehaviors", controlBox} };
    QVBoxLayout* mouseLay{ new QVBoxLayout{mouseBehaviors} };
    QCheckBox* mouseBallTrackingChbox{ new QCheckBox{"MouseTracking", controlBox} };
    QCheckBox* mouseEliminateChbox{ new QCheckBox{"MouseEliminate", controlBox} };
    QCheckBox* mouseAddBallChbox{ new QCheckBox{"MouseAddBall", controlBox} };
    controlLay->addWidget(mouseBehaviors);
    mouseBehaviors->setLayout(mouseLay);
    mouseLay->addWidget(mouseBallTrackingChbox);
    mouseLay->addWidget(mouseEliminateChbox);
    mouseLay->addWidget(mouseAddBallChbox);

    // New Ball Property
    QGroupBox* newballProps{ new QGroupBox{"NewBallProperties", controlBox} };
    QGridLayout* nballLay{ new QGridLayout{newballProps} };
    QSlider* massSld{ new QSlider{newballProps} };
    QLabel* massDisplay{ new QLabel{newballProps} };
    QSlider* veloX{ new QSlider{newballProps} };
    QLabel* veloXDisplay{ new QLabel{newballProps} };
    QSlider* veloY{ new QSlider{newballProps} };
    QLabel* veloYDisplay{ new QLabel{newballProps} };
    controlLay->addWidget(newballProps);
    newballProps->setLayout(nballLay);

    massSld->setMinimum(1);
    massSld->setMaximum(10);
    massSld->setOrientation(Qt::Orientation::Horizontal);
    nballLay->addWidget(new QLabel{ "Mass", newballProps }, 0, 0);
    nballLay->addWidget(massSld, 0, 1);
    nballLay->addWidget(massDisplay, 0, 2);

    nballLay->addWidget(new QLabel{ "VeloX", newballProps }, 1, 0);
    nballLay->addWidget(veloX, 1, 1);
    nballLay->addWidget(veloXDisplay, 1, 2);
    veloX->setOrientation(Qt::Orientation::Horizontal);

    nballLay->addWidget(new QLabel{ "VeloY", newballProps }, 2, 0);
    nballLay->addWidget(veloY, 2, 1);
    nballLay->addWidget(veloYDisplay, 2, 2);
    veloY->setOrientation(Qt::Orientation::Horizontal);

    // Final init
    massDisplay->setText(QString::number(massSld->value()));
    veloXDisplay->setText(QString::number(veloX->value()));
    veloYDisplay->setText(QString::number(veloY->value()));
    veloX->setMinimum(0);
    veloX->setMaximum(2000);
    veloY->setMinimum(0);
    veloY->setMaximum(2000);

    // Connects
    connect(startBtn, &QPushButton::clicked, this->view, &MyGraphicsView::startAll);
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

    connect(mouseBallTrackingChbox, &QCheckBox::toggled, this,
        [this](bool checked)->void {
            this->view->setMouseBallTracking(checked);
        }
    );
    connect(mouseEliminateChbox, &QCheckBox::toggled, this,
        [this](bool checked)->void {
            this->view->setMouseEliminate(checked);
        }
    );
    connect(mouseAddBallChbox, &QCheckBox::toggled, this,
        [this](bool checked)->void {
            this->view->setMouseAddBall(checked);
        }
    );

}

StartWidget::~StartWidget() {
}

