#include <stdio.h>
#include<iostream>
#include<stdlib.h>
#include "SOIL.h"
#include<string.h>
#include<math.h>
#include<set>
#include<string.h>
#include <GL/glut.h>
#include<time.h>
#define DEG2RAD 0.01745329251
#define move_unit 7
#define true 1
#define false 0
using namespace std;

void times(int VALUE);

void gameover();

void output(float x, float y, const char *string,int choice);

float color[20][3],color1[20][3];

float size1[20],size2[20],sizep=0.6,instrSize=0.0;

float psx[700],psy[700],psy2[300];

float posX=0,posY=0,movx[20]={},movm[20]={};

static const int FPS = 60;

static int menuFlag=true,gameFlag=false,jawleft=false,jawright=true,collisionx=false,collisiony=false,game_over=false,fish_ate=false,instruction_flag=false,congo=false,highFlag=false,highwindflag=false,highfill=false,aboutflag=false;

int f1[20]={},f2[20]={},score=0,counter=0,lastscore;

char scankey;

int scankeyCount=0;


int a=0,t=0,HighScore,frame=0;

GLuint tex2_2d[50];


set<pair <int,string> > highScoreList;
FILE* highScoreFile;

void drawObject(int num,float x,float y,float xsize,float ysize)
{
    //Clears the window with current clearing color

       //Sets current drawing color
       //NOTE: Values are in float format, so 1.0f is full intensity
    glColor3f(0.8549f, 0.6470f, 0.125f);

    //Draws a square/rectangle with above drawing color

    glEnable(GL_TEXTURE_2D);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, tex2_2d[num]);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex2f(x, y);
    glTexCoord2f(1.0, 0.0); glVertex2f(x, y+ysize);
    glTexCoord2f(1.0, 1.0); glVertex2f(x+xsize, y+ysize);
    glTexCoord2f(0.0, 1.0); glVertex2f(x+xsize, y);
    glEnd();

    //Swaps the onscreen and offscreen buffers and flushes them
    //glutSwapBuffers();
    glDisable(GL_TEXTURE_2D);

}



void load_high()
{
  string temp1;
  char temp1c[30];
  int temp2;
  pair <int,string> highPair;
  highScoreFile=fopen("highscores.txt","a+");
  while(fscanf(highScoreFile,"%s %d",temp1c,&temp2)!=EOF)
  {
        temp1=temp1c;
        highPair.first=temp2;//number
        highPair.second=temp1;//name or char
        highScoreList.insert(highPair);
  }

  fclose(highScoreFile);
  highScoreFile=fopen("highscores.txt","a");
  HighScore=(*highScoreList.rbegin()).first;
}

void highScoreWindow()
{

    if(highwindflag)
    {

        glPushMatrix();
        drawObject(2,683-1366*instrSize/2.0,384-768*instrSize/2.0,1366*instrSize,768*instrSize);
        glPopMatrix();
        if(!highfill&&instrSize<=0.6)
            instrSize+=0.004166667;
        if(instrSize>=0.59)
        {
            glColor3f(1,0,1);
            output(1200,100,"Press F1 to go back to menu",2);
            glColor3f(1,0.9,0);
            output(400,500,"Name",1);
            output(700,500,"Score",1);
            set< pair<int,string> >::reverse_iterator iter;int itc=0;
            for(iter=highScoreList.rbegin();iter!=highScoreList.rend()&&itc<10;iter++,itc++)
            {
                output(400,450-40*itc,(*iter).second.c_str(),1);
                char tempdis[30];
                sprintf(tempdis,"%d",(*iter).first);
                output(700,450-40*itc,tempdis,1);
            }
        }
        if(highfill&&instrSize>=0)
            instrSize-=0.004166667;
        if(instrSize<0.001){instrSize=0;
            highwindflag=false;}

    }
}

    string name;
    //For displaying and taking and showing the input from highscore menu
void highScoreMenu()
{
    drawObject(1,171,96,1024,576);
    if(scankey==8&&scankeyCount==0)
    {
        name.erase(name.size()-1);
    }
    else if(scankey!=0&&scankeyCount==0)
            name.append(&scankey);
    output(500,450,name.c_str(),2);
    if(scankey==13&&scankeyCount==0)
    {
        highFlag=false;
        pair <int,string> highScorePair(lastscore,name);
        highScoreList.insert(highScorePair);
       set< pair <int,string> >::iterator iter;
        for(iter=highScoreList.begin();iter!=highScoreList.end();iter++)
        {
            cout<<(*iter).second<<"\t"<<(*iter).first<<endl;

        }
        scankey=-100;
        char tempname[30];
        strcpy(tempname,name.c_str());
        tempname[name.size()-1]='\0';
        fprintf(highScoreFile,"%s\t%d\n",tempname,lastscore );
        name.erase();
    }
    char score[30];
    sprintf(score,"%d",lastscore);
    output(600,400,score,2);
    scankeyCount++;
}



