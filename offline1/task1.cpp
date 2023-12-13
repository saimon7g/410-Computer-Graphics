#include <bits/stdc++.h>
using namespace std;

#ifdef __linux__
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#elif __WIN32__
#include <windows.h>
#include <glut.h>
#endif

#define pi (2*acos(0.0))

int counter = 0;

struct Point{
    double x,y,z;
    Point(double x,double y,double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Point(){
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Point(const Point &p){
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }


};



//global variables

Point cameraPos(50,20,20);
Point cameraLook(0,1,0);
Point cameraUp(0,0,1);
Point cameraRight(-1,0,0);




void axes()
{
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f );
        glVertex3f(100,0,0);
        glVertex3f(-100,0,0);

        glColor3f(0.0f, 1.0f, 1.0f );
        glVertex3f(100,0,0);
        glVertex3f(90,0,0);

        glColor3f(0.0f, 1.0f, 0.0f );
        glVertex3f(0,100,0);
        glVertex3f(0,-100,0);

        glColor3f(1.0f, 0.0f, 1.0f );
        glVertex3f(0,100,0);
        glVertex3f(0,90,0);

        glColor3f(0.0f, 0.0f, 1.0f );
        glVertex3f(0,0,100);
        glVertex3f(0,0,-100);

        glColor3f(1.0f, 1.0f, 0.0f );
        glVertex3f(0,0,100);
        glVertex3f(0,0,90);

    }glEnd();
}

void square(double a)
{
    glBegin(GL_QUADS);
    {
        glVertex3f(a, a, 0);
        glVertex3f(a, -a, 0);
        glVertex3f(-a, -a, 0);
        glVertex3f(-a, a, 0);
        
    }
    glEnd();
}

void draw_cylinder(double radious, double height){




    double slices = 24;
    double stacks = 20;
    double slice_angle = (2*pi)/slices;
    double stack_height = height/stacks;
    double stack_radius = radious/stacks;

    double x,y,z;

    for(int i=0;i<slices;i++){
        for(int j=0;j<stacks;j++){
            x = radious * cos(i*slice_angle);
            y = radious * sin(i*slice_angle);
            z = j*stack_height;

            Point p1(x,y,z);

            x = radious * cos((i+1)*slice_angle);
            y = radious * sin((i+1)*slice_angle);
            z = j*stack_height;

            Point p2(x,y,z);

            x = radious * cos((i+1)*slice_angle);
            y = radious * sin((i+1)*slice_angle);
            z = (j+1)*stack_height;

            Point p3(x,y,z);

            x = radious * cos(i*slice_angle);
            y = radious * sin(i*slice_angle);
            z = (j+1)*stack_height;

            Point p4(x,y,z);

            glBegin(GL_QUADS);
            {
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
                glVertex3f(p3.x,p3.y,p3.z);
                glVertex3f(p4.x,p4.y,p4.z);
            }
            glEnd();
        }

}
}

void draw_sphere(double radious){

    int stack=20;
    int sector=20;
    double stack_angle;
    double sector_angle;

    Point mat[stack+1][sector+1];

    double x,y,z;
    for (int i = 0; i <= stack; i++)
    {
        for (int j = 0; j <= sector; j++)
        {

            stack_angle = (pi/2) - (i * (pi/stack));
            sector_angle = j * (2*pi/sector);
            x = radious * cos(stack_angle) * cos(sector_angle);
            y = radious * cos(stack_angle) * sin(sector_angle);
            z = radious * sin(stack_angle);

            mat[i][j].x = x;
            mat[i][j].y = y;
            mat[i][j].z = z;
        }
    }

    for (int i = 0; i < stack; i++)
    {
        for (int j = 0; j < sector; j++)
        {

            glColor3f(i%2,j%2,(i+j)%2);
            glBegin(GL_QUADS);
            {
                glVertex3f(mat[i][j].x,mat[i][j].y,mat[i][j].z);
                glVertex3f(mat[i][j+1].x,mat[i][j+1].y,mat[i][j+1].z);
                glVertex3f(mat[i+1][j+1].x,mat[i+1][j+1].y,mat[i+1][j+1].z);
                glVertex3f(mat[i+1][j].x,mat[i+1][j].y,mat[i+1][j].z);
            }
            glEnd();
        }

}
}
void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cameraPos.x,cameraPos.y,cameraPos.z,
              cameraLook.x,cameraLook.y,cameraLook.z,
              cameraUp.x,cameraUp.y,cameraUp.z);

    axes();
    // draw_cylinder(5,10);
    draw_sphere(15);







    glutSwapBuffers();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(60, 1, 0, 100);
}

