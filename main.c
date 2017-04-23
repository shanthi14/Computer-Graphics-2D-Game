#include <stdio.h>
//#include<SOIL.h>
#include<string.h>
#include<math.h>
#include<string.h>
#include <GL/glut.h>
#include<time.h>
#define DEG2RAD 0.01745329251
#define move_unit 7
#define true 1
#define false 0

void times(int VALUE);
void output(float x, float y, const char *string,int choice);

float color[20][3],color1[20][3];/*={{1,1,0},{0,0,1},{0,1,0},{0.5019,0,0.5019},{1,0.498,0.3137},{0.7529,0.7529,0.7529},{0.980,0.5019,0.447},{1,0.843,0},{1,0.7529,0.796},{0.901,0.901,0.9803},{0.8235,0.4117,0.1176}};//yellow,blue,green,purple,orange,silver,salmon pink,golden yellow,pink,lavendor,brown*/

float size1[20],size2[20],sizep=1;/*={0.4,0.4,0.5,0.5,0.6,1.5,1.3,0.3,0.7,1.4};*/

float psx[700],psy[700],psy2[300];

float posX=0,posY=0,movx[20]={},movm[20]={};

static const int FPS = 60;

static int menuFlag=true,gameFlag=false,jawleft=false,jawright=true,collisionx=false,collisiony=false,game_over=false,fish_ate=false;

int f1[20]={},f2[20]={},fish_count=0,counter=0;


void reshape(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w<=h)
    gluOrtho2D(0.0,1000.0,0*(GLfloat)h/(GLfloat)w,768.0*(GLfloat)h/(GLfloat)w);
    else
    gluOrtho2D(0*(GLfloat)w/(GLfloat)h,1000.0*(GLfloat)w/(GLfloat)h,0.0,768.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();

}


void myinit()
{
    glClearColor(0.2,0.9,1,1.0);
    glColor3f(1.0,0.0,0.0);
    glPointSize(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,1360,0,768);

}


void keyPress(unsigned char key,int x,int y)
{
    switch(key)
    {
        case 49:menuFlag=false;
                gameFlag=true;//49 IS THE ASCII VALUE FOR 1.SO IF PRESSED NUM IS 1 THEN GAME MODE
                break;
        case 52:exit(0);
    }
}


void keyboard(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_RIGHT://RIGHT ARROW AND POSX HAS RIGHT SCREEN LIMIT AS 1600
            jawright=true;
            jawleft=false;
            if(posX<=1600)
            {
                posX+=move_unit;
            }//YOU ARE MOVING BY INCREMENT POSX BY MOVE_UNIT
            break;

        case GLUT_KEY_LEFT:
            jawleft=true;
            jawright=false;
            if(posX>=-800)
            posX-=move_unit;;//DECREMENT ,BECAUSE LEFT SIDE MOTION
            break;

        case GLUT_KEY_UP:
            if(posY<=450)
            posY+=move_unit;;//UP ,SO POSY
            break;

        case GLUT_KEY_DOWN:
            if(posY>=-300)
            posY-=move_unit;;//DOWN
            break;

        case GLUT_KEY_F1://go back to menu
            menuFlag=true;
            gameFlag=false;
            break;


        default:
         break;
    }

    glutPostRedisplay();

}



