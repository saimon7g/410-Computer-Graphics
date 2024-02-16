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

vector<Point> cylinder_Points(double radious)
{
    vector<Point> points;

    double offset = 70.5287794 * M_PI / 180.0;

    for (int i = 0; i < 101; i++)
    {
        Point p;
        double theta = -offset / 2 + i * offset / 100;
        p.x = radious * cos(theta);
        p.y = radious * sin(theta);
        points.push_back(p);
    }
    return points;
}

void draw_cylinder_half(double radious, double height)
{
    if (radious < 0)
    {
        printf("Cylinder radius is negative\n");
        return;
    }
    if (height > 0)
    {
        int numOfSegment = 100;
        vector<Point> points = cylinder_Points(radious);
        glBegin(GL_QUADS);
        for (int i = 0; i < numOfSegment; i++)
        {
            glVertex3f(points[i].x, points[i].y, height / 2);
            glVertex3f(points[i].x, points[i].y, -height / 2);
            glVertex3f(points[i + 1].x, points[i + 1].y, -height / 2);
            glVertex3f(points[i + 1].x, points[i + 1].y, height / 2);
        }
        glEnd();
    }
    else
    {
        printf("Cylinder height is 0\n");
    }
}
void draw_cylinder()
{
    double height = currentTriangleSide * sqrt(2.0);
    double radious = currentSphereRadius;

    if (radious > 0)
    {

        if (height > 0)
        {

            for (int j = 0; j < 2; j++)
            {
                glPushMatrix();
                {
                    glRotatef(180 * j, 0, 1, 0);
                    for (int i = 0; i < 4; i++)
                    {
                        glPushMatrix();
                        {
                            // yellow cylinder
                            glColor3f(1, 1, 0);
                            glRotatef(90 * i, 0, 0, 1);
                            glRotatef(45, 0, 1, 0);
                            glTranslatef(currentTriangleSide / sqrt(2.0), 0, 0);
                            draw_cylinder_half(radious, height);
                        }
                        glPopMatrix();
                    }
                }
                glPopMatrix();
            }
        }
        else
        {
            printf("Cylinder height is 0\n");
        }
    }
    else
    {
        printf("Cylinder radius is 0\n");
    }

    if (height > 0 && radious > 0)
    {

        for (int i = 0; i < 4; i++)
        {
            glPushMatrix();
            {
                glColor3f(1, 1, 0);
                glRotatef(90 * i, 0, 0, 1);
                glTranslatef(currentTriangleSide / 2, currentTriangleSide / 2, 0);
                glRotatef(45, 0, 0, 1);
                glRotatef(90, 1, 0, 0);
                draw_cylinder_half(radious, height);
            }
            glPopMatrix();
        }
    }
    else
    {
        printf("Cylinder height is 0\n");
    }
}

vector<vector<Point>> buildUnitPositiveX(int subdivision)
{
    const float radian = acos(-1) / 180.0f;

    int pointsPerRow = (int)pow(2, subdivision) + 1;

    vector<vector<Point>> vertices(pointsPerRow);
    float n1[3];
    float n2[3];
    float v[3];
    float a1;
    float a2;

    for (unsigned int i = 0; i < pointsPerRow; ++i)
    {
        a2 = radian * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        for (unsigned int j = 0; j < pointsPerRow; ++j)
        {
            a1 = radian * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            float scale = 1 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            vertices[i].push_back({v[0], v[1], v[2]});
        }
    }

    return vertices;
}

void drawOneFace(int subdivision)
{
    vector<vector<Point>> vertices = buildUnitPositiveX(subdivision);
    glBegin(GL_QUADS);
    for (unsigned int i = 0; i < vertices.size() - 1; ++i)
    {
        for (unsigned int j = 0; j < vertices[i].size() - 1; ++j)
        {
            glVertex3f(vertices[i][j].x, vertices[i][j].y, vertices[i][j].z);
            glVertex3f(vertices[i][j + 1].x, vertices[i][j + 1].y, vertices[i][j + 1].z);
            glVertex3f(vertices[i + 1][j + 1].x, vertices[i + 1][j + 1].y, vertices[i + 1][j + 1].z);
            glVertex3f(vertices[i + 1][j].x, vertices[i + 1][j].y, vertices[i + 1][j].z);
        }
    }
    glEnd();
}

