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

double maxTriangleSide = 20;
double currentTriangleSide = 20;

double maxSphereRadius = maxTriangleSide / sqrt(3.0);
double currentSphereRadius = 0;

int numfSteps = 20;
double rotationAngle = 0.0;

void axes()
{
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(50, 0, 0);
        glVertex3f(-30, 0, 0);

        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f(50, 0, 0);
        glVertex3f(40, 0, 0);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0, 50, 0);
        glVertex3f(0, -30, 0);

        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(0, 50, 0);
        glVertex3f(0, 40, 0);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0, 0, 50);
        glVertex3f(0, 0, -30);

        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(0, 0, 50);
        glVertex3f(0, 0, 40);
    }
    glEnd();
}

void circle(double a)
{
    // draw circle
    double theta = 0;

    double prevx = 0;
    double prevy = 0;
    while (theta <= 360)
    {
        double thetainrad = theta * 3.1416 / 180;
        double x = a*cos(thetainrad);
        double y = a*sin(thetainrad);
        glBegin(GL_LINES);
        {
            glColor3f(1, 0, 0);
            glVertex3f(prevx, prevy, 0);
            glVertex3f(x, y, 0);
        }
        glEnd();
        prevx = x;
        prevy = y;
        theta += 5;
    }
}


void drawSquare()
{
    glColor3f(1.0,1.0,1.0);

    glBegin(GL_QUADS);
    {

        glVertex3f(5, 5, 0);
        glVertex3f(5, -5, 0);
        glVertex3f(-5, -5, 0);
        glVertex3f(5, -5, 0);
    }
    glEnd();
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
    circle(10.0);
    glPushMatrix();
    {

        glTranslatef(5,5,0);

        glBegin(GL_LINE);
        {

            glVertex3f(0, 0, 0);
            glVertex3f(-5, -5, -5);
        }
        glEnd();

        glBegin(GL_LINE);
        {

            glVertex3f(0, 0, 0);
            glVertex3f(5, 5, -5);
        }
        glEnd();
        glTranslatef(0, 0, -5);
        drawSquare();
    }
    glPopMatrix();

    // glPushMatrix();
    // {
    //     glRotatef(rotationAngle, 0, 0, 1);
    //     draw_octahedron();
    //     draw_sphere();
    //     draw_cylinder();
    // }
    // glPopMatrix();

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
    // printf("Idle\n");
    glutPostRedisplay();
}
void Timer(int value)
{
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

    case ',':
        printf(", pressed\n");
        currentTriangleSide -= 1.0;
        if (currentTriangleSide < 0)
        {
            currentTriangleSide = 0;
        }
        currentSphereRadius += maxSphereRadius / numfSteps;
        if (currentSphereRadius > maxSphereRadius)
        {
            currentSphereRadius = maxSphereRadius;
        }

        break;
    case '.':
        printf(". pressed\n");
        currentTriangleSide += 1;
        if (currentTriangleSide > maxTriangleSide)
        {
            currentTriangleSide = maxTriangleSide;
        }
        currentSphereRadius -= maxSphereRadius / numfSteps;
        if (currentSphereRadius < 0)
        {
            currentSphereRadius = 0;
        }
        break;

    case 'a':
        printf("a pressed\n");
        rotationAngle += 5.0;
        if (rotationAngle > 360)
        {
            rotationAngle = rotationAngle - 360;
        }
        break;

    case 'd':
        printf("d pressed\n");
        rotationAngle -= 5.0;
        if (rotationAngle < 0)
        {
            rotationAngle = 360 + rotationAngle;
        }
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
    // glutTimerFunc(10, Timer, 0);

    glutMainLoop();
    return 0;
}