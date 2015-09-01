#include "ballcollide.h"

BallCollide::~BallCollide(){
    delete ibplus;
    delete jbplus;
}
/**
 * @brief BallCollide::GetPart calculate v_x,v_y,Angle_x,Angle_y;
 */
void BallCollide::GetPart()
{
   GLfloat Angle_x=atan((jbplus->getOp().y-ibplus->getOp().y)/(jbplus->getOp().x-ibplus->getOp().x));
   if(Angle_x==0){
       {// deal with i
           if((ibplus->getA()>=0&&ibplus->getA()<Pi/2)||(ibplus->getA()>1.5*Pi&&ibplus->getA()<=2*Pi)){
               ibplus->resetA_x(0);
           }else
               ibplus->resetA_x(Pi);
           if((ibplus->getA()>=0&&ibplus->getA()<=Pi))
               ibplus->resetA_y(Pi/2);
           else
               ibplus->resetA_y(Pi*1.5);

           GLfloat iv_x=ibplus->getV()*cos(ibplus->getA_x());
           if(iv_x<0)
               iv_x=-iv_x;
           ibplus->resetV_x(iv_x);

           GLfloat iv_y=ibplus->getV()*sin(ibplus->getA_x());
           if(iv_y<0)
               iv_y=-iv_y;
           ibplus->resetV_y(iv_y);
       }
       {// deal with j
           if((jbplus->getA()>=0&&jbplus->getA()<Pi/2)||(jbplus->getA()>1.5*Pi&&jbplus->getA()<=2*Pi)){
               jbplus->resetA_x(0);
           }else
               jbplus->resetA_x(Pi);
           if((jbplus->getA()>=0&&jbplus->getA()<=Pi))
               jbplus->resetA_y(Pi/2);
           else
               jbplus->resetA_y(Pi*1.5);

           GLfloat jv_x=jbplus->getV()*cos(jbplus->getA_x());
           if(jv_x<0)
               jv_x=-jv_x;
           jbplus->resetV_x(jv_x);

           GLfloat jv_y=jbplus->getV()*sin(jbplus->getA_x());
           if(jv_y<0)
               jv_y=-jv_y;
           jbplus->resetV_y(jv_y);
       }
   }else if(Angle_x>0){
       {//deal with i
        if((ibplus->getA()>Angle_x+Pi/2)&&(ibplus->getA()<=Pi*1.5))
            ibplus->resetA_x(Angle_x+Pi);
        else
            ibplus->resetA_x(Angle_x);

        if((ibplus->getA()>=Angle_x)&&(ibplus->getA()<Angle_x+Pi))
            ibplus->resetA_y(Angle_x+Pi/2);
        else
            ibplus->resetA_y(Angle_x+Pi*1.5);
        GLfloat iv_x=ibplus->getV()*cos(ibplus->getA()-Angle_x);//different situation differ Pi*n,so we can only consider symbol
        if(iv_x<0)
            iv_x=-iv_x;
        ibplus->resetV_x(iv_x);
        GLfloat iv_y=ibplus->getV()*sin(ibplus->getA()-Angle_x);
        if(iv_y<0)
            iv_y=-iv_y;
        ibplus->resetV_y(iv_y);

       }
       {//deal with j
           if((jbplus->getA()>Angle_x+Pi/2)&&(jbplus->getA()<=Pi*1.5))
               jbplus->resetA_x(Angle_x+Pi);
           else
               jbplus->resetA_x(Angle_x);

           if((jbplus->getA()>=Angle_x)&&(jbplus->getA()<Angle_x+Pi))
               jbplus->resetA_y(Angle_x+Pi/2);
           else
               jbplus->resetA_y(Angle_x+Pi*1.5);
           GLfloat jv_x=jbplus->getV()*cos(jbplus->getA()-Angle_x);//different situation differ Pi*n,so we can only consider symbol
           if(jv_x<0)
               jv_x=-jv_x;
           jbplus->resetV_x(jv_x);
           GLfloat jv_y=jbplus->getV()*sin(jbplus->getA()-Angle_x);
           if(jv_y<0)
               jv_y=-jv_y;
           jbplus->resetV_y(jv_y);
       }

   }else{
       Angle_x+=Pi;
       {//deal with i

           if((ibplus->getA()>=Angle_x-Pi/2)&&(ibplus->getA()<Angle_x+Pi/2))
               ibplus->resetA_x(Angle_x);
           else
               ibplus->resetA_x(Angle_x+Pi);
           if(ibplus->getA()>Angle_x&&ibplus->getA()<=Angle_x+Pi)
               ibplus->resetA_y(Angle_x+Pi/2);
           else
               ibplus->resetA_y(Angle_x-Pi/2);
           GLfloat iv_x=ibplus->getV()*cos(ibplus->getA()-Angle_x);
           if(iv_x<0)
               iv_x=-iv_x;
           ibplus->resetV_x(iv_x);
           GLfloat iv_y=ibplus->getV()*sin(ibplus->getA()-Angle_x);
           if(iv_y<0)
               iv_y=-iv_y;
           ibplus->resetV_y(iv_y);
       }
       {//deal with j

           if((jbplus->getA()>=Angle_x-Pi/2)&&(jbplus->getA()<Angle_x+Pi/2))
               jbplus->resetA_x(Angle_x);
           else
               jbplus->resetA_x(Angle_x+Pi);
           if(jbplus->getA()>Angle_x&&jbplus->getA()<=Angle_x+Pi)
               jbplus->resetA_y(Angle_x+Pi/2);
           else
               jbplus->resetA_y(Angle_x-Pi/2);
           GLfloat jv_x=jbplus->getV()*cos(jbplus->getA()-Angle_x);
           if(jv_x<0)
               jv_x=-jv_x;
           jbplus->resetV_x(jv_x);
           GLfloat jv_y=jbplus->getV()*sin(jbplus->getA()-Angle_x);
           if(jv_y<0)
               jv_y=-jv_y;
           jbplus->resetV_y(jv_y);
       }
   }
}
/**
 * @brief BallCollide::update update the ball info by swaping V_x,Angle_x and calculate new V
 */
