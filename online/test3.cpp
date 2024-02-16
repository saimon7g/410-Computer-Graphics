#include <bits/stdc++.h>
#include <GL/glut.h>

#define pi (2 * acos(0.0))

using namespace std;

// Global variables
float sunRadius = 2.0;
float mercuryRadius = 0.5;
float venusRadius = 0.7;
float earthRadius = 0.8;
float moonRadius = 0.2;

float sunDistance = 0.0;
float mercuryDistance = 8.0;
float venusDistance = 15.0;
float earthDistance = 22.0;
float moonDistance = 3.0;

float sunAngle = 0.0;
float mercuryAngle = 0.0;
float venusAngle = 0.0;
float earthAngle = 0.0;
float moonAngle = 0.0;

void drawSphere(float radius, float distance, float angle, GLfloat r, GLfloat g, GLfloat b)
{
    glPushMatrix();
    {
        glRotatef(angle, 0, 0, 1);
        glTranslatef(distance, 0, 0);
        glColor3f(r, g, b);
        glutSolidSphere(radius, 100, 100);
    }
    glPopMatrix();
}

void drawOrbit(float distance)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++)
    {
        float angle = i * pi / 180;
        glVertex3f(distance * cos(angle), distance * sin(angle), 0);
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Draw orbits
    glColor3f(0.5, 0.5, 0.5);
    drawOrbit(mercuryDistance);
    drawOrbit(venusDistance);
    drawOrbit(earthDistance);

    // Draw Sun
    drawSphere(sunRadius, sunDistance, sunAngle, 1.0, 1.0, 0.0);

    // Draw planets
    drawSphere(mercuryRadius, mercuryDistance, mercuryAngle, 0.7, 0.2, 0.0);
    drawSphere(venusRadius, venusDistance, venusAngle, 1.0, 0.5, 0.0);
    drawSphere(earthRadius, earthDistance, earthAngle, 0.0, 0.0, 1.0);

    // Draw Moon
    glPushMatrix();
    {
        glRotatef(earthAngle, 0, 0, 1);
        glTranslatef(moonDistance, 0, 0);
        glRotatef(moonAngle, 0, 0, 1);
        glColor3f(0.7, 0.7, 0.7);
        glutSolidSphere(moonRadius, 100, 100);
    }
    glPopMatrix();

    glutSwapBuffers();
}

void update(int value)
{
    sunAngle += 0.1;
    mercuryAngle += 0.5;
    venusAngle += 0.4;
    earthAngle += 0.3;
    moonAngle += 1.0;

    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.0, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Solar System");

    init();

    glutDisplayFunc(display);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}
