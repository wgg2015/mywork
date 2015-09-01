#ifndef GLPOINT_H
#define GLPOINT_H
#include<GL/glu.h>
class GLpoint
{
   //Q_OBJECT
public:
    GLpoint(GLfloat nx=0,GLfloat ny=0):x(nx),y(ny){}
    GLpoint(const GLpoint& p){x=p.x;y=p.y;}
    GLpoint& operator= (GLpoint& glp){x=glp.x;y=glp.y;return *this;}
public:
    GLfloat x;
    GLfloat y;
};

#endif // GLPOINT_H