void idle()
{
    printf("Idle\n");
    glutPostRedisplay();
}
void Timer(int value)
{
    // printf("Timer\n");
    glutPostRedisplay();
    glutTimerFunc(10, Timer, 0);
}




void keyboardHandler(unsigned char key,int x,int y){
    double rotation_angle = 5.0;
    switch (key)
    {
    case '1':
    printf("1 pressed\n");
        cameraRight.x= (cameraRight.x * cos(rotation_angle) ) + (cameraLook.x * sin(rotation_angle));
        cameraRight.y= (cameraRight.y * cos(rotation_angle) ) + (cameraLook.y * sin(rotation_angle));
        cameraRight.z= (cameraRight.z * cos(rotation_angle) ) + (cameraLook.z * sin(rotation_angle));

        cameraLook.x= (cameraLook.x * cos(rotation_angle) ) - (cameraRight.x * sin(rotation_angle));
        cameraLook.y= (cameraLook.y * cos(rotation_angle) ) - (cameraRight.y * sin(rotation_angle));
        cameraLook.z= (cameraLook.z * cos(rotation_angle) ) - (cameraRight.z * sin(rotation_angle));     
        break;
    case '2':
        printf("2 pressed\n");
        cameraRight.x= (cameraRight.x * cos(-rotation_angle) ) + (cameraLook.x * sin(-rotation_angle));
        cameraRight.y= (cameraRight.y * cos(-rotation_angle) ) + (cameraLook.y * sin(-rotation_angle));
        cameraRight.z= (cameraRight.z * cos(-rotation_angle) ) + (cameraLook.z * sin(-rotation_angle));

        cameraLook.x= (cameraLook.x * cos(-rotation_angle) ) - (cameraRight.x * sin(-rotation_angle));
        cameraLook.y= (cameraLook.y * cos(-rotation_angle) ) - (cameraRight.y * sin(-rotation_angle));
        cameraLook.z= (cameraLook.z * cos(-rotation_angle) ) - (cameraRight.z * sin(-rotation_angle));
        break;
    case '3':
        printf("3 pressed\n");
        cameraLook.x= (cameraLook.x * cos(rotation_angle) ) + (cameraUp.x * sin(rotation_angle));
        cameraLook.y= (cameraLook.y * cos(rotation_angle) ) + (cameraUp.y * sin(rotation_angle));
        cameraLook.z= (cameraLook.z * cos(rotation_angle) ) + (cameraUp.z * sin(rotation_angle));

        cameraUp.x= (cameraUp.x * cos(rotation_angle) ) - (cameraLook.x * sin(rotation_angle));
        cameraUp.y= (cameraUp.y * cos(rotation_angle) ) - (cameraLook.y * sin(rotation_angle));
        cameraUp.z= (cameraUp.z * cos(rotation_angle) ) - (cameraLook.z * sin(rotation_angle));
        break;
    case '4':
        printf("4 pressed\n");
        cameraLook.x= (cameraLook.x * cos(-rotation_angle) ) + (cameraUp.x * sin(-rotation_angle));
        cameraLook.y= (cameraLook.y * cos(-rotation_angle) ) + (cameraUp.y * sin(-rotation_angle));
        cameraLook.z= (cameraLook.z * cos(-rotation_angle) ) + (cameraUp.z * sin(-rotation_angle));

        cameraUp.x= (cameraUp.x * cos(-rotation_angle) ) - (cameraLook.x * sin(-rotation_angle));
        cameraUp.y= (cameraUp.y * cos(-rotation_angle) ) - (cameraLook.y * sin(-rotation_angle));
        cameraUp.z= (cameraUp.z * cos(-rotation_angle) ) - (cameraLook.z * sin(-rotation_angle));
        break;
    case '5':
        printf("5 pressed\n");
        cameraUp.x= (cameraUp.x * cos(rotation_angle) ) + (cameraRight.x * sin(rotation_angle));
        cameraUp.y= (cameraUp.y * cos(rotation_angle) ) + (cameraRight.y * sin(rotation_angle));
        cameraUp.z= (cameraUp.z * cos(rotation_angle) ) + (cameraRight.z * sin(rotation_angle));

        cameraRight.x= (cameraRight.x * cos(rotation_angle) ) - (cameraUp.x * sin(rotation_angle));
        cameraRight.y= (cameraRight.y * cos(rotation_angle) ) - (cameraUp.y * sin(rotation_angle));
        cameraRight.z= (cameraRight.z * cos(rotation_angle) ) - (cameraUp.z * sin(rotation_angle));
        break;
    case '6':
        printf("6 pressed\n");
        cameraUp.x= (cameraUp.x * cos(-rotation_angle) ) + (cameraRight.x * sin(-rotation_angle));
        cameraUp.y= (cameraUp.y * cos(-rotation_angle) ) + (cameraRight.y * sin(-rotation_angle));
        cameraUp.z= (cameraUp.z * cos(-rotation_angle) ) + (cameraRight.z * sin(-rotation_angle));

        cameraRight.x= (cameraRight.x * cos(-rotation_angle) ) - (cameraUp.x * sin(-rotation_angle));
        cameraRight.y= (cameraRight.y * cos(-rotation_angle) ) - (cameraUp.y * sin(-rotation_angle));
        cameraRight.z= (cameraRight.z * cos(-rotation_angle) ) - (cameraUp.z * sin(-rotation_angle));
        break;
    case 'q':
        printf("q pressed\n");
        exit(0);
        break;
    
    default:
        break;
    }
    glutPostRedisplay();
}

