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

#define pi (2 * acos(0.0))

int counter = 0;

struct Point
{
    double x, y, z;
    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Point()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Point(const Point &p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }
};

// global variables
Point cameraPos(30, 70, 30);
Point cameraLook(0, 0, 0);
Point cameraUp(0, 0, 1);
Point cameraRight(1, 0, 0);

Point sphereCenter(0, 5, 5);
Point sphereUp(0, 0, 1);
Point sphereFront(1, 0, 0);

double rotationAngle = 5.0;
double maxBorderlength=40;
double sphereRadious=5.0;
bool simulation = false;

void axes()
{
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(50, 0, 0);
        glVertex3f(-50, 0, 0);

        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f(50, 0, 0);
        glVertex3f(40, 0, 0);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0, 50, 0);
        glVertex3f(0, -50, 0);

        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(0, 50, 0);
        glVertex3f(0, 40, 0);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0, 0, 50);
        glVertex3f(0, 0, -50);

        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(0, 0, 50);
        glVertex3f(0, 0, 40);
    }
    glEnd();
}

void draw_plane()
{

    int perUnit = 10;
    int maxi = 10 * perUnit;

    // Draw a 2D grid with each square representing a one-unit area
    for (int i = -maxi; i < maxi; i += perUnit)
    {
        for (int j = -maxi; j < maxi; j += perUnit)
        {
            if ((i + j) % (2 * perUnit) == 0)
                glColor3f(1.0f, 1.0f, 1.0f); // White
            else
                glColor3f(0.5f, 0.5f, 0.5f); // Gray

            // Draw each square
            glBegin(GL_QUADS);
            {
                glVertex3f(i, j, 0);                     // Bottom-left corner
                glVertex3f(i + perUnit, j, 0);           // Bottom-right corner
                glVertex3f(i + perUnit, j + perUnit, 0); // Top-right corner
                glVertex3f(i, j + perUnit, 0);           // Top-left corner
            }
            glEnd();
        }
    }
}

void go_forward()
{
    

    if(sphereCenter.x + 5.0 > maxBorderlength){
        sphereFront.x = -sphereFront.x;
    }
    else if(sphereCenter.x - 5.0 < -maxBorderlength){
        sphereFront.x = -sphereFront.x;
    }
    if(sphereCenter.y + 5.0 > maxBorderlength){
        sphereFront.y = -sphereFront.y;
    }
    else if(sphereCenter.y - 5.0 < -maxBorderlength){
        sphereFront.y = -sphereFront.y;
    }
    sphereCenter.x += sphereFront.x;
    sphereCenter.y += sphereFront.y;
    sphereCenter.z += sphereFront.z;

    double length=sqrt(sphereFront.x*sphereFront.x+sphereFront.y*sphereFront.y+sphereFront.z*sphereFront.z);
    double theta=length/sphereRadious;
    theta=theta*180/pi;
    rotationAngle+=theta;


    
}

void go_backward()
{

    if(sphereCenter.x + 5.0 > maxBorderlength){
        sphereFront.x = -sphereFront.x;
    }
    else if(sphereCenter.x - 5.0 < -maxBorderlength){
        sphereFront.x = -sphereFront.x;
    }
    if(sphereCenter.y + 5.0 > maxBorderlength){
        sphereFront.y = -sphereFront.y;
    }
    else if(sphereCenter.y - 5.0 < -maxBorderlength){
        sphereFront.y = -sphereFront.y;
    }


    sphereCenter.x -= sphereFront.x;
    sphereCenter.y -= sphereFront.y;
    sphereCenter.z -= sphereFront.z;
    double length=sqrt(sphereFront.x*sphereFront.x+sphereFront.y*sphereFront.y+sphereFront.z*sphereFront.z);
    double theta=length/sphereRadious;
    theta=theta*180/pi;
    rotationAngle-=theta;
}

void clockwise_rotate(){
    double rotate = -0.2;
    double x, y, z;
    Point p1(0, 0, 0);
    // cross product of sphere up and sphere front
    p1.x = (sphereUp.y * sphereFront.z) - (sphereFront.y * sphereUp.z);
    p1.y = (sphereUp.z * sphereFront.x) - (sphereFront.z * sphereUp.x);
    p1.z = (sphereUp.x * sphereFront.y) - (sphereFront.x * sphereUp.y);

    x = sphereFront.x * cos(rotate) + p1.x * sin(rotate);
    y = sphereFront.y * cos(rotate) + p1.y * sin(rotate);   
    z = sphereFront.z * cos(rotate) + p1.z * sin(rotate);

    sphereFront.x = x;
    sphereFront.y = y;
    sphereFront.z = z;
}

