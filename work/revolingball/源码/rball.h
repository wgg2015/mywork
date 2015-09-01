#ifndef RBALL_H
#define RBALL_H
#ifndef Pi
#define Pi 3.141592653
#endif
#ifndef Np_Circle
#define Np_Circle 80
#endif
#include <QGLWidget>
#include<GL/glu.h>
#include"ball.h"
#include"glpoint.h"
#include<cmath>
#include"ballcollide.h"
#include<memory>
#include<time.h>
#include<vector>
#include<QKeyEvent>
#include<iterator>

class RBall : public QGLWidget
{
    Q_OBJECT
public:
    explicit RBall(QWidget *parent = 0);
    RBall& addNewBall(Ball &);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );

    void timerEvent(QTimerEvent *);

private:
    std::vector<Ball> VB;
    bool fullScreen;

    GLfloat SWidth;
    GLfloat SHeight;
    bool pause;
    enum typepoint{
        CurPoint,
        PrePoint
    };
private:
    RBall& DrawBall(Ball&);

    int  BallDistence(Ball&, Ball&, typepoint tp);
    //int  WallDistence(Ball &ball, GLfloat &wall,typepoint tp);
    GLfloat DealAngleToAcute(GLfloat);
    void Collide();//check collide
    int  ColWall();//collide wall
    int  ColBall();//collide ball
signals:

public slots:
inline void ClearScrean(){
    VB.clear();
}
void do_pause(){pause=!pause;}
};


#endif // RBALL_H
