#include<math.h>
#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include<cstdio>
#include<string.h>
#include <sys/time.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
#define no_of_coins 9
#define friction 0.0008


// Function Declarations
void drawScene();
void update(int value);
void drawBox(float len);
void drawBall(float rad);
void drawTriangle();
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);
void handleMouseActiveMotion(int x, int y);
void processMousePassiveMotion(int x, int y);


// Global Variables
float pos_x = 0.0f;
float pos_y = -2.0f;

float vel_x = 0.00f;
float vel_y = 0.04f;

float ball_rad = 0.15f;
float box_len = 6.0f;
float tri_x = 0.0f;
float tri_y = 0.0f;
float theta = 0.0f; 

struct timeval start;
struct timeval now;

class Coins
{
public:
    char type;
    double rad;
    double pos_x;
    double pos_y;
    double vel_x;
    double vel_y;
    double fi;
    int mov;
    int speed;
    Coins(double a, double x,double y,char ch,double vx,double vy)
    {
        rad=a;
        pos_x=x;
        pos_y=y;
        type=ch;
        vel_x=vx;
        vel_y=vy;
    }
    void drawcoin()
    {
        glTranslatef(pos_x, pos_y, 0.0f);
        if(type=='w')
            glColor3f(1.0f, 1.0f, 1.0f);
        else if(type=='b')
            glColor3f(0.0f, 0.0f, 0.0f);
        else if(type=='s')
            glColor3f(0.0f, 0.0f, 1.0f);
        else if(type=='q')
            glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        for(int i=0 ; i<360 ; i++) {
            glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
        }
        glEnd();

        if(type=='s'&&mov!=1)
        {
        glPushMatrix();
        glRotatef(fi,0.0,0.0,1.0);
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 1.0f);
        
        glVertex3f(-0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.8, 0.0 );
        
        glVertex3f(-0.15, 0.35, 0.0);
        glVertex3f(0.0, 0.8, 0.0 );

        glVertex3f(0.15, 0.35, 0.0);
        glVertex3f(0.0, 0.8, 0.0 );

        glEnd();
        glPopMatrix();
        }

        glPushMatrix();
        glLoadIdentity();
        glTranslatef(-4.5, -2.0, -8.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        for(int j=speed;j>=0;j--)
        {
        if(j%2) glColor3f(1.0f, 0.0f, 0.0f);
        else glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
        for(int i=0 ; i<180 ; i++) {
            glVertex2f(0.1*j * cos(DEG2RAD(i)), 0.1*j * sin(DEG2RAD(i)));
        }
        glEnd();
        }
        glPopMatrix();
    }
};

class Striker : public Coins
{
public:
    
    Striker(double a, double x,double y,char ch,double vx,double vy) : Coins(a,x,y,ch,vx,vy)
    {
       fi=0.0;
       mov=0;
       speed=0;
    }
    void move(int key)
    {
        if(mov==1)
            return;
        if (key == GLUT_KEY_LEFT)
        pos_x -= 0.1;
        if (key == GLUT_KEY_RIGHT)
        pos_x += 0.1;
    }
    void mousemove(int x)
    {
    
        if(mov!=1)
            pos_x=x*10.0/glutGet(GLUT_WINDOW_WIDTH)-5.0;

    }
    void powerstriker(int x,int y)
    {
        if(mov==1)
            return;

        x=x*15.0/glutGet(GLUT_WINDOW_WIDTH)-7.5;
        y=y*-7.5/glutGet(GLUT_WINDOW_HEIGHT)+3.75;
       
        fi=atan((x-pos_x)/(y-pos_y))*-180.0/PI;

        speed=(int)(5*sqrt(friction*sqrt(pow(x-pos_x,2)+pow(y-pos_y,2)))/0.03);
       
        if(speed>12)
            speed=12;
    }

    void handleSpeed(int key)
    {
        if(mov==1)
            return;

        
   
        
        if(key==GLUT_KEY_UP)
        {
            if(speed<=12)
             speed++;

        }  
        if(key==GLUT_KEY_DOWN)
        { 
            if(speed>=0)
            speed--;
        }
        
        
       
    }
    void rotate(int key)
    {
       
        
        if(key==97)
        { 
          fi +=3.0;
         
        }
        if(key==99)
        { 
          fi -=3.0;
         
        } 
        

    }
    void launch(int key)
    {
        if(mov==1)
            return;
        if(key==32)
        { 
          mov=1;
          float f=fi* PI/180;
          vel_x=-0.03*speed*sin(f);
          vel_y=0.03*speed*cos(f);
      }
      
    }


};
class Hole
{
private:
    
public:
    double rad;
    double pos_x;
    double pos_y;
    Hole(double a, double x,double y)
    {
        rad=a;
        pos_x=x;
        pos_y=y;
    }
    void drawhole()
    {
        glTranslatef(pos_x, pos_y, 0.0f);
        glColor3f(0.21f, 0.16f, 0.1f);
        glBegin(GL_TRIANGLE_FAN);
        for(int i=0 ; i<360 ; i++) {
            glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
        }
        glEnd();
    }
    
};