void anticlockwise_rotate(){
    double rotate = 0.2;


    double x, y, z;
    Point p1(0, 0, 0);
    // cross product of sphere up and sphere front
    p1.x = (sphereUp.y * sphereFront.z) - (sphereFront.y * sphereUp.z);
    p1.y = (sphereUp.z * sphereFront.x) - (sphereFront.z * sphereUp.x);
    p1.z = (sphereUp.x * sphereFront.y) - (sphereFront.x * sphereUp.y);

    x = sphereFront.x * cos(rotate) + p1.x * sin(rotate);
    y = sphereFront.y * cos(rotate) + p1.y * sin(rotate);
    z = sphereFront.z * cos(rotate) + p1.z * sin(rotate);

    sphereFront.x = x;
    sphereFront.y = y;
    sphereFront.z = z;
}

void draw_border_oneSide(double borderLength)
{

    double borderHeight = 1.0;

    glBegin(GL_QUADS);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0, 0, 0);
        glVertex3f(borderLength, 0, 0);
        glVertex3f(borderLength, 0, borderHeight);
        glVertex3f(0, 0, borderHeight);
    }
    glEnd();
}
void draw_border()
{

    double borderLength = 2*maxBorderlength;

    for(int i=0; i<4; i++){
        glPushMatrix();
        {
            glRotatef(i*90, 0, 0, 1);
            // glRotatef(90, borderLength/2, 0, 1);
            glTranslatef(0, borderLength/2, 0);
            glTranslatef(-borderLength/2, 0, 0);
            draw_border_oneSide(borderLength);
        }
        glPopMatrix();
    }
}
void draw_sphere_only(double radious)
{

    int stack = 20;
    int sector = 20;
    double stack_angle;
    double sector_angle;

    Point mat[stack + 1][sector + 1];

    double x, y, z;
    for (int i = 0; i <= stack; i++)
    {
        for (int j = 0; j <= sector; j++)
        {

            stack_angle = (pi / 2) - (i * (pi / stack));
            sector_angle = j * (2 * pi / sector);
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

            glBegin(GL_QUADS);
            {
                glColor3f(i % 2, j % 2, (i + j) % 2);
                glVertex3f(mat[i][j].x, mat[i][j].y, mat[i][j].z);
                glVertex3f(mat[i][j + 1].x, mat[i][j + 1].y, mat[i][j + 1].z);
                glVertex3f(mat[i + 1][j + 1].x, mat[i + 1][j + 1].y, mat[i + 1][j + 1].z);
                glVertex3f(mat[i + 1][j].x, mat[i + 1][j].y, mat[i + 1][j].z);
            }
            glEnd();
        }
    }
}
void draw_sphere(double radious)
{

    Point p1(0, 0, 0);
    // cross product of sphere up and sphere front
    p1.x = (sphereUp.y * sphereFront.z) - (sphereFront.y * sphereUp.z);
    p1.y = (sphereUp.z * sphereFront.x) - (sphereFront.z * sphereUp.x);
    p1.z = (sphereUp.x * sphereFront.y) - (sphereFront.x * sphereUp.y);


    glPushMatrix();
    {
        glTranslatef(sphereCenter.x, sphereCenter.y, sphereCenter.z);
        glRotatef(rotationAngle, p1.x, p1.y, p1.z);
        draw_sphere_only(radious);
    }
    glPopMatrix();
}

void draw_cylinder()
{
    double radious = 0.5;
    double height = 15.0;
    int sector = 20;
    double sector_angle = 2 * pi / sector;

    for (int i = 0; i < sector; i++)
    {

        glBegin(GL_QUADS);
        {
            glVertex3f(0, 0, 0);
            glVertex3f(radious * cos(i * sector_angle), radious * sin(i * sector_angle), 0);
            glVertex3f(radious * cos((i + 1) * sector_angle), radious * sin((i + 1) * sector_angle), 0);
            glVertex3f(0, 0, 0);
        }
        glEnd();

        glBegin(GL_QUADS);
        {
            glVertex3f(0, 0, height);
            glVertex3f(radious * cos(i * sector_angle), radious * sin(i * sector_angle), height);
            glVertex3f(radious * cos((i + 1) * sector_angle), radious * sin((i + 1) * sector_angle), height);
            glVertex3f(0, 0, height);
        }
        glEnd();

        glBegin(GL_QUADS);
        {
            glVertex3f(radious * cos(i * sector_angle), radious * sin(i * sector_angle), 0);
            glVertex3f(radious * cos((i + 1) * sector_angle), radious * sin((i + 1) * sector_angle), 0);
            glVertex3f(radious * cos((i + 1) * sector_angle), radious * sin((i + 1) * sector_angle), height);
            glVertex3f(radious * cos(i * sector_angle), radious * sin(i * sector_angle), height);
        }
        glEnd();
    }
}