int i,gameoverX=600,gameoverY=800;
void gameover()
{
    counter++;


        glColor3f(1.0, 0.0, 1.0);
        gameFlag=false;
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        output(gameoverX,gameoverY,"Game Over!",1);
       /* winner(200,800,"score:");
        winner(1000,800,"enter to replay");*/
        glPopMatrix();
        glutSwapBuffers();
        glFlush();
        //if(counter>=100)
        {

            menuFlag=true;

            counter=0;
            posX=0;
            posY=0;
            jawleft=false;
            jawright=true;
            for(i=0;i<20;i++)
            {
                f1[i]=false;
                f2[i]=false;
                movx[i]=0;
                movm[i]=0;
            }

        }


}
void check_collision(int i,int mirr)
{
    if(mirr==0)
    {
        collisionx=((-100+movx[i])+(70*size1[i])>=800+posX) &&(((800+posX)+70*sizep)>=(-100+movx[i]));
        collisiony=(psy[i]+(60*size1[i])>=260+posY) &&((260+posY)+(60*sizep)>=psy[i]);
        if(collisionx==true && collisiony==true)
        {
            if(size1[i]<=0.8)
            {
                fish_ate=true;
                //game_over=false;
                fish_count++;
                f1[i]=false;
                movx[i]=0;
                printf("\nfishcount=%d\n",fish_count);
            }
            else if(size1[i]>0.8)
            {
                fish_ate=false;
                game_over=true;
                printf("game over\n");
                //gameover();

            }
        }

    }
    else
    {
        collisionx=((1800+movm[i])+(70*size2[i])>=800+posX) &&((800+posX)+(70*sizep)>=(1800+movm[i]));
        collisiony=(psy2[i]+(60*size2[i])>=(260+posY)) &&((260+posY)+(60*sizep)>=psy2[i]);
        if(collisionx==true && collisiony==true)
        {
            if(size2[i]<=0.8)
            {
                fish_ate=true;
                //game_over=false;
                fish_count++;
                f2[i]=false;
                movm[i]=0;
            }
            else if(size2[i]>0.8)
            {
                fish_ate=false;
                game_over=true;
                printf("go\n");
                //gameover();

            }
        }

    }

}
int a=0,g=0;
void times(int v)//USED FOR SMOOTH MOTION OR AUTOMATIC
{   a++;
    int i;
    for(i=0;i<20;i++)
    {
        if(a==180*(i+1))//for every 3 seconds new fish will be created.40 frames=1 s.so make flag true
        {
            f1[i]=true;
            f2[i]=true;
         //   printf("%f\n",size1[i]);
        }

    }
    for(i=0;i<20;i++)
    {
        if(movx[i]>=1900)
        {
            f1[i]=false;
            movx[i]=0;
        }
        if(movm[i]<=-2600)
        {
            f2[i]=false;
            movm[i]=0;
        }
    }


    if(a==3600)
    {
        a=0;//when it exceeds 600 seconds reset =>continues motion

    }
    if(gameFlag)
    {
    for(i=0;i<20;i++)
        {
            if(f1[i]==true)
            {
                movx[i]+=4;

            }
            if(f2[i]==true)
            {
                movm[i]-=4;

            }
        //movm-=4;//TOWARDS LEFT
        }
    }
    for(i=0;i<20;i++)
    {
        if(f1[i]==true)
        {
            check_collision(i,0);
        }
        if(f2[i]==true)
        {
            check_collision(i,1);
        }
    }
    glutPostRedisplay();// "MARKS THE CURRENT WINDOW AS NEEDING TO BE REDISPLAYED"
    glutTimerFunc(1000/FPS, times, v);//REGISTERS A TIMER CALLBACK TO BE TRIGGERED IN A SPECIFIED NO OF MILLISECONDS
    //1ST ARG:UNSIGNED INT MS,2ND:VOID (*FUNC)(INT VALUE),3RD:VALUE}
}

void drawfish(float color[],float size1,float x,float psy)
{
//printf("%f\n%f\n%f\n",color[0],color[1],color[2]);
        glBegin(GL_POLYGON);//FISH BODY
        glColor3fv(color);
        glVertex2d(20*size1-x,30*size1+psy);
        glVertex2d(40*size1-x,50*size1+psy);
        glVertex2d(70*size1-x,30*size1+psy);
        glVertex2d(40*size1-x,10*size1+psy);
        glEnd();

        glBegin(GL_TRIANGLES);//FISH TAIL
        glColor3fv(color);
        glVertex2d(0*size1-x,10*size1+psy);
        glVertex2d(0*size1-x,50*size1+psy);
        glVertex2d(20*size1-x,30*size1+psy);
        glEnd();

        glBegin(GL_TRIANGLES);//FISH FINS
        glColor3fv(color);
        glVertex2d(30*size1-x,40*size1+psy);
        glVertex2d(20*size1-x,60*size1+psy);
        glVertex2d(40*size1-x,50*size1+psy);
        glEnd();
        glBegin(GL_TRIANGLES);
        glColor3fv(color);
        glVertex2d(30*size1-x,20*size1+psy);
        glVertex2d(40*size1-x,10*size1+psy);
        glVertex2d(20*size1-x,0*size1+psy);
        glEnd();

        glBegin(GL_LINES);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(60*size1-x,23*size1+psy);
        glVertex2f(45*size1-x,25*size1+psy);
        glEnd();

        glBegin(GL_POINTS);
        glPointSize(2.0);
        glColor3f(0.0,0.0,0.0);
        glVertex2d(55*size1-x,35*size1+psy);
        glEnd();
        glFlush();


}