class Board
{
private:
    float len;
public:
    Board(float a)
    {
        len=a;
    }
    void drawsides(float len)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);
        glBegin(GL_QUADS);
        glVertex2f(-len / 2, -len / 2);
        glVertex2f(-len / 2, len / 2);
        glVertex2f(len / 2, len /2 );
        glVertex2f(len / 2 , -len /2);
        glEnd();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }

   void drawBox() 
   {
        glColor3f(0.0F,0.0F,0.0F);
        drawsides(len*1.1);
        glColor3f(1.0F,0.6980F,0.4F);
        drawsides(len);
        for(int i=0;i<4;i++)
        {
            glRotatef(90 , 0.0 ,0.0 , 1.0);
            drawMargin();
        }

    }

    void drawMargin()
    {
        
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        
        glVertex3f(-0.4*len, 0.325*len, 0.0);
        glVertex3f(-0.4*len, -0.325*len, 0.0 );

        glVertex3f(-0.35*len, 0.325*len, 0.0);
        glVertex3f(-0.35*len, -0.325*len, 0.0 );

        
        glEnd();
        float rad=0.025*len;
        
        glPushMatrix();

        glTranslatef(-0.375*len, 0.325*len, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        for(int i=0 ; i<360 ; i++) {
            glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
        }
        glEnd();

        glPopMatrix();
        
        glPushMatrix();

        glTranslatef(-0.375*len, -0.325*len, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        for(int i=0 ; i<360 ; i++) {
            glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
        }
        glEnd();

        glPopMatrix();
        
        rad=0.15*len;
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        for(int i=0 ; i<360 ; i++) {
            glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
        }
        glEnd();
        rad=rad-0.08;
        glColor3f(1.0F,0.6980F,0.4F);
        glBegin(GL_TRIANGLE_FAN);
        for(int i=0 ; i<360 ; i++) {
            glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
        }
        glEnd();




    }


};

        Coins white[no_of_coins]={
        Coins(0.12,0.6,0.0,'w',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(45)),0.6*sin(DEG2RAD(45)),'b',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(90)),0.6*sin(DEG2RAD(90)),'w',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(135)),0.6*sin(DEG2RAD(135)),'b',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(180)),0.6*sin(DEG2RAD(180)),'w',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(225)),0.6*sin(DEG2RAD(225)),'b',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(270)),0.6*sin(DEG2RAD(270)),'w',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(315)),0.6*sin(DEG2RAD(315)),'b',0.0,0.0),
        Coins(0.12,0.0,0.0,'q',0.0,0.0),
            };
        
        Coins black[no_of_coins]={
        Coins(0.12,0.6,0.0+4.0,'w',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(45))+4.0,0.6*sin(DEG2RAD(45)),'b',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(90))+4.0,0.6*sin(DEG2RAD(90)),'w',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(135))+4.0,0.6*sin(DEG2RAD(135)),'b',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(180))+4.0,0.6*sin(DEG2RAD(180)),'w',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(225))+4.0,0.6*sin(DEG2RAD(225)),'b',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(270))+4.0,0.6*sin(DEG2RAD(270)),'w',0.0,0.0),
        Coins(0.12,0.6*cos(DEG2RAD(315))+4.0,0.6*sin(DEG2RAD(315)),'b',0.0,0.0),
        Coins(0.12,0.0+4.0,0.0,'q',0.0,0.0),
            };
        Striker player=Striker(0.15,0.0,-0.375*box_len,'s',0.0,0.0);

        Hole h[]={Hole(0.25f,2.750,2.750),Hole(0.25f,-2.750,2.750),Hole(0.25f,2.750,-2.750),Hole(0.25f,-2.750,-2.750)};

int reset=0;
int main(int argc, char **argv) {

    
    gettimeofday(&start, NULL);
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    int w = glutGet(GLUT_SCREEN_WIDTH);
    int h = glutGet(GLUT_SCREEN_HEIGHT);
    int windowWidth = w * 2/3;
    int windowHeight = h* 2/3;
    

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

    glutCreateWindow("Assignment~1 Carrom Game");  // Setup the window
    initRendering();
    glClearColor(0.0f, 0.80784f, 1.0f, 1.0f);
 

    glutDisplayFunc(drawScene);
    glutIdleFunc(drawScene);
    glutKeyboardFunc(handleKeypress1);

   
    

    glutSpecialFunc(handleKeypress2);

    glutReshapeFunc(handleResize);
    glutMouseFunc(handleMouseclick);
    glutMotionFunc(handleMouseActiveMotion);
    glutPassiveMotionFunc(processMousePassiveMotion);
    //glutEntryFunc(processMouseEntry);


   
    glutTimerFunc(10, update, 0);

    glutMainLoop();
    return 0;
}