void draw_right_vector()
{

    Point p1(0, 0, 0);
    double rotate = 90.0;
    // cross product of sphere up and sphere front
    p1.x = (sphereUp.y * sphereFront.z) - (sphereFront.y * sphereUp.z);
    p1.y = (sphereUp.z * sphereFront.x) - (sphereFront.z * sphereUp.x);
    p1.z = (sphereUp.x * sphereFront.y) - (sphereFront.x * sphereUp.y);

    glPushMatrix();
    {
        glColor3f(1, 0, 0);
        glTranslatef(sphereCenter.x, sphereCenter.y, sphereCenter.z);
        glRotatef(rotate, p1.x, p1.y, p1.z);
        draw_cylinder();
    }
    glPopMatrix();
}

void draw_Up_vector()
{
    glPushMatrix();
    {
        glColor3f(0, 1, 0);
        glTranslatef(sphereCenter.x, sphereCenter.y, sphereCenter.z);
        draw_cylinder();
    }
    glPopMatrix();
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glClearDepth(1.0f);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glLoadIdentity();

    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
              cameraPos.x + cameraLook.x, cameraPos.y + cameraLook.y, cameraPos.z + cameraLook.z,
              cameraUp.x, cameraUp.y, cameraUp.z);

    axes();
    draw_plane();
    draw_border();
    draw_sphere(5.0);
    draw_right_vector();
    draw_Up_vector();

    glutSwapBuffers();
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(80, 1, 1, 1000);
    cameraLook.x = (cameraRight.y * cameraUp.z) - (cameraUp.y * cameraRight.z);
    cameraLook.y = (cameraRight.z * cameraUp.x) - (cameraUp.z * cameraRight.x);
    cameraLook.z = (cameraRight.x * cameraUp.y) - (cameraUp.x * cameraRight.y);
}

void idle()
{
    printf("Idle\n");
    glutPostRedisplay();
}
void Timer(int value)
{
    if(simulation){
        go_forward();
    }
    // printf("Timer\n");
    glutPostRedisplay();
    glutTimerFunc(10, Timer, 0);
}