void BallCollide::update()
{
    GLfloat A_swap;
    GLfloat V_swap;

    A_swap=ibplus->getA_x();
    ibplus->resetA_x(jbplus->getA_x());
    jbplus->resetA_x(A_swap);

    V_swap=ibplus->getV_x();
    ibplus->resetV_x(jbplus->getV_x());
    jbplus->resetV_x(V_swap);
    { // deal with i
        GLfloat ix_x=ibplus->getV_x()*cos(ibplus->getA_x());
        GLfloat ix_y=ibplus->getV_x()*sin(ibplus->getA_x());
        GLfloat iy_x=ibplus->getV_y()*cos(ibplus->getA_y());
        GLfloat iy_y=ibplus->getV_y()*sin(ibplus->getA_y());
        GLfloat iv_x=ix_x+iy_x;
        GLfloat iv_y=ix_y+iy_y;
        GLfloat iv=sqrt(pow(iv_x,2)+pow(iv_y,2));
        iball.resetV(iv);
        GLfloat i_am=atan(iv_y/iv_x);//unfinal angle
        if(i_am>=0){
            if(iv_x>=0)
                iball.resetAngle(i_am);
            else
                iball.resetAngle(i_am+Pi);
        }else{
            if(iv_x<=0)
                iball.resetAngle(i_am+Pi);
            else
                iball.resetAngle(i_am+Pi*2);
        }
    }
    { // deal with j
        GLfloat jx_x=jbplus->getV_x()*cos(jbplus->getA_x());
        GLfloat jx_y=jbplus->getV_x()*sin(jbplus->getA_x());
        GLfloat jy_x=jbplus->getV_y()*cos(jbplus->getA_y());
        GLfloat jy_y=jbplus->getV_y()*sin(jbplus->getA_y());
        GLfloat jv_x=jx_x+jy_x;
        GLfloat jv_y=jx_y+jy_y;
        GLfloat jv=sqrt(pow(jv_x,2)+pow(jv_y,2));
        jball.resetV(jv);
        GLfloat j_am=atan(jv_y/jv_x);//unfinal angle
        if(j_am>=0){
            if(jv_x>=0)
                jball.resetAngle(j_am);
            else
                jball.resetAngle(j_am+Pi);
        }else{
            if(jv_x<=0)
                jball.resetAngle(j_am+Pi);
            else
                jball.resetAngle(j_am+Pi*2);
        }
    }
}

BallCollide::BallCollide(Ball &ib,Ball &jb):iball(ib),jball(jb){
    ibplus=new BallPlus(iball);
    jbplus=new BallPlus(jball);
    GetPart();
}
