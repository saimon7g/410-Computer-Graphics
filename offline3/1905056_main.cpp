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

int recursionLevel = 0;
int screenDimension = 0;
int numberOfObjects = 0;


void loadData()
{
    ifstream file;
    file.open("scene.txt");
    if (!file.is_open())
    {
        printf("File not found\n");
        return;
    }
    file >> recursionLevel;
    file >> screenDimension;
    file >> numberOfObjects;

    string object_type;
    for(int i=0;i<numberOfObjects;i++){
        file >> object_type;
        if(object_type == "triangle"){
            double x,y,z;
            file >> x >> y >> z;
            printf("Triangle %lf %lf %lf\n",x,y,z);
        }
        else if(object_type == "sphere"){
            double x,y,z;
            file >> x >> y >> z;
            printf("Sphere %lf %lf %lf\n",x,y,z);
        }
        else if(object_type == "object"){
            double x,y,z;
            file >> x >> y >> z;
            printf("Cylinder %lf %lf %lf\n",x,y,z);
        }
    }



    file.close();
}

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

void draw_cylinder_half(double radious, double height)
{

    // double slices = 200;
    // double stacks = 200;
    // double total_theta = acos(-1 / 3.0);
    // double theta = 2 * pi / slices;
    // double stack_height = height / stacks;
    // double temp = -theta / 2;

    // printf("------------------------------------------------------------------\n");

    // printf("total_theta %lf\n", total_theta);
    // printf("theta %lf\n", 180 * theta / pi);
    // printf("stack_height %lf\n", stack_height);
    // printf("height %lf\n", height);
    // printf("radious %lf\n", radious);
    // printf("slices %lf\n", slices);
    // printf("stacks %lf\n", stacks);
    // printf("temp %lf\n", temp);

    // for (int i = 0; i < slices; i++)
    // {
    //     if (i * theta > total_theta)
    //     {
    //         continue;
    //     }
    //     double x1 = radious * cos((i * theta) + temp);
    //     double y1 = radious * sin((i * theta) + temp);
    //     double x2 = radious * cos(((i + 1) * theta) + temp);
    //     double y2 = radious * sin(((i + 1) * theta) + temp);

    //     for (int j = 0; j < stacks; j++)
    //     {

    //         glBegin(GL_QUADS);
    //         {
    //             glVertex3f(x1, y1, 0);
    //             glVertex3f(x2, y2, 0);
    //             glVertex3f(x2, y2, height);
    //             glVertex3f(x1, y1, height);
    //         }
    //         glEnd();
    //     }
    // }

    int segments = 100;

    double radius = radious;
    struct Point points[segments + 1];

    double offset = 70.5287794 * M_PI / 180.0;

    for (int i = 0; i < segments + 1; i++)
    {
        double theta = -offset / 2 + i * offset / segments;
        points[i].x = radius * cos(theta);
        points[i].y = radius * sin(theta);
    }

    glBegin(GL_QUADS);
    for (int i = 0; i < segments; i++)
    {
        glVertex3f(points[i].x, points[i].y, height / 2);
        glVertex3f(points[i].x, points[i].y, -height / 2);
        glVertex3f(points[i + 1].x, points[i + 1].y, -height / 2);
        glVertex3f(points[i + 1].x, points[i + 1].y, height / 2);
    }
    glEnd();
}
void draw_cylinder()
{
    double height = currentTriangleSide * sqrt(2.0);
    double radious = currentSphereRadius;
    double difference = currentTriangleSide / sqrt(3.0);
    double theta = 180 - pi * acos(-1 / 3.0);
    difference = difference;

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

    // for(int i=0;i<4;i++){
    //     glPushMatrix();
    //     {
    //         glColor3f(1,1,0);
    //         glTranslatef(difference,difference,0);
    //                     glRotatef(-45,0,0,1);
    //         glRotatef(90,0,1,0);
    //         glTranslatef(0,0,-height/2);
    //         glRotatef(theta/2,0,0,1);
    //         draw_cylinder_half(radious, height);
    //     }
    //     glPopMatrix();
    // }
}

void draw_sphere_quad()
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
            x = currentSphereRadius * cos(stack_angle) * cos(sector_angle);
            y = currentSphereRadius * cos(stack_angle) * sin(sector_angle);
            z = currentSphereRadius * sin(stack_angle);

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
                glVertex3f(mat[i][j].x, mat[i][j].y, mat[i][j].z);
                glVertex3f(mat[i][j + 1].x, mat[i][j + 1].y, mat[i][j + 1].z);
                glVertex3f(mat[i + 1][j + 1].x, mat[i + 1][j + 1].y, mat[i + 1][j + 1].z);
                glVertex3f(mat[i + 1][j].x, mat[i + 1][j].y, mat[i + 1][j].z);
            }
            glEnd();
        }
    }
}