void drawmirror_fish(float color1[],float size2,float x,float psy2)
{
        glBegin(GL_POLYGON);
        glColor3fv(color1);
        glVertex2d(0*size2-x,30*size2+psy2);
        glVertex2d(30*size2-x,50*size2+psy2);
        glVertex2d(50*size2-x,30*size2+psy2);
        glVertex2d(30*size2-x,10*size2+psy2);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3fv(color1);
        glVertex2d(70*size2-x,10*size2+psy2);
        glVertex2d(70*size2-x,50*size2+psy2);
        glVertex2d(50*size2-x,30*size2+psy2);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3fv(color1);
        glVertex2d(40*size2-x,40*size2+psy2);
        glVertex2d(50*size2-x,60*size2+psy2);
        glVertex2d(30*size2-x,50*size2+psy2);
        glEnd();
        glBegin(GL_TRIANGLES);
        glColor3fv(color1);
        glVertex2d(40*size2-x,20*size2+psy2);
        glVertex2d(30*size2-x,10*size2+psy2);
        glVertex2d(50*size2-x,0*size2+psy2);
        glEnd();

        glBegin(GL_LINES);
        glLineWidth(2.0);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(12*size2-x,22*size2+psy2);
        glVertex2f(25*size2-x,25*size2+psy2);
        glEnd();

        glBegin(GL_POINTS);
        glPointSize(2.0);
        glColor3f(0.0,0.0,0.0);
        glVertex2d(15*size2-x,35*size2+psy2);
        glEnd();
        glFlush();



}


void drawjawy(float x,float y,float size,float a,float b,float c)//JAWY PLAYER
{

    glBegin(GL_POLYGON);//FISH BODY
    glColor3f(a,b,c);//a,b,c ARE COLORS PASSED AS ARGS
    glVertex2d(20*size+x,30*size+y);
    glVertex2d(40*size+x,50*size+y);
    glVertex2d(70*size+x,30*size+y);
    glVertex2d(40*size+x,10*size+y);
    glEnd();

    glBegin(GL_TRIANGLES);//FISH TAIL
    glColor3f(a,b,c);
    glVertex2d(0*size+x,10*size+y);
    glVertex2d(0*size+x,50*size+y);
    glVertex2d(20*size+x,30*size+y);
    glEnd();

    glBegin(GL_TRIANGLES);//FISH FINS
    glColor3f(a,b,c);
    glVertex2d(30*size+x,40*size+y);
    glVertex2d(20*size+x,60*size+y);
    glVertex2d(40*size+x,50*size+y);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(a,b,c);
    glVertex2d(30*size+x,20*size+y);
    glVertex2d(40*size+x,10*size+y);
    glVertex2d(20*size+x,0*size+y);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0,0.0,0.0);
    glVertex2f(70*size+x,30*size+y);
    glVertex2f(50*size+x,22*size+y);
    glEnd();

    /*glBegin(GL_TRIANGLES);
    glColor3f(0,0,0);
    glVertex2f(50*size+x,22*size+y);
    glVertex2f(54*size+x,30*size+y);
    glVertex2f(58*size+x,26*size+y);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0,0,0);
    glVertex2f(58*size+x,26*size+y);
    glVertex2f(60*size+x,32*size+y);
    glVertex2f(64*size+x,28*size+y);
    glEnd();*/

    glBegin(GL_POINTS);
    glPointSize(2.0);
    glColor3f(0.0,0.0,0.0);
    glVertex2d(55*size+x,35*size+y);
    glEnd();

    glFlush();


}
void draw_mirrorjawy(float x,float y,float size,float a,float b,float c)
{
    glBegin(GL_POLYGON);
    glColor3f(a,b,c);
    glVertex2d(0*size+x,30*size+y);
    glVertex2d(30*size+x,50*size+y);
    glVertex2d(50*size+x,30*size+y);
    glVertex2d(30*size+x,10*size+y);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(a,b,c);
    glVertex2d(70*size+x,10*size+y);
    glVertex2d(70*size+x,50*size+y);
    glVertex2d(50*size+x,30*size+y);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(a,b,c);
    glVertex2d(40*size+x,40*size+y);
    glVertex2d(50*size+x,60*size+y);
    glVertex2d(30*size+x,50*size+y);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(a,b,c);
    glVertex2d(40*size+x,20*size+y);
    glVertex2d(30*size+x,10*size+y);
    glVertex2d(50*size+x,0*size+y);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0,0.0,0.0);
    glVertex2f(0*size+x,30*size+y);
    glVertex2f(20*size+x,22*size+y);
    glEnd();

    /*glBegin(GL_TRIANGLES);
    glColor3f(0,0,0);
    glVertex2f(22*size+x,22*size+y);
    glVertex2f(18*size+x,30*size+y);
    glVertex2f(14*size+x,26*size+y);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0,0,0);
    glVertex2f(14*size+x,26*size+y);
    glVertex2f(12*size+x,32*size+y);
    glVertex2f(8*size+x,28*size+y);
    glEnd();*/

    glBegin(GL_POINTS);
    glPointSize(2.0);
    glColor3f(0.0,0.0,0.0);
    glVertex2d(15*size+x,35*size+y);
    glEnd();


    glFlush();
}
void output(float x, float y, const char *string,int choice)
{
      int len, i;

           glRasterPos2f(x,y);
           len=(int) strlen(string);
           for (i = 0; i < len; i++)
			{
			switch(choice){
             case 1:glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
             break;
             case 2:glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
             break;
             default:glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,string[i]);
             }


			}
}




