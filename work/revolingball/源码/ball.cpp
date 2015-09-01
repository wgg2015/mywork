#include "ball.h"
#include "math.h"
#include<time.h>
/**
 * @brief Ball::move
 * @param times control v
 * @return
 */
Ball& Ball::move(GLfloat times)
{
   Prep=Op;
   Op.x+=cos(bAngle)*bV*times;
   Op.y+=sin(bAngle)*bV*times;
   return *this;
}

void Ball::InitRandcolor(){
    srand(time(0));
    //int rgb_r,rgb_g,rgb_b;
    color.r=rand()%256;

    color.g=rand()%256;

    color.b=rand()%256;
}

Ball::Ball(GLfloat angle,GLfloat v,GLfloat r,GLfloat x,GLfloat y):
    bAngle(angle),bV(v),bR(r){
    Op.x=x;Op.y=y;Prep=Op;InitRandcolor();
}

Ball::Ball(GLfloat angle,GLfloat v,GLfloat r,GLpoint *p):
    bAngle(angle),bV(v),bR(r),Op(*p){
    Prep=Op;InitRandcolor();
}

Ball::Ball(const Ball& ball):
    bAngle(ball.getA()),bV(ball.getV()),bR(ball.getR()),Op(ball.getOp()),Prep(ball.getPrep()),color(ball.color){
}
