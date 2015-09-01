#include "rball.h"
RBall::RBall(QWidget *parent) :
    QGLWidget(parent)
{
    setGeometry( 30, 30, 640, 480 );
    pause=false;
}

void RBall::initializeGL(){
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClearDepth( 1.0 );

    glEnable( GL_DEPTH_TEST );

    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}
/**
 * @brief RBall::paintGL
 */
void RBall::paintGL(){

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//清楚屏幕和深度缓存。

    glLoadIdentity();//重置当前的模型观察矩阵

  // glTranslatef( 0, 0.0, -1.0 );// move current point from original point(0,0,0)
    //glRotatef( 0, 0.0, 1.0, 0.0 );

for(auto ball:VB)
   DrawBall(ball);
}

void RBall::resizeGL(int width,int height){


    if(height==0)
        height=1;
    SWidth=width;
    SHeight=height;
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    //选择投影矩阵。
    glLoadIdentity();
   //重置投影矩阵。
   // gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
    gluOrtho2D(-width/2,width/2,-height/2,height/2);
   // 建立透视投影矩阵。
    glMatrixMode(GL_MODELVIEW);
    // 选择模型观察矩阵。
    glLoadIdentity();
    startTimer(10);
}

void RBall::timerEvent(QTimerEvent *){
    if(!pause)
    {
        for(Ball &ball:VB){
            ball.move();
        }
        Collide();
        updateGL();
    }
}

RBall& RBall::addNewBall(Ball &newBall){

            VB.push_back(newBall);
            return *this;
}
void RBall::Collide(){
    ColWall();
    ColBall();
}
/**
 * @brief RBall::ColWall
 * @return times of collide
 */
int RBall::ColWall(){
    int times=0;
    std::vector<Ball>::iterator i_ball;
    for(i_ball=VB.begin();i_ball!=VB.end();i_ball++){
        Ball &ball=*i_ball;
        if((ball.getOp().x+ball.getR())>=SWidth/2&&ball.getPrep().x<ball.getOp().x){
            //if(){
            GLfloat V_a=Pi-ball.getA();
            if(V_a<0)
                V_a+=Pi*2;
            if(V_a>Pi*2)
                V_a-=Pi*2;
            ball.resetAngle(V_a);
            times++;
            //}
        }
        if((ball.getOp().x-ball.getR())<=-SWidth/2&&ball.getPrep().x>ball.getOp().x){
            GLfloat V_a=Pi-ball.getA();
            if(V_a<0)
                V_a+=Pi*2;
            if(V_a>Pi*2)
                V_a-=Pi*2;
            ball.resetAngle(V_a);
            times++;
        }
        if((ball.getOp().y+ball.getR())>=SHeight/2&&ball.getPrep().y<ball.getOp().y){
            GLfloat V_a=2*Pi-ball.getA();
            if(V_a<0)
                V_a+=Pi*2;
            if(V_a>Pi*2)
                V_a-=Pi*2;
            ball.resetAngle(V_a);
            times++;
        }
        if((ball.getOp().y-ball.getR())<=-SHeight/2&&ball.getPrep().y>ball.getOp().y){
            GLfloat V_a=2*Pi-ball.getA();
            if(V_a<0)
                V_a+=Pi*2;
            if(V_a>Pi*2)
                V_a-=Pi*2;
            ball.resetAngle(V_a);
            times++;
        }
    }
    return times;
}
/**
 * @brief RBall::ColWall
 * @return times of collide
 */
int RBall::ColBall(){
    int times=0;
    std::vector<Ball>::iterator i_ball;
    for(i_ball=VB.begin();i_ball!=VB.end();i_ball++){
        for(auto j_ball=(i_ball+1);j_ball!=VB.end();j_ball++){
            Ball &iball=*i_ball;
            Ball &jball=*j_ball;
            int CurDistence=BallDistence(iball,jball,CurPoint);
            if(CurDistence<=iball.getR()+jball.getR())
            {
                int PreDistence=BallDistence(iball,jball,PrePoint);
              if(PreDistence<=CurDistence){// far away from another ball
               // if(false){
                    return times;
                }else{
                   BallCollide bc(iball,jball);
                   bc.update();
                }
            }
        }

    }
    return times;
}

/**
 * @brief RBall::BallDistence
 * @param i first ball
 * @param j second ball
 * @param tp point type
 * @return distence between balls
 */
int RBall::BallDistence(Ball &i, Ball &j,typepoint tp){
    int distence=0;
    if(tp==CurPoint)
       distence=sqrt(pow((i.getOp().x-j.getOp().x),2)+pow((i.getOp().y-j.getOp().y),2));
    else
       distence=sqrt(pow((i.getPrep().x-j.getPrep().x),2)+pow((i.getPrep().y-j.getPrep().y),2));
    return distence;
}
/**
 * @brief DealAngle  shift the angle to acute angle by add Pi
 * @param Angle
 * @return
 */
GLfloat RBall::DealAngleToAcute(GLfloat Angle){
    if(Angle>0)
    Angle=-Angle;
    GLfloat a;
    for( a=Angle;a<0;a+=Pi);
    if(a>Pi/2)
        a=-(a-Pi);
    return a;
}
/**
 * @brief RBall::DrawBall
 * @param ball the ball which will be draw
 * @return  RBll
 */
RBall& RBall::DrawBall(Ball& ball){

    glLoadIdentity();
    GLfloat R=ball.getR();
    GLpoint P=ball.getOp();
    GLRGBcolor color;
    color=ball.getColor();
    glBegin(GL_POLYGON);
    for(int i=0; i<Np_Circle; ++i){
        glColor3ub(color.r,color.g,color.b);
        glVertex2f(R*cos(2*Pi/Np_Circle*i)+P.x, R*sin(2*Pi/Np_Circle*i)+P.y);
    }
    glEnd();

    return *this;
}
