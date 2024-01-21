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

int counter = 0;
void axes()
{
    glBegin(GL_LINES);
    {
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(-1, 0);
        glVertex2f(1, 0);

        glColor3f(0, 0, 1);
        glVertex2f(0, -1);
        glVertex2f(0, 1);
    }
    glEnd();
}

void square(double a){
    glBegin(GL_QUADS);
    {
        glVertex2f(0,0);
        glVertex2f(0, a);
        glVertex2f(a, a);
        glVertex2f(a, 0);
    }
    glEnd();

}

void display()
{
    printf("Display %d\n", counter++);
    glClear(GL_COLOR_BUFFER_BIT);

    axes();

    double x[] = {.5,.5,-.5,-.5};
    double y[] = {.5,-.5,-.5,.5};

    // for (int i = 0; i < 4; i++)
    // {
    //     glPushMatrix();
    //     glTranslatef(x[i], y[i], 0);
    //     glRotatef(45, 0, 0, 1);
    //     square(.1);
    //     glPopMatrix();
    //     // glTranslatef(-x[i], -y[i], 0);

    // }


    // for (int i = 0; i < 4; i++)
    // {
        glPushMatrix();
        glRotatef(counter%360, 0, 0, 1);
        glTranslatef(.5, 0.5, 0);
        square(.1);
        glPopMatrix();
        // glTranslatef(-x[i], -y[i], 0);

    // }

    glFlush();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void idle()
{
    printf("Idle\n");
    glutPostRedisplay();
}
void Timer(int value)
{
    printf("Timer\n");
    glutPostRedisplay();
    glutTimerFunc(1000, Timer, 0);
}
int main(int argc, char **argv)
{
    printf("Hello World\n");
    glutInit(&argc, argv);

    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGl Test");

    init();

    glutDisplayFunc(display);

    // glutIdleFunc(idle);
    glutTimerFunc(1000, Timer, 0);

    glutMainLoop();
    return 0;
}