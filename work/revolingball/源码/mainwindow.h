#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include"rball.h"
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include<QSlider>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSpinBox>
#include<QGroupBox>
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    //~MainWindow(){delete lab_br;}
private:
    QLabel *lab_br; QSpinBox *spb_br; QSlider  *sld_br;// radius
    QLabel *lab_bx; QSpinBox *spb_bx; QSlider  *sld_bx;// x coordinate
    QLabel *lab_by; QSpinBox *spb_by; QSlider  *sld_by;// y coordinate
    QLabel *lab_bv; QSpinBox *spb_bv; QSlider  *sld_bv;//  speed
    QLabel *lab_ba; QSpinBox *spb_ba; QSlider  *sld_ba;// angle
    QPushButton *btn_Cball;//create new ball


    QPushButton *btn_Pause;
    bool pause;
    QPushButton *btn_Clear;//clean ball

    QPushButton *btn_Close;//close application

    RBall *rb;//qgl screen
private:
    void layout();
signals:
private slots:
void createball();
void do_pause();
};

#endif // MAINWINDOW_H