void keyboardHandler(unsigned char key, int x, int y)
{
    double rotation_angle = .01;
    switch (key)
    {
    case '1':
        printf("1 pressed\n");
        cameraRight.x = (cameraRight.x * cos(rotation_angle)) + (cameraLook.x * sin(rotation_angle));
        cameraRight.y = (cameraRight.y * cos(rotation_angle)) + (cameraLook.y * sin(rotation_angle));
        cameraRight.z = (cameraRight.z * cos(rotation_angle)) + (cameraLook.z * sin(rotation_angle));

        cameraLook.x = (cameraLook.x * cos(rotation_angle)) - (cameraRight.x * sin(rotation_angle));
        cameraLook.y = (cameraLook.y * cos(rotation_angle)) - (cameraRight.y * sin(rotation_angle));
        cameraLook.z = (cameraLook.z * cos(rotation_angle)) - (cameraRight.z * sin(rotation_angle));
        break;
    case '2':
        printf("2 pressed\n");
        cameraRight.x = (cameraRight.x * cos(-rotation_angle)) + (cameraLook.x * sin(-rotation_angle));
        cameraRight.y = (cameraRight.y * cos(-rotation_angle)) + (cameraLook.y * sin(-rotation_angle));
        cameraRight.z = (cameraRight.z * cos(-rotation_angle)) + (cameraLook.z * sin(-rotation_angle));

        cameraLook.x = (cameraLook.x * cos(-rotation_angle)) - (cameraRight.x * sin(-rotation_angle));
        cameraLook.y = (cameraLook.y * cos(-rotation_angle)) - (cameraRight.y * sin(-rotation_angle));
        cameraLook.z = (cameraLook.z * cos(-rotation_angle)) - (cameraRight.z * sin(-rotation_angle));
        break;
    case '3':
        printf("3 pressed\n");
        cameraLook.x = (cameraLook.x * cos(rotation_angle)) + (cameraUp.x * sin(rotation_angle));
        cameraLook.y = (cameraLook.y * cos(rotation_angle)) + (cameraUp.y * sin(rotation_angle));
        cameraLook.z = (cameraLook.z * cos(rotation_angle)) + (cameraUp.z * sin(rotation_angle));

        cameraUp.x = (cameraUp.x * cos(rotation_angle)) - (cameraLook.x * sin(rotation_angle));
        cameraUp.y = (cameraUp.y * cos(rotation_angle)) - (cameraLook.y * sin(rotation_angle));
        cameraUp.z = (cameraUp.z * cos(rotation_angle)) - (cameraLook.z * sin(rotation_angle));
        break;
    case '4':
        printf("4 pressed\n");
        cameraLook.x = (cameraLook.x * cos(-rotation_angle)) + (cameraUp.x * sin(-rotation_angle));
        cameraLook.y = (cameraLook.y * cos(-rotation_angle)) + (cameraUp.y * sin(-rotation_angle));
        cameraLook.z = (cameraLook.z * cos(-rotation_angle)) + (cameraUp.z * sin(-rotation_angle));

        cameraUp.x = (cameraUp.x * cos(-rotation_angle)) - (cameraLook.x * sin(-rotation_angle));
        cameraUp.y = (cameraUp.y * cos(-rotation_angle)) - (cameraLook.y * sin(-rotation_angle));
        cameraUp.z = (cameraUp.z * cos(-rotation_angle)) - (cameraLook.z * sin(-rotation_angle));
        break;
    case '5':
        printf("5 pressed\n");
        cameraUp.x = (cameraUp.x * cos(rotation_angle)) + (cameraRight.x * sin(rotation_angle));
        cameraUp.y = (cameraUp.y * cos(rotation_angle)) + (cameraRight.y * sin(rotation_angle));
        cameraUp.z = (cameraUp.z * cos(rotation_angle)) + (cameraRight.z * sin(rotation_angle));

        cameraRight.x = (cameraRight.x * cos(rotation_angle)) - (cameraUp.x * sin(rotation_angle));
        cameraRight.y = (cameraRight.y * cos(rotation_angle)) - (cameraUp.y * sin(rotation_angle));
        cameraRight.z = (cameraRight.z * cos(rotation_angle)) - (cameraUp.z * sin(rotation_angle));
        break;
    case '6':
        printf("6 pressed\n");
        cameraUp.x = (cameraUp.x * cos(-rotation_angle)) + (cameraRight.x * sin(-rotation_angle));
        cameraUp.y = (cameraUp.y * cos(-rotation_angle)) + (cameraRight.y * sin(-rotation_angle));
        cameraUp.z = (cameraUp.z * cos(-rotation_angle)) + (cameraRight.z * sin(-rotation_angle));

        cameraRight.x = (cameraRight.x * cos(-rotation_angle)) - (cameraUp.x * sin(-rotation_angle));
        cameraRight.y = (cameraRight.y * cos(-rotation_angle)) - (cameraUp.y * sin(-rotation_angle));
        cameraRight.z = (cameraRight.z * cos(-rotation_angle)) - (cameraUp.z * sin(-rotation_angle));
        break;

    case 'j':
        printf("j pressed\n");
        clockwise_rotate();
        break;
    case 'l':
        printf("l pressed\n");
        anticlockwise_rotate();
        break;
    case 'i':
        printf("i pressed\n");
        go_forward();
        break;
    case 'k':
        printf("k pressed\n");
        go_backward();
        break;

    case ' ':
        printf("space pressed\n");
        simulation = !simulation;
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

void specialKeyHandler(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        printf("UP pressed");
        cameraPos.x += cameraLook.x;
        cameraPos.y += cameraLook.y;
        cameraPos.z += cameraLook.z;
        break;
    case GLUT_KEY_DOWN:
        printf("DOWN pressed");
        cameraPos.x -= cameraLook.x;
        cameraPos.y -= cameraLook.y;
        cameraPos.z -= cameraLook.z;
        break;
    case GLUT_KEY_LEFT:
        printf("LEFT pressed");
        cameraPos.x -= cameraRight.x;
        cameraPos.y -= cameraRight.y;
        cameraPos.z -= cameraRight.z;
        break;
    case GLUT_KEY_RIGHT:
        printf("RIGHT pressed");
        cameraPos.y += cameraRight.y;
        cameraPos.x += cameraRight.x;
        cameraPos.z += cameraRight.z;
        break;
    case GLUT_KEY_PAGE_DOWN:
        printf("PAGE_DOWN pressed");
        cameraPos.x -= cameraUp.x;
        cameraPos.y -= cameraUp.y;
        cameraPos.z -= cameraUp.z;
        break;
    case GLUT_KEY_PAGE_UP:
        printf("PAGE_UP pressed");
        cameraPos.x += cameraUp.x;
        cameraPos.y += cameraUp.y;
        cameraPos.z += cameraUp.z;
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