#include <bits/stdc++.h>

#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

using namespace std;

double radius = 50;

struct Point
{
    double x, y;
};

Point trace, trace2;

double rotation = 0;
double rotation2 = 0;

void init()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black

    trace.x = radius;
    trace.y = 0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 1, 1000.0);
}

void drawAxes()
{

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    {
        glVertex3f(100, 0, 0);
        glVertex3f(-100, 0, 0);

        glVertex3f(0, -100, 0);
        glVertex3f(0, 100, 0);

        glVertex3f(0, 0, 100);
        glVertex3f(0, 0, -100);
    }
    glEnd();
}

void keyboardListener(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'K':
        /* code */
        printf("K pressed\n");
        break;

    case '1':
        rotation += 5;
        break;

    case '2':
        rotation -= 5;
        break;

    default:
        printf("We don't know what you pressed\n");
        break;
    }
}
void drawSquare(double a)
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

void drawLines()
{
    glBegin(GL_LINES);
    {
        glVertex3f(trace.x, trace.y, 0);
        glVertex3f(trace.x + 5, trace.y, -40);
    }
    glEnd();

    glBegin(GL_LINES);
    {
        glVertex3f(trace.x, trace.y, 0);
        glVertex3f(trace.x - 5, trace.y, -40);
    }
    glEnd();
}

void drawCircle()
{

    double theta = 0;
    double prev_x = radius;
    double prev_y = 0;

    while (theta <= 2 * 3.1416)
    {
        double x = radius * cos(theta);
        double y = radius * sin(theta);

        glBegin(GL_LINES);
        {
            glColor3f(1, 1, 1);
            glVertex3f(prev_x, prev_y, 0);
            glVertex3f(x, y, 0);
        }
        glEnd();

        prev_x = x;
        prev_y = y;
        theta += 0.01;
    }
}

void drawPlate()
{

    glPushMatrix();
    // glRotatef(i * 90, 1, 0, 0);
    // glTranslatef(trace.x, trace.y, -40);
    // glRotatef(-rotation, 0, 1, 0);
    glTranslatef(trace.x, trace.y, -40);
    glColor3f(1, 1, 1);
    // glColor3f(_rand(), _rand(), _rand()); // Red

    drawSquare(10);

    glPopMatrix();

    drawLines();
}

void drawPlates()
{
    for (int i = 0; i < 8; i++)
    {
        glPushMatrix();
        glRotatef(i * 60, 0, 0, 1);
        glRotatef(-rotation, 0, 1, 0);
        drawPlate();
        glPopMatrix();
    }
}


void display()
{
    // printf("Display function called for %d times\n", counter);

    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(100, 100, 100, 0, 0, 0, 0, 0, 1);
    drawAxes();

    // rotation += 0.0005;
    // rotation2 += 0.003;

    // if(rotation > 2*3.1416)
    //     rotation = 0;

    // if (rotation2 > 2 * 3.1416)
    //     rotation2 = 0;

    // glBegin(GL_LINES);{
    //     glColor3f(1.0f, 1.0f, 1.0f); // Green
    //     glVertex2f(-1.0f, 0.0f);
    //     glVertex2f(1.0f, 0.0f);
    //     glVertex2f(0.0f, -1.0f);
    //     glVertex2f(0.0f, 1.0f);

    // }glEnd();

    // for(int i = 0; i < 8; i++){
    //     glPushMatrix();
    //     glRotatef(i * 90, 1, 0, 0);
    //     glTranslatef(0, 0, 20);
    //     glColor3f(1, 0, 0);
    //     // glColor3f(_rand(), _rand(), _rand()); // Red
    //     drawSquare(20);
    //     glPopMatrix();
    // }

    glPushMatrix();
    drawPlates();
    glPopMatrix();
    drawCircle();

    trace.x = radius * cos(rotation2);
    trace.y = radius * sin(rotation2);

    // glFlush();
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(750, 750);
    glutInitWindowPosition(50, 50);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("1905056_online");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);

    init();

    glutMainLoop();
    return 0;
}