void load_textures()
{
    int i;
    char path[100][100]={"3.png","highScorePage.png","highScoreWindow.png","arrow-keys-vectors_637061.png","small.png","big.png"};
    for(i=0;i<19;i++)
    tex2_2d[i] = SOIL_load_OGL_texture
    (
        path[i],
        SOIL_LOAD_RGBA,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_NTSC_SAFE_RGB
    );
    glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//............BIRD...............
//texture 1 active
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);



}



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
    glClearColor(1,1,1,1);
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
        case 49:
                if(menuFlag==true && gameFlag==false && instruction_flag== false&&highwindflag==false)
                {
                    menuFlag=false;
                    gameFlag=true;//49 IS THE ASCII VALUE FOR 1.SO IF PRESSED NUM IS 1 THEN GAME MODE
                    game_over=false;
                    instruction_flag=false;
                    highwindflag=false;
                }
                break;
        case 50:
                if(!instruction_flag)
                {
                    menuFlag=false;
                    instruction_flag=true;
                    gameFlag=false;
                    game_over=false;
                    highwindflag=false;
                }
                break;
        case 51:if(!highwindflag)
                {
                    highwindflag=true;
                    highfill=false;
                }break;
        case 52:if(!aboutflag)
                {
                    aboutflag=true;
                    menuFlag=false;
                    instruction_flag=false;
                    gameFlag=false;
                    game_over=false;
                    highwindflag=false;
                }
                break;
        case 53:exit(0);

    }
    if(highFlag)
    {
        if((key>64&&key<91)||(key>96&&key<128)||key==8||key==13)
        {
            scankey=key;scankeyCount=0;
        }
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
            if(instruction_flag)
            {
                instruction_flag=false;
                menuFlag=true;

            }
            else if(gameFlag)
            {
                menuFlag=true;
                gameFlag=false;
                gameover();
            }

            else if(highwindflag)
            {
                highwindflag=false;
                highfill=false;
            }
            else if(aboutflag)
            {
                menuFlag=true;
                gameFlag=false;
                aboutflag=false;
            }

        default:
         break;
    }

    glutPostRedisplay();
    glutSwapBuffers();

}



int v,gameoverX=600,gameoverY=455;

void gameover()
{
    set< pair<int,string> >::reverse_iterator iter;int itcount=0;
    //counter++;
    a=0;frame=0;
    sizep=0.5;
    posX=0;
    posY=0;
    jawleft=false;
    jawright=true;
    for(v=0;v<20;v++)
    {

        f1[v]=false;
        f2[v]=false;
        movx[v]=0;
        movm[v]=0;
    }
    for(iter=highScoreList.rbegin();iter!=highScoreList.rend()&&itcount<=10;iter++,itcount++){
        if(score>=(*iter).first)
            highFlag=true;
    }
    if(counter>=250)
    {
        lastscore=score;
        game_over=false;printf("100 over\n");
        gameFlag=false;
        menuFlag=true;
        counter=0;
    }


}


void check_collision(int i,int mirr)
{

    if(mirr==0)
    {
        collisionx=((-100+movx[i])+(70*size1[i])>=800+posX) &&(((800+posX)+70*sizep)>=(-100+movx[i]));
        collisiony=(psy[i]+(60*size1[i])>=260+posY) &&((260+posY)+(60*sizep)>=psy[i]);
        if(collisionx==true && collisiony==true )
        {
            if(size1[i]<=sizep&&jawright==false)
            {
                if(size1[i]<=0.6)
                {
                    score++;
                    fish_ate=true;
                }
                else if(size1[i]>0.6 && size1[i]<=0.8)
                {
                    score+=2;
                    fish_ate=true;
                }
                else if(size1[i]>0.8 && size1[i]<=1.0)
                {
                    score+=3;
                    fish_ate=true;
                }
                else if(size1[i]>1.0)
                {
                    score+=4;
                }


                if(score%7==0)
                {
                    sizep=sizep+0.1;

                }


                f1[i]=false;
                movx[i]=0;

            }
            else if(size1[i]>sizep)
            {
                fish_ate=false;
                game_over=true;
                printf("game over\n");
                gameover();

            }
        }

    }
    else
    {
        collisionx=((1800+movm[i])+(70*size2[i])>=800+posX) &&((800+posX)+(70*sizep)>=(1800+movm[i]));
        collisiony=(psy2[i]+(60*size2[i])>=(260+posY)) &&((260+posY)+(60*sizep)>=psy2[i]);
        if(collisionx==true && collisiony==true)
        {
            if(size2[i]<=sizep&&jawleft==false)
            {

                 if(size2[i]<=0.6)
                {
                    score++;
                    fish_ate=true;
                }
                else if(size2[i]>0.6 && size2[i]<=0.8)
                {
                    score+=2;
                    fish_ate=true;
                }
                else if(size2[i]>0.8 && size2[i]<=1.0)
                {
                    score+=3;
                    fish_ate=true;
                }
                else if(size2[i]>1.0)
                {
                    score+=4;
                }


                if(score%7==0)
                {

                    sizep=sizep+0.1;

                }



                f2[i]=false;
                movm[i]=0;
            }
            else if(size2[i]>sizep)
            {
                fish_ate=false;
                game_over=true;
                printf("go\n");
                gameover();

            }

        }


    }

}