void draw_sphere_slice()
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

            // Only draw one-sixth of the sphere (e.g., the upper right front portion)
            if (stack_angle >= 0 && stack_angle <= pi / 3 && sector_angle >= 0 && sector_angle <= pi / 3)
            {
                x = currentSphereRadius * cos(stack_angle) * cos(sector_angle);
                y = currentSphereRadius * cos(stack_angle) * sin(sector_angle);
                z = currentSphereRadius * sin(stack_angle);

                mat[i][j].x = x;
                mat[i][j].y = y;
                mat[i][j].z = z;
            }
        }
    }

    for (int i = 0; i < stack; i++)
    {
        for (int j = 0; j < sector; j++)
        {
            glBegin(GL_QUADS);
            {
                glVertex3f(mat[i][j].x, mat[i][j].y, mat[i][j].z);
                glVertex3f(mat[i][j + 1].x, mat[i][j + 1].y, mat[i][j + 1].z);
                glVertex3f(mat[i + 1][j + 1].x, mat[i + 1][j + 1].y, mat[i + 1][j + 1].z);
                glVertex3f(mat[i + 1][j].x, mat[i + 1][j].y, mat[i + 1][j].z);
            }
            glEnd();
        }
    }
}

vector<vector<Point>> buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    vector<vector<Point>> vertices(pointsPerRow);
    float n1[3]; // normal of longitudinal plane rotating along Y-axis
    float n2[3]; // normal of latitudinal plane rotating along Z-axis
    float v[3];  // direction vector intersecting 2 planes, n1 x n2
    float a1;    // longitudinal angle along Y-axis
    float a2;    // latitudinal angle along Z-axis

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for (unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for (unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            vertices[i].push_back({v[0], v[1], v[2]});

            // vertices.push_back({v[0], v[1], v[2]});
            //  vertices.push_back(v[1]);
            //  vertices.push_back(v[2]);
        }
    }

    return vertices;
}

void drawOneFace(int subdivision)
{
    vector<vector<Point>> vertices = buildUnitPositiveX(subdivision);

    // draw +X face of the cube using quadrilaterals
    glBegin(GL_QUADS);
    for (unsigned int i = 0; i < vertices.size() - 1; ++i)
    {
        for (unsigned int j = 0; j < vertices[i].size() - 1; ++j)
        {
            // draw a quadrilateral using 4 vertices of 2 adjacent rows
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
    for (int i = 0; i < 2; i++)
    {
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

void drawTriangle()
{

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
    double difference = maxTriangleSide - currentTriangleSide;
    difference /= 3.0;

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glColor3f((i + 1) % 2, i % 2, 1.0);
            glRotatef(90 * i, 0, 0, 1);
            glTranslatef(difference, difference, difference);
            glScalef(currentTriangleSide, currentTriangleSide, currentTriangleSide);
            drawTriangle();
        }
        glPopMatrix();
    }

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glColor3f(i % 2, (i + 1) % 2, 1.0);
            glRotatef(90 * i, 0, 0, 1);
            glTranslatef(difference, difference, -difference);
            glScalef(currentTriangleSide, currentTriangleSide, currentTriangleSide);
            glRotatef(180, 1, 1, 0);
            drawTriangle();
        }
        glPopMatrix();
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

    // draw_cylinder(5,10);

    // drawTriangle();
    // draw_octahedron();

    // draw_sphere();

    // draw_sphere_slice();
    // glScalef(currentSphereRadius, currentSphereRadius, currentSphereRadius);
    // drawOneFace(6);

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
        cameraRight.x = (cameraRight.x * cos(-rotation_angle)) + (cameraLook.x * sin(-rotation_angle));
        cameraRight.y = (cameraRight.y * cos(-rotation_angle)) + (cameraLook.y * sin(-rotation_angle));
        cameraRight.z = (cameraRight.z * cos(-rotation_angle)) + (cameraLook.z * sin(-rotation_angle));

        cameraLook.x = (cameraLook.x * cos(-rotation_angle)) - (cameraRight.x * sin(-rotation_angle));
        cameraLook.y = (cameraLook.y * cos(-rotation_angle)) - (cameraRight.y * sin(-rotation_angle));
        cameraLook.z = (cameraLook.z * cos(-rotation_angle)) - (cameraRight.z * sin(-rotation_angle));
        break;
    case '2':
        printf("2 pressed\n");
        cameraRight.x = (cameraRight.x * cos(rotation_angle)) + (cameraLook.x * sin(rotation_angle));
        cameraRight.y = (cameraRight.y * cos(rotation_angle)) + (cameraLook.y * sin(rotation_angle));
        cameraRight.z = (cameraRight.z * cos(rotation_angle)) + (cameraLook.z * sin(rotation_angle));

        cameraLook.x = (cameraLook.x * cos(rotation_angle)) - (cameraRight.x * sin(rotation_angle));
        cameraLook.y = (cameraLook.y * cos(rotation_angle)) - (cameraRight.y * sin(rotation_angle));
        cameraLook.z = (cameraLook.z * cos(rotation_angle)) - (cameraRight.z * sin(rotation_angle));
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
        cameraPos.y += cameraRight.y;
        cameraPos.x += cameraRight.x;
        cameraPos.z += cameraRight.z;
        break;
    case GLUT_KEY_RIGHT:
        printf("RIGHT pressed");
        cameraPos.x -= cameraRight.x;
        cameraPos.y -= cameraRight.y;
        cameraPos.z -= cameraRight.z;
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
    // printf("Hello World\n");
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(500, 500);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("OpenGl Test");

    init();
    loadData();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardHandler);
    glutSpecialFunc(specialKeyHandler);

    glutIdleFunc(idle);
    glutTimerFunc(10, Timer, 0);



    glutMainLoop();
    return 0;
}