void draw_sphere()
{
    if (currentSphereRadius > 0)
    {
        double traslaseLength = currentTriangleSide;
        for (int i = 0; i < 4; i++)
        {
            glPushMatrix();
            {
                glColor3f(0, i % 2, (i + 1) % 2);
                glRotatef(90 * i, 0, 0, 1);
                glTranslatef(traslaseLength, 0, 0);
                glScalef(currentSphereRadius, currentSphereRadius, currentSphereRadius);
                drawOneFace(6);
            }
            glPopMatrix();
        }
    }
    else
    {
        printf("Sphere radius is 0\n");
    }
    if (currentSphereRadius > 0)
    {
        for (int i = 0; i < 2; i++)
        {
            double traslaseLength = currentTriangleSide;

            glPushMatrix();
            {
                glColor3f(1, 0, 0);
                glRotatef(180 * i, 0, 1, 0);
                glRotatef(90, 0, 1, 0);
                glTranslatef(traslaseLength, 0, 0);
                glScalef(currentSphereRadius, currentSphereRadius, currentSphereRadius);
                drawOneFace(6);
            }
            glPopMatrix();
        }
    }
    else
    {
        printf("Sphere radius is 0\n");
    }
}

void drawTriangle(double scale)
{
    glScalef(scale, scale, scale);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(1, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 1);
    }
    glEnd();
}
void draw_octahedron()
{

    if (currentTriangleSide > 0)
    {

        for (int j = 0; j < 2; j++)
        {
            glPushMatrix();
            {
                glRotatef(180 * j, 0, 1, 0);
                for (int i = 0; i < 4; i++)
                {
                    double traslaseLength = maxTriangleSide - currentTriangleSide;
                    traslaseLength /= 3.0;
                    glPushMatrix();
                    {
                        glColor3f((i + 1) % 2, i % 2, 1.0);
                        glRotatef(90 * i, 0, 0, 1);
                        glTranslatef(traslaseLength, traslaseLength, traslaseLength);
                        drawTriangle(currentTriangleSide);
                    }
                    glPopMatrix();
                }
            }
            glPopMatrix();
        }
    }
    else
    {
        printf("Triangle side is 0\n");
    }
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

    glPushMatrix();
    {
        glRotatef(rotationAngle, 0, 0, 1);
        draw_octahedron();
        draw_sphere();
        draw_cylinder();
    }
    glPopMatrix();

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

    case '7':
        printf("7 pressed\n");
        // look up left
        cameraLook.x = (cameraLook.x * cos(rotation_angle)) + (cameraUp.x * sin(rotation_angle));
        cameraLook.y = (cameraLook.y * cos(rotation_angle)) + (cameraUp.y * sin(rotation_angle));
        cameraLook.z = (cameraLook.z * cos(rotation_angle)) + (cameraUp.z * sin(rotation_angle));

    case '8':
        printf("8 pressed\n");
        // look up right
        cameraLook.x = (cameraLook.x * cos(-rotation_angle)) + (cameraUp.x * sin(-rotation_angle));
        cameraLook.y = (cameraLook.y * cos(-rotation_angle)) + (cameraUp.y * sin(-rotation_angle));
        cameraLook.z = (cameraLook.z * cos(-rotation_angle)) + (cameraUp.z * sin(-rotation_angle));
        break;

    case '9':
        // look down left
        printf("9 pressed\n");
        cameraLook.x = (cameraLook.x * cos(rotation_angle)) - (cameraUp.x * sin(rotation_angle));
        cameraLook.y = (cameraLook.y * cos(rotation_angle)) - (cameraUp.y * sin(rotation_angle));
        cameraLook.z = (cameraLook.z * cos(rotation_angle)) - (cameraUp.z * sin(rotation_angle));
        break;

    case '0':
        // look down right
        printf("0 pressed\n");
        cameraLook.x = (cameraLook.x * cos(-rotation_angle)) - (cameraUp.x * sin(-rotation_angle));
        cameraLook.y = (cameraLook.y * cos(-rotation_angle)) - (cameraUp.y * sin(-rotation_angle));
        cameraLook.z = (cameraLook.z * cos(-rotation_angle)) - (cameraUp.z * sin(-rotation_angle));
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
        // size increase

        break;

    case GLUT_KEY_DOWN:
        printf("DOWN pressed");

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