void rect()
{

    glColor3f(1.0, 0.0, 0.0);
    output(100.0,700.0,"JAWS",2);
    output(200.0,400.0,"1. New Game",1);
    output(200.0,300.0,"2. Instructions",1);
    output(200.0,200.0,"3. High Score",1);
    output(200.0,100.0,"4. Exit",1);
}




void clearscreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glPopMatrix();
    glutSwapBuffers();
    glFlush();
}
void gamescreen()
{
    int i;
    for(i=0;i<20;i++){
    if(!f1[i])
    {

        psy[i]=rand()%660+10;
        size1[i]=(rand()%10)/(float)10+0.4;

        color[i][0]=(rand()%255)/(float)255;
        color[i][1]=(rand()%255)/(float)255;
        color[i][2]=(rand()%255)/(float)255;
         //printf("%f\n%f\n%f\n",color[i][0],color[i][1],color[i][2]);
    }}
    for(i=0;i<20;i++){
    if(!f2[i])
    {

        psy2[i]=rand()%660+10;
        size2[i]=(rand()%10)/(float)10+0.3;

        color1[i][0]=(rand()%255)/(float)255;
        color1[i][1]=(rand()%255)/(float)255;
        color1[i][2]=(rand()%255)/(float)255;
         //printf("%f\n%f\n%f\n",color[i][0],color[i][1],color[i][2]);
    }}
        glClear(GL_COLOR_BUFFER_BIT);//CLEARS DISPLAY
        for(i=0;i<20;i++)
        {
        if(f1[i]==true )
        {
            glPushMatrix();//PUSHES CURRENT MATRIX STACK DOWN BY 1,DUPLICATING THE CURRENT MATRIX
            glTranslatef(movx[i],0,0);//FOR AUTOMATIC/SMOOTH MOTION OF NORMAL FISH ,TOWARDS LEFT
            drawfish(color[i],size1[i],100,psy[i]);
            glPopMatrix();//POPS THE CURRENT MATRIX STACK REPLACING THE CURRENT MATRIX WITH THE 1 BELOW IT ON STAC
        }
        if(f2[i]==true )
        {
            glPushMatrix();//PUSHES CURRENT MATRIX STACK DOWN BY 1,DUPLICATING THE CURRENT MATRIX
            glTranslatef(movm[i],0,0);//FOR AUTOMATIC/SMOOTH MOTION OF NORMAL FISH ,TOWARDS LEFT
            drawmirror_fish(color1[i],size2[i],-1800,psy2[i]);
            glPopMatrix();//POPS THE CURRENT MATRIX STACK REPLACING THE CURRENT MATRIX WITH THE 1 BELOW IT ON STAC
        }
    }
//PLAYER
    if(jawright)
    {
    glPushMatrix();
    glTranslatef(posX,posY,0);//REQUIRES SPECIAL FUNCTIONS,MANUAL MOVEMENT,2D SO Z=0
    drawjawy(800,260,sizep,1,0,.1);
    glPopMatrix();
    }

        if(jawleft)
    {
        glPushMatrix();
        glTranslatef(posX,posY,0);;
        draw_mirrorjawy(800,260,sizep,1,0,.1);
        glPopMatrix();

    }
    glutSwapBuffers();//INSTRUCTS THE COMPUTER THAT YOU ARE DONE WITH THE CURRENT FRAME&BUFFERS SHOULD BE SWAPPED
    glFlush();
}
void display()
{
    if(menuFlag)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        rect();
        glPopMatrix();
        glutSwapBuffers();
        glFlush();
    }

    //If in Game mode Display game Screen
    else if(gameFlag)
    {
        gamescreen();
    }


    else
    {
        clearscreen();
    }

}

int main(int argc,char ** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(1360,768);
    glutInitWindowPosition(0,0);
    glutCreateWindow("JAWS");
    glutReshapeFunc(reshape);
            srand(time(NULL));
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPress);
    glutSpecialFunc(keyboard);
    glutTimerFunc(100,times,0);
    myinit();
    glutMainLoop();
    return 0;
}

