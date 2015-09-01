#ifndef BALL_H
#define BALL_H
#include"glpoint.h"
class GLRGBcolor{
public:
    GLRGBcolor(int cr=255,int cg=255,int cb=255):r(cr),g(cg),b(cb){}
    //GLRGBcolor():r(255),g(255),b(255){}
    GLRGBcolor& operator= (GLRGBcolor& c){r=c.r;g=c.g;b=c.b;return *this;}
    GLRGBcolor(const GLRGBcolor& c):r(c.r),g(c.g),b(c.b){}
    int r;
    int g;
    int b;
};
class Ball
{
    //Q_OBJECT
public:
    Ball():bAngle(0),bV(10),bR(10),Op(0,0){InitRandcolor();}
    Ball(GLfloat angle=0,GLfloat v=10,GLfloat r=10,GLpoint *p=new GLpoint);
    Ball(GLfloat angle=0,GLfloat v=10,GLfloat r=10,GLfloat x=0,GLfloat y=0);
    Ball(const Ball&);
    inline GLfloat getA() const {return bAngle;}
    inline GLfloat getV() const {return bV;}
    inline GLfloat getR() const {return bR;}
    inline GLpoint getOp() const {return Op;}
    inline GLpoint getPrep() const  {return Prep;}
    inline GLRGBcolor& getColor() {return color;}


    inline void resetAngle(GLfloat a){bAngle=a;}
    inline void resetV(GLfloat v){bV=v;}
    inline void resetR(GLfloat r){bR=r;}
    inline void resetOp(GLpoint p){Op=p;}
    inline void resetPrep(GLpoint pp){Prep=pp;}
    Ball& move(GLfloat times=0.2);
protected:
    GLfloat bAngle;
    GLfloat bV;
    GLfloat bR;
    GLpoint Op;
    GLpoint Prep;
    GLRGBcolor color;
private:
    void InitRandcolor();
};

#endif // BALL_H