void times(int v)//USED FOR SMOOTH MOTION OR AUTOMATIC
{
    if(gameFlag)
    {
        a++;
        frame++;

    }


    if(game_over==true)
    {
        counter++;
        gameover();
    }

    int i;
    for(i=0;i<20;i++)
    {
        if(a==120*(i+1))//for every 2 seconds new fish will be created.40 frames=1 s.so make flag true
        {
            f1[i]=true;
            f2[i]=true;
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


    if(a==2400)
    {
        a=0;//when it exceeds 600 seconds reset =>continues motion

    }
    if(gameFlag)
    {
        for(i=0;i<20;i++)
            {
                if(f1[i]==true)
                {
                    movx[i]+=6;

                }
                if(f2[i]==true)
                {
                    movm[i]-=6;

                }
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
    glutTimerFunc(999/FPS, times, v);//REGISTERS A TIMER CALLBACK TO BE TRIGGERED IN A SPECIFIED NO OF MILLISECONDS
    //1ST ARG:UNSIGNED INT MS,2ND:VOID (*FUNC)(INT VALUE),3RD:VALUE}
}

void drawfish(float color[],float size1,float x,float psy)
{

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

void drawTextNum(char ch[],int numtext,int xpos, int ypos)//counting the score
{
    int len;
    int k,i;
    k = 0;
    len = numtext - strlen (ch);
    glLoadIdentity ();
    glRasterPos2f( xpos , ypos);
    for (i = 0; i <=numtext - 1; i++)
    {
    if ( i < len )
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'0');
    else
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch[k]);
        k++;
    }

    }
}






void rect()
{

    glColor3f(1.0, 0.0, 0.0);
    output(700.0,600.0,"JAWS",2);
    glColor3f(0,0,0);
    output(700.0,550.0,"1. New Game",1);
    output(700.0,500.0,"2. Instructions",1);
    output(700.0,450.0,"3. High Score",1);
    output(700.0,400.0,"4. About",1);
    output(700.0,350,"5. Exit",1);
    glColor3f(1,0,0);
    output(700,250,"< use keyboard to select options >",2);
}
void about()
{
    glColor3f(0,0,0);
    output(1200,100,"Press F1 to go back to menu",2);
    glClearColor(0,0.80784,0.8196,0);
    glColor3f(1,0.3,0.1);
    output(700,400,"Created by:",2);
    glColor3f(1.0,0.0,0.5);
    output(700,350,"Shanthi K (1PE14CS128)",1);
    output(700,300,"Sharath C Naik (1PE14CS129)",1);
}


void instr()
{
    glColor3f(0,0,0);
    output(1200,100,"Press F1 to go back to menu",2);
    glClearColor(0,0.80784,0.8196,0);
    glColor3f(1.0, 0.0, 0.5);
    output(100,640,"1. Use arrow keys to move player.",1);
    output(100,550,"2.Eat only smaller or equal sized fish.",2);
    output(100,350,"3.If you try to eat larger fish,game over.",2);
    output(100,200,"4.Your size will be increased for each 5 fish",2);

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

char temp[30],temp1[30],temp2[30],temp3[30],temp4[30],temp5[40];



int scores=0;

void gamescreen()
{
    int i;



    output(1200,100,"Press F1 to go back to menu",2);
    if(sizep>=1.3)
        {

            congo=true;

            gameover();
        }

    char score_Array[20]="Score:",time_array[20]="time:",highscore_array[30]="HighScore:";
    char charc_score[40],charc_time[40];
    for(i=0;i<20;i++)
    {
        if(!f1[i])
        {

            psy[i]=rand()%660+10;
            size1[i]=(rand()%10)/(float)10+0.4;
            color[i][0]=(rand()%255)/(float)255;
            color[i][1]=(rand()%255)/(float)255;
            color[i][2]=(rand()%255)/(float)255;
        }
    }
    for(i=0;i<20;i++)
    {
        if(!f2[i])
        {
            psy2[i]=rand()%660+10;
            size2[i]=(rand()%10)/(float)10+0.3;
            color1[i][0]=(rand()%255)/(float)255;
            color1[i][1]=(rand()%255)/(float)255;
            color1[i][2]=(rand()%255)/(float)255;

        }
    }



    //glClear(GL_COLOR_BUFFER_BIT);//CLEARS DISPLAY
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


    strcpy(temp,score_Array);
    sprintf(charc_score,"%d",score);
    strcpy(temp1,charc_score);
    glColor3f(1,0.1,0.6);
    output(400,700,temp,1);
    output(493,700,temp1,1);

//   itoa (score, buffer, 10);//itoa=function to convert int data type to string,format:char *  itoa ( int value, char * str, int base ); base=10 means decimal
  // drawTextNum(buffer, 6, 193,700);
   //sprintf(buffer,"%d",score);



    //int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);


    if(frame==FPS)
    {
        frame=0;
        t+=1;
    }
    strcpy(temp2,time_array);
    sprintf(charc_time,"%d",t);
    strcpy(temp3,charc_time);
    glColor3f(1,0.1,0.6);
    output(900,700,temp2,1);
    output(1093,700,temp3,1);

    if(game_over)
    {
        glColor3f(1,0.3,0.1);
        output(gameoverX,gameoverY,"Game Over!",1);
        output(420,455,"Score:",1);
        sprintf(charc_score,"%d",score);
        strcpy(temp1,charc_score);
        output(500,455,temp1,1);
        if(score>HighScore)
        {
            HighScore=score;
            highFlag=true;
        }
        strcpy(temp4,highscore_array);
        output(800,455,temp4,1);
        sprintf(temp5,"%d",HighScore);
        output(940,455,temp5,1);

    strcpy(temp2,time_array);
    sprintf(charc_time,"%d",t);
    strcpy(temp3,charc_time);
    output(1000,455,temp2,1);
    output(1093,455,temp3,1);

    }
     if(congo)
    {
        gameover();
        glColor3f(1,0.3,0.1);
        output(500,400,"Level Over! ",1);
        output(500,300,"Thank you for playing.",1);
        output(420,455,"Score:",1);
        sprintf(charc_score,"%d",score);
        strcpy(temp1,charc_score);
        output(500,455,temp1,1);
        if(score>HighScore)
        {
            HighScore=score;
        }
        strcpy(temp4,highscore_array);
        output(800,455,temp4,1);
        sprintf(temp5,"%d",HighScore);
        output(940,455,temp5,1);

    strcpy(temp2,time_array);
    sprintf(charc_time,"%d",t);
    strcpy(temp3,charc_time);
    glColor3f(1,0.3,0.1);
    output(1000,455,temp2,1);
    output(1093,455,temp3,1);
    }



    //glutSwapBuffers();//INSTRUCTS THE COMPUTER THAT YOU ARE DONE WITH THE CURRENT FRAME&BUFFERS SHOULD BE SWAPPED
    glFlush();
}




void display()
{
    int i;

    char score_Array[20]="Score:",time_array[20]="time:",highscore_array[30]="HighScore:";
    char charc_score[40],charc_time[40];

    if(gameFlag)
    {

        glClearColor(0.2,0.9,1,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        drawObject(0,0,0,1900,768);
        gamescreen();

        glFlush();
        glutSwapBuffers();

    }
    else if(menuFlag)
    {
        score=0;
        t=0;
        glClearColor(0.2,0.9,1,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        drawObject(0,0,0,1900,768);
        rect();
        glPopMatrix();
        highScoreWindow();
        if(highFlag)
            highScoreMenu();
        glutSwapBuffers();
        glFlush();
    }

    //If in Game mode Display game Screen


    else if(instruction_flag==true)
    {


        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        drawObject(3,550,600,200,100);
        drawObject(4,650,450,250,150);
        drawObject(5,650,290,250,150);
        glLoadIdentity();
        glPushMatrix();
        instr();
        glPopMatrix();
        glutSwapBuffers();
        glFlush();

    }
    else if(aboutflag==true)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        about();

        glPopMatrix();
        glutSwapBuffers();
        glFlush();

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
    load_textures();
    load_high();
    glutDisplayFunc(display);

    glutKeyboardFunc(keyPress);
    glutSpecialFunc(keyboard);
    glutTimerFunc(100,times,0);
    myinit();
    glutMainLoop();
    fclose(highScoreFile);
    return 0;
}