int score=30;

void RenderString(float x, float y, void *font, const char* string,float r,float g,float b)
{  

  glColor3f(r, g, b); 
  glRasterPos3f(x, y,0);
  int xx = strlen(string);

  for (int i=0;i<xx;i++)
  glutBitmapCharacter(font, string[i]);
}

void calcScore()
{
    char str[50]; 
    gettimeofday(&now, NULL);
    int s=score-(now.tv_sec-start.tv_sec);
    sprintf(str,"%d",s);
    glBegin(GL_QUADS);
        glVertex2f(-5.0, 4.0);
        glVertex2f(-3.5, 4.0);
        glVertex2f(-3.5, 2.0);
        glVertex2f(-5.0, 2.0);
    glEnd();
    RenderString(-4.5,3.0, GLUT_BITMAP_TIMES_ROMAN_24, "Score: ", 1.0,0.2705,0.0);
    RenderString(-3.8,3.0, GLUT_BITMAP_TIMES_ROMAN_24, str, 0.0f, 1.0f, 0.0f);
}
    

// Function to draw objects on the screen
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    // Draw Box
    glTranslatef(0.0f, 0.0f, -8.0f);

    

    glColor3f(1.0f, 0.0f, 0.0f);
    Board b=Board(box_len);
    b.drawBox();
    //b.drawMargin();

    // Draw Ball


    for(int i=0;i<4;i++)
    {
        glPushMatrix();
        h[i].drawhole();
        glPopMatrix();

    }

    for(int i=0;i<no_of_coins;i++)
    {
    glPushMatrix();
    //white[i]=Coins(ball_rad,pos_x+i*0.4,pos_y+i*0.4,'w',vel_x+i*0.005,vel_y-i*0.005);
    white[i].drawcoin();

    glPopMatrix();
    }

    glPushMatrix();
    player.drawcoin();
    glPopMatrix();
   

    calcScore();
 
    glPopMatrix();

    glutSwapBuffers();
}


