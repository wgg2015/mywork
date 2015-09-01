#ifndef BALLCOLLIDE_H
#define BALLCOLLIDE_H
#include"ball.h"
#include<cmath>

#ifndef Pi
#define Pi 3.141592653
#endif

class BallPlus:public Ball{
  public:
    BallPlus(Ball& ball):Ball(ball){}
    BallPlus();
    inline GLfloat getV_x() const {return V_x;}
    inline GLfloat getV_y() const {return V_y;}
    inline GLfloat getA_x() const {return Angle_x;}
    inline GLfloat getA_y() const {return Angle_y;}
    inline void resetV_x(GLfloat vx){V_x=vx;}
    inline void resetV_y(GLfloat vy){V_y=vy;}
    inline void resetA_x(GLfloat Ax){Angle_x=Ax;}
    inline void resetA_y(GLfloat Ay){Angle_y=Ay;}
  private:
    GLfloat V_x;
    GLfloat V_y;
    GLfloat Angle_x;
    GLfloat Angle_y;
};
class BallCollide
{
    public:
      BallCollide(Ball &ib,Ball &jb);
      ~BallCollide();
      void update();// update the ball info
    private:
      Ball &iball;
      Ball &jball;
      BallPlus *ibplus;
      BallPlus *jbplus;
private:
      void GetPart();//calculate v_x,v_y,Angle_x,Angle_y;

};

#endif // BALLCOLLIDE_H