void specialKeyHandler(int key,int x,int y){
    switch (key)
    {
    case GLUT_KEY_UP:
    printf("UP pressed");
        cameraPos.x-=cameraLook.x;
        cameraPos.y-=cameraLook.y;
        cameraPos.z-=cameraLook.z;
        break;
    case GLUT_KEY_DOWN:
        printf("DOWN pressed");
        cameraPos.x+=cameraLook.x;
        cameraPos.y+=cameraLook.y;
        cameraPos.z+=cameraLook.z;
        break;
    case GLUT_KEY_LEFT:
        printf("LEFT pressed");
        cameraPos.x-=cameraRight.x;
        cameraPos.y-=cameraRight.y;
        cameraPos.z-=cameraRight.z;
        break;
    case GLUT_KEY_RIGHT:
        printf("RIGHT pressed");
        cameraPos.y+=cameraRight.y;
        cameraPos.x+=cameraRight.x;
        cameraPos.z+=cameraRight.z;
        break;
    case GLUT_KEY_PAGE_DOWN:
        printf("PAGE_DOWN pressed");
        cameraPos.x-=cameraUp.x;
        cameraPos.y-=cameraUp.y;
        cameraPos.z-=cameraUp.z;
        break;
    case GLUT_KEY_PAGE_UP:
        printf("PAGE_UP pressed");
        cameraPos.x+=cameraUp.x;
        cameraPos.y+=cameraUp.y;
        cameraPos.z+=cameraUp.z;
        break;
    
    default:
        break;
    }
    glutPostRedisplay();
}





int main(int argc, char **argv)
{
    printf("Hello World\n");
    glutInit(&argc, argv);

    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(500, 5000);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("OpenGl Test");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardHandler);
    glutSpecialFunc(specialKeyHandler);


    // glutIdleFunc(idle);
    glutTimerFunc(10, Timer, 0);




    glutMainLoop();
    return 0;
}