void applyfriction(double *a , double *b )
{
    float k=abs(atan(*b / *a));
    if(*a==0)
        k=PI/2;

   
    if(*a>0)
    { 
       
        *a -= friction* cos(k);
        
        if(*a<0)
            *a=0;
    }
    else if(*a<0)
    {
        *a += friction* cos(k);
        if(*a>0)
            *a=0;
    }
    if(*b>0)
    { 

    
        *b -= friction* sin(k);
        if(*b<0)
            *b=0;
    } 
    else if(*b<0)
    {
        *b += friction* sin(k);
        if(*b>0)
            *b=0;
    } 

}
void update(int value) {
    
   
    glLoadIdentity();
   
    for(int i=0;i<no_of_coins;i++)
    {
    if(white[i].pos_x + white[i].rad > box_len/2 || white[i].pos_x - white[i].rad < -box_len/2)
    {
        
        white[i].vel_x *= -1;
    }   
    if(white[i].pos_y + white[i].rad > box_len/2 || white[i].pos_y - white[i].rad < -box_len/2)
    {
        white[i].vel_y *= -1;
        
    } 
    for(int j=i+1;j<no_of_coins;j++)
    {
    if(sqrt(pow(white[i].pos_x-white[j].pos_x,2)+pow(white[i].pos_y-white[j].pos_y,2)) < 2*ball_rad)
    {
        double angle=atan((white[i].pos_y-white[j].pos_y)/(white[i].pos_x-white[j].pos_x));
        double si=sin(angle);
        double co=cos(angle);
        double bll1_velx=white[i].vel_x;
        double bll1_vely=white[i].vel_y;
        double bll2_velx=white[j].vel_x;
        double bll2_vely=white[j].vel_y;

        {
        white[i].vel_x=(bll1_velx*si-bll1_vely*co)*si + (bll2_velx*co+bll2_vely*si)*co;
        white[j].vel_x=(bll2_velx*si-bll2_vely*co)*si + (bll1_velx*co+bll1_vely*si)*co;
        white[i].vel_y= -1*(bll1_velx*si-bll1_vely*co)*co + (bll2_velx*co+bll2_vely*si)*si;
        white[j].vel_y= (bll2_velx*si-bll2_vely*co)*co - (bll1_velx*co+bll1_vely*si)*si;
        }  

       

    }
    
    }
    if(sqrt(pow(white[i].pos_x-player.pos_x,2)+pow(white[i].pos_y-player.pos_y,2)) <= 2*ball_rad)
    {
        double angle=atan((white[i].pos_y-player.pos_y)/(white[i].pos_x-player.pos_x));
        double si=sin(angle);
        double co=cos(angle);
        double bll1_velx=white[i].vel_x;
        double bll1_vely=white[i].vel_y;
        double bll2_velx=player.vel_x;
        double bll2_vely=player.vel_y;
  
        {
        white[i].vel_x=(bll1_velx*si-bll1_vely*co)*si + (bll2_velx*co+bll2_vely*si  )*co;
        player.vel_x=(bll2_velx*si-bll2_vely*co)*si + ( bll1_velx*co+bll1_vely*si)*co ;
        white[i].vel_y= -1*(bll1_velx*si-bll1_vely*co)*co + (bll2_velx*co+bll2_vely*si )*si ;
        player.vel_y= (bll2_velx*si-bll2_vely*co)*co - ( bll1_velx*co+bll1_vely*si)*si ;
        }
        PlaySound("wood.mp3", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);



      
    }


    
    }
    
    if(player.pos_x + player.rad > box_len/2 || player.pos_x - player.rad < -box_len/2)
    {
      
        player.vel_x *= -1;
    }  
    if(player.pos_y + player.rad > box_len/2 || player.pos_y - player.rad < -box_len/2)
    {
     
        player.vel_y *= -1;
    }
    

    for(int i=0;i<4; i++)
    {

        for(int j=0;j<no_of_coins;j++)
        {
          if(sqrt(pow(h[i].pos_x-white[j].pos_x,2)+pow(white[j].pos_y-h[i].pos_y,2)) <= h[i].rad) 
            {
                white[j]=black[j];
                /*white[j].vel_x=0;
                white[j].vel_y=0;
                white[j].pos_x=12.0;
                white[j].pos_y=12.0;*/
                cout<<white[j].type<<"  badass\n";
                if(white[j].type=='w')
                {
                    cout<<"grfrs";
                    score+=10;
                }
                else if(white[j].type=='b')
                    score-=5;
                else if(white[j].type=='q')
                    score+=50;
            } 
        }
    }
    for(int i=0;i<4; i++)
    {
    if(sqrt(pow(h[i].pos_x-player.pos_x,2)+pow(player.pos_y-h[i].pos_y,2)) <= h[i].rad) 
    {
      
          player=Striker(0.2,0.0,-0.375*box_len,'s',0.0,0.0); 
          score-=5;
    } 
    }

    if(player.mov==1)
    {
        player.pos_x += player.vel_x;
        player.pos_y += player.vel_y;
      
        applyfriction(&player.vel_x,&player.vel_y);
        
     
    } 
    for(int i=0;i<no_of_coins;i++)
    {
    white[i].pos_x += white[i].vel_x;
    white[i].pos_y += white[i].vel_y;
    applyfriction(&white[i].vel_x,&white[i].vel_y);
    }
    
    if(player.vel_x==0 && player.vel_y==0 && player.mov==1)
    {   
        int flag=0;
        for(int i=0;i<no_of_coins;i++)
            if(abs(white[i].vel_x)>=0.0001 || abs(white[i].vel_y)>=0.0001)
            {
               
                flag=1;
            }
        if(flag==0)
        {
            player=Striker(0.2,0.0,-0.375*box_len,'s',0.0,0.0);
        }
    }
    glutTimerFunc(10, update, 0);
}



void drawBall(float rad) {
    
   glBegin(GL_TRIANGLE_FAN);
   for(int i=0 ; i<360 ; i++) {
       glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
   }
   glEnd();
}

void drawTriangle() {

    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();
}


void initRendering() {

    glEnable(GL_DEPTH_TEST);      
    glEnable(GL_COLOR_MATERIAL);    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   
}


void handleResize(int w, int h) {

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) {

    if (key == 27) {
        exit(0);     // escape key is pressed
    }
    if(key=='r')
        reset=1;
    player.rotate(key);
    player.launch(key);
   
}


void handleKeypress2(int key, int x, int y) {

    
    player.move(key);
    player.handleSpeed(key);
    
}

int but;

void handleMouseclick(int button, int state, int x, int y) {

    if ((state == GLUT_UP))
    {
        but=100;
        
        if (button == GLUT_LEFT_BUTTON) {
            player.launch(32);
        }
    }
    if ((state == GLUT_DOWN))
    {
        but=button;
       
        
    }
    
}


void handleMouseActiveMotion(int x, int y) {

   
    if(but==GLUT_RIGHT_BUTTON) {
       player.mousemove(x); 
   }
   if(but==GLUT_LEFT_BUTTON)
      player.powerstriker( x, y);
    
}

void processMousePassiveMotion(int x, int y) {
    
   
    
}
