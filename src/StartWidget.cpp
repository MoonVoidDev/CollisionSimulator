#include "StartWidget.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>

#include <QCheckBox>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QRadioButton>
#include <QTimer>

#include <QLabel>
#include <QSlider>
#include <QSpinBox>

StartWidget::StartWidget(QWidget *parent) : QWidget(parent) {

  // Start init
  this->setWindowTitle("Collision Simulator 241107");

  QHBoxLayout *mainLay{new QHBoxLayout{this}};
  QGroupBox *renderBox{new QGroupBox{"Render", this}};
  QHBoxLayout *renderLay{new QHBoxLayout{renderBox}};
  QGroupBox *controlBox{new QGroupBox{"Control", this}};
  QFormLayout *controlLay{new QFormLayout{controlBox}};
  this->setLayout(mainLay);
  renderBox->setLayout(renderLay);
  controlBox->setLayout(controlLay);
  mainLay->addWidget(renderBox);
  mainLay->addWidget(controlBox);
  renderBox->setFixedSize(1340, 1020);

  // Main Widgets
  this->scene = new QGraphicsScene{this};
  this->view = new MyGraphicsView{this};
  this->view->setScene(this->scene); // Important
  renderLay->addWidget(this->view);
  this->scene->setSceneRect(0, 0, 1280, 960);

  // Control Widgets
  QPushButton *startBtn{new QPushButton{"Start"}};
  controlLay->addRow("", startBtn);

  // Mouse Behaviors
  QGroupBox *mouseBehaviors{new QGroupBox{"MouseBehaviors", controlBox}};
  QVBoxLayout *mouseLay{new QVBoxLayout{mouseBehaviors}};
  QCheckBox *mouseBallTrackingChbox{new QCheckBox{"MouseTracking", controlBox}};
  QCheckBox *mouseEliminateChbox{new QCheckBox{"MouseEliminate", controlBox}};
  QCheckBox *mouseAddBallChbox{new QCheckBox{"MouseAddBall", controlBox}};
  controlLay->addWidget(mouseBehaviors);
  mouseBehaviors->setLayout(mouseLay);
  mouseLay->addWidget(mouseBallTrackingChbox);
  mouseLay->addWidget(mouseEliminateChbox);
  mouseLay->addWidget(mouseAddBallChbox);

  // New Ball Property
  QGroupBox *newballProps{new QGroupBox{"NewBallProperties", controlBox}};
  QGridLayout *nballLay{new QGridLayout{newballProps}};
  QSlider *massSld{new QSlider{newballProps}};
  QLabel *massDisplay{new QLabel{newballProps}};
  QSlider *veloX{new QSlider{newballProps}};
  QLabel *veloXDisplay{new QLabel{newballProps}};
  QSlider *veloY{new QSlider{newballProps}};
  QLabel *veloYDisplay{new QLabel{newballProps}};
  QSlider *radiusSld{new QSlider{newballProps}};
  QLabel *radiusDisplay{new QLabel{newballProps}};
  controlLay->addWidget(newballProps);
  newballProps->setLayout(nballLay);

  massSld->setOrientation(Qt::Orientation::Horizontal);
  nballLay->addWidget(new QLabel{"Mass", newballProps}, 0, 0);
  nballLay->addWidget(massSld, 0, 1);
  nballLay->addWidget(massDisplay, 0, 2);

  nballLay->addWidget(new QLabel{"VeloX", newballProps}, 1, 0);
  nballLay->addWidget(veloX, 1, 1);
  nballLay->addWidget(veloXDisplay, 1, 2);
  veloX->setOrientation(Qt::Orientation::Horizontal);

  nballLay->addWidget(new QLabel{"VeloY", newballProps}, 2, 0);
  nballLay->addWidget(veloY, 2, 1);
  nballLay->addWidget(veloYDisplay, 2, 2);
  veloY->setOrientation(Qt::Orientation::Horizontal);

  nballLay->addWidget(new QLabel{"Radius", newballProps}, 3, 0);
  nballLay->addWidget(radiusSld, 3, 1);
  nballLay->addWidget(radiusDisplay, 3, 2);
  radiusSld->setOrientation(Qt::Orientation::Horizontal);

  // Misc
  QGroupBox *miscBox{new QGroupBox{controlBox}};
  QVBoxLayout *miscLay{new QVBoxLayout{miscBox}};
  QSpinBox *batchSpin{new QSpinBox{miscBox}};
  QPushButton *batchAddBtn{new QPushButton{"BatchAdd", miscBox}};
  QPushButton *batchDeleteBtn{new QPushButton{"BatchDelete", miscBox}};
  controlLay->addWidget(miscBox);
  miscBox->setLayout(miscLay);
  miscBox->setTitle("Misc");
  miscLay->addWidget(batchSpin);
  miscLay->addWidget(batchAddBtn);
  miscLay->addWidget(batchDeleteBtn);
  batchSpin->setSingleStep(20);

  // Algo switcher
  QGroupBox *algoSwicthBox{new QGroupBox{controlBox}};
  QHBoxLayout *algoSwitchLay{new QHBoxLayout{algoSwicthBox}};
  QRadioButton *brutalRadioBtn{new QRadioButton{"Brutal", algoSwicthBox}};
  QRadioButton *quadtreeRadioBtn{new QRadioButton{"Quadtree", algoSwicthBox}};
  controlLay->addWidget(algoSwicthBox);
  algoSwicthBox->setLayout(algoSwitchLay);
  algoSwicthBox->setTitle("AlgoSwitch");
  algoSwitchLay->addWidget(brutalRadioBtn);
  algoSwitchLay->addWidget(quadtreeRadioBtn);

  // Final init
  massSld->setMinimum(1);
  massSld->setMaximum(25);
  this->view->setNewBallMass(1);
  veloX->setMinimum(0);
  veloX->setMaximum(3000);
  this->view->setNewBallVeloX(0);
  veloY->setMinimum(0);
  veloY->setMaximum(3000);
  this->view->setNewBallVeloY(0);
  radiusSld->setMinimum(10);
  radiusSld->setMaximum(200);
  this->view->setNewBallRadius(10);
  massDisplay->setText(QString::number(massSld->value()));
  veloXDisplay->setText(QString::number(veloX->value()));
  veloYDisplay->setText(QString::number(veloY->value()));
  radiusDisplay->setText(QString::number(radiusSld->value()));

  // Connects
  connect(startBtn, &QPushButton::clicked, this->view,
          &MyGraphicsView::startAll);
  connect(massSld, &QSlider::valueChanged, this, [=](int val) -> void {
    massDisplay->setText(QString::number(val));
    this->view->setNewBallMass(val);
  });
  connect(veloX, &QSlider::valueChanged, this, [=](int val) -> void {
    veloXDisplay->setText(QString::number(val));
    this->view->setNewBallVeloX(val);
  });
  connect(veloY, &QSlider::valueChanged, this, [=](int val) -> void {
    veloYDisplay->setText(QString::number(val));
    this->view->setNewBallVeloY(val);
  });
  connect(radiusSld, &QSlider::valueChanged, this, [=](int val) -> void {
    radiusDisplay->setText(QString::number(val));
    this->view->setNewBallRadius(val);
  });

  connect(mouseBallTrackingChbox, &QCheckBox::toggled, this,
          [this](bool checked) -> void {
            this->view->setMouseBallTracking(checked);
          });
  connect(
      mouseEliminateChbox, &QCheckBox::toggled, this,
      [this](bool checked) -> void { this->view->setMouseEliminate(checked); });
  connect(
      mouseAddBallChbox, &QCheckBox::toggled, this,
      [this](bool checked) -> void { this->view->setMouseAddBall(checked); });

  connect(batchAddBtn, &QPushButton::clicked, this,
          [this, batchSpin]() -> void {
            this->view->batchAdd(batchSpin->value());
          });

  connect(batchDeleteBtn, &QPushButton::clicked, this,
          [this, batchSpin]() -> void {
            this->view->batchDelete(batchSpin->value());
          });

  connect(brutalRadioBtn, &QRadioButton::toggled, this,
          [this]() -> void { this->view->useQuadtreeAlgo(false); });

  connect(quadtreeRadioBtn, &QRadioButton::toggled, this,
          [this]() -> void { this->view->useQuadtreeAlgo(true); });

  // default configs
  mouseBallTrackingChbox->setChecked(true);
  mouseAddBallChbox->setChecked(true);

  quadtreeRadioBtn->setChecked(true);

  // other
  this->view->initMouseBall();
}

StartWidget::~StartWidget() {}
