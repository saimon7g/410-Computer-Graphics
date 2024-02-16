#include <bits/stdc++.h>
#include <GL/glut.h>

#define pi (2 * acos(0.0))

using namespace std;

double inclinationAngle = 30; // Global variable for inclination angle

struct Point
{
    double x, y, z;
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
    Point() : x(0), y(0), z(0) {}
};

// Global variables
Point cameraPos(30, 70, 30);
Point cameraLook(0, 0, 0);
Point cameraUp(0, 0, 1);
Point cameraRight(1, 0, 0);

// Function to draw inclined surface
void drawInclinedSurface(double inclinationAngle)
{
    glBegin(GL_QUADS);
    {
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-50, 0, 0);
        glVertex3f(50, 0, 0);
        glVertex3f(50, 50 * tan(inclinationAngle * pi / 180), 0);
        glVertex3f(-50, 50 * tan(inclinationAngle * pi / 180), 0);
    }
    glEnd();
}

// Function to draw directional vector of the wheel
void drawWheelDirectionalVector()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    {
        glVertex3f(0, 0, 0);
        glVertex3f(10, 0, 0); // Extend the line by 10 units in x-direction from the center of the wheel
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
              cameraPos.x + cameraLook.x, cameraPos.y + cameraLook.y, cameraPos.z + cameraLook.z,
              cameraUp.x, cameraUp.y, cameraUp.z);

    // Draw the inclined surface
    drawInclinedSurface(inclinationAngle);

    // Draw the directional vector of the wheel
    drawWheelDirectionalVector();

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

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(500, 500);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("OpenGL Test");

    init();

    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
