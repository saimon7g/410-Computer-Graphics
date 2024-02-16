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

struct Point
{
    double x, y;
};
double r = 0.5;
double rotation = 0;
double rotation2 = 0;
Point trace;
Point trace2;
double minir = 0.2;
double minir2 = 0.05;
void axes()
{
    glBegin(GL_LINES);
    {
        glColor3f(1, 1, 1);

        // x axis

        glVertex2f(-1, 0);
        glVertex2f(1, 0);

        // y axis

        glVertex2f(0, -1);
        glVertex2f(0, 1);
    }
    glEnd();
}

void MiniCircle()
{
    double theta = 0;

    double prevx = minir - 0.3 + trace.x;
    double prevy = 0 + trace.y;
    while (theta <= 360)
    {
        double thetainrad = theta * 3.1416 / 180;
        double x = minir * cos(thetainrad) - 0.3 + trace.x;
        double y = minir * sin(thetainrad) + trace.y;
        glBegin(GL_LINES);
        {
            glColor3f(1, 0, 0);
            glVertex2f(prevx, prevy);
            glVertex2f(x, y);
        }
        glEnd();
        prevx = x;
        prevy = y;
        theta += 5;
    }
}

void MiniCircle2()
{
    double theta = 0;

    double prevx = minir2 - 0.3 + trace2.x;
    double prevy = 0 + trace2.y;
    while (theta <= 360)
    {
        double thetainrad = theta * 3.1416 / 180;
        double x = minir2 * cos(thetainrad) - 0.3 + trace2.x;
        double y = minir2 * sin(thetainrad) + trace2.y;
        glBegin(GL_LINES);
        {
            glColor3f(1, 1, 1);
            glVertex2f(prevx, prevy);
            glVertex2f(x, y);
        }
        glEnd();
        prevx = x;
        prevy = y;
        theta += 5;
    }
}

void DrawCircle()
{
    double theta = 0;
    double prevx = r - 0.3;
    double prevy = 0;
    while (theta <= 360)
    {
        double thetainrad = theta * 3.1416 / 180;
        double x = r * cos(thetainrad) - 0.3;
        double y = r * sin(thetainrad);
        glBegin(GL_LINES);
        {
            glColor3f(0, 0, 1);
            glVertex2f(prevx, prevy);
            glVertex2f(x, y);
        }
        glEnd();
        prevx = x;
        prevy = y;
        theta += 5;
    }
}
void Drawline()
{

    glBegin(GL_LINES);
    {
        glColor3f(0, 0, 1);
        glVertex2f(-0.3, 0);
        glVertex2f(trace.x - 0.3, trace.y);
    }
    glEnd();

    glBegin(GL_LINES);
    {
        glColor3f(0, 1, 0);
        glVertex2f(trace.x - 0.3, trace.y);
        glVertex2f(trace2.x - 0.3, trace2.y);
    }
    glEnd();
}
void display()
{

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // axes();

    // glTranslatef(-0.3,0,0);
    DrawCircle();
    Drawline();
    // DrawSineWave();
    MiniCircle();
    MiniCircle2();
    glFlush();
}

void init()
{

    glClearColor(0, 0, 0, 1);
    trace.x = r;
    trace.y = 0;
    trace2.x = minir + trace.x;
    trace2.y = minir + trace.y;
}

void Timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(80, Timer, 0);
    rotation += 1;
    rotation2 += 0.5;
    if (rotation >= 360)
    {
        rotation = 0;
    }

    if (rotation2 >= 360)
        rotation2 = 0;
    double x = r * cos(rotation * 3.1416 / 180);
    double y = r * sin(rotation * 3.1416 / 180);
    double xx = minir * cos(rotation2) + trace.x;
    double yy = minir * sin(rotation2) + trace.y;
    trace.x = x;
    trace.y = y;
    trace2.x = xx;
    trace2.y = yy;
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitWindowPosition(500, 100);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("1905074");
    init();

    glutDisplayFunc(display);

    glutTimerFunc(0, Timer, 0);
    glutMainLoop();

    return 0;
}
