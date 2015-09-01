#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent){
    pause=false;
    layout();
}
void MainWindow::createball(){
    float r=spb_br->value();
    float v=spb_bv->value();
    float a=2*Pi/360*spb_ba->value();
    float x=spb_bx->value();
    float y=spb_by->value();

    Ball ball(a,v,r,x,y);
    rb->addNewBall(ball);
}

void MainWindow::do_pause()
{
  pause=!pause;
  if(pause){
      btn_Cball->setEnabled(false);
      btn_Pause->setText("run");
  }else{
      btn_Cball->setEnabled(true);
      btn_Pause->setText("pause");
  }
}

void MainWindow::layout(){
    lab_br=new QLabel("R:");
    sld_br=new QSlider(Qt::Horizontal);
    spb_br=new QSpinBox;
    sld_br->setRange(20,40);
    spb_br->setRange(20,40);

    QHBoxLayout *RTLayout=new QHBoxLayout;
    RTLayout->addWidget(lab_br);
    RTLayout->addWidget(spb_br);
    RTLayout->addWidget(sld_br);
    connect(sld_br,SIGNAL(valueChanged(int)),spb_br,SLOT(setValue(int)));
    connect(spb_br,SIGNAL(valueChanged(int)),sld_br,SLOT(setValue(int)));

    lab_bv=new QLabel("V/20ms:");
    sld_bv=new QSlider(Qt::Horizontal);
    spb_bv=new QSpinBox;
    sld_bv->setRange(1,15);
    spb_bv->setRange(1,15);
    QHBoxLayout *RMLayout=new QHBoxLayout;
    RMLayout->addWidget(lab_bv);
    RMLayout->addWidget(spb_bv);
    RMLayout->addWidget(sld_bv);
    connect(sld_bv,SIGNAL(valueChanged(int)),spb_bv,SLOT(setValue(int)));
    connect(spb_bv,SIGNAL(valueChanged(int)),sld_bv,SLOT(setValue(int)));

    lab_bx=new QLabel("Ox:");
    sld_bx=new QSlider(Qt::Horizontal);
    spb_bx=new QSpinBox;
    sld_bx->setRange(-600,600);
    spb_bx->setRange(-600,600);
    QHBoxLayout *RBLayout_x=new QHBoxLayout;
    RBLayout_x->addWidget(lab_bx);
    RBLayout_x->addWidget(spb_bx);
    RBLayout_x->addWidget(sld_bx);
    connect(sld_bx,SIGNAL(valueChanged(int)),spb_bx,SLOT(setValue(int)));
    connect(spb_bx,SIGNAL(valueChanged(int)),sld_bx,SLOT(setValue(int)));


    lab_by=new QLabel("Oy:");
    sld_by=new QSlider(Qt::Horizontal);
    spb_by=new QSpinBox;
    sld_by->setRange(-500,500);
    spb_by->setRange(-500,500);
    QHBoxLayout *RBLayout_y=new QHBoxLayout;
    RBLayout_y->addWidget(lab_by);
    RBLayout_y->addWidget(spb_by);
    RBLayout_y->addWidget(sld_by);
    connect(sld_by,SIGNAL(valueChanged(int)),spb_by,SLOT(setValue(int)));
    connect(spb_by,SIGNAL(valueChanged(int)),sld_by,SLOT(setValue(int)));

    lab_ba=new QLabel("θ°:");
    sld_ba=new QSlider(Qt::Horizontal);
    spb_ba=new QSpinBox;
    sld_ba->setRange(0,360);
    spb_ba->setRange(0,360);

    QHBoxLayout *RBLayout_a=new QHBoxLayout();
    RBLayout_a->addWidget(lab_ba);
    RBLayout_a->addWidget(spb_ba);
    RBLayout_a->addWidget(sld_ba);
    connect(sld_ba,SIGNAL(valueChanged(int)),spb_ba,SLOT(setValue(int)));
    connect(spb_ba,SIGNAL(valueChanged(int)),sld_ba,SLOT(setValue(int)));
    spb_ba->setValue(45);

    btn_Cball=new QPushButton("new ball");
    connect(btn_Cball,SIGNAL(clicked()),this,SLOT(createball()));

    btn_Clear=new QPushButton("clean screen");

    btn_Close=new QPushButton("close");
    connect(btn_Close,SIGNAL(clicked()),this,SLOT(close()));

    btn_Pause= new QPushButton("pause");
    connect(btn_Pause,SIGNAL(clicked()),this,SLOT(do_pause()));



    QVBoxLayout *box_layout=new QVBoxLayout;
    box_layout->addLayout(RTLayout);
    box_layout->addLayout(RMLayout);
    box_layout->addLayout(RBLayout_x);
    box_layout->addLayout(RBLayout_y);
    box_layout->addLayout(RBLayout_a);
    box_layout->addLayout(RBLayout_a);
    box_layout->addWidget(btn_Cball);



    QGroupBox *box_Cb=new QGroupBox("create new ball");
    box_Cb->setLayout(box_layout);
    //box_Cb->setVisible(true);

     QVBoxLayout *RLayout=new QVBoxLayout;
    RLayout->addWidget(box_Cb);

    RLayout->addWidget(btn_Pause);


    RLayout->addWidget(btn_Clear);
    RLayout->addWidget(btn_Close);

    rb=new RBall(0);
    connect(btn_Clear,SIGNAL(clicked()),rb,SLOT(ClearScrean()));
    connect(btn_Pause,SIGNAL(clicked()),rb,SLOT(do_pause()));


    QHBoxLayout *MainLayout=new QHBoxLayout;
    MainLayout->addWidget(rb,100,0);
    MainLayout->addLayout(RLayout);
    setLayout(MainLayout);
    this->resize(640,480);

}
