#include <bits/stdc++.h>
using namespace std;

#include <GL/glut.h>
#define pi (2*acos(0.0))
// Global variables

struct Color {
    double r, g, b;
    Color() {
        r = 0, g = 0, b = 0;
    }
    Color(double x, double y, double z) {
        r = x, g = y, b = z;
    }
    static Color red() {
        return Color(1, 0, 0);
    }
    static Color green() {
        return Color(0, 1, 0);
    }
    static Color blue() {
        return Color(0, 0, 1);
    }
    static Color white() {
        return Color(1, 1, 1);
    }
    static Color black() {
        return Color(0, 0, 0);
    }
    static Color yellow() {
        return Color(1, 1, 0);
    }
    static Color cyan() {
        return Color(0, 1, 1);
    }
    static Color purple() {
        return Color(1, 0, 1);
    }
};

struct Point {
    double x, y, z;
    Point() {
        x = 0, y = 0, z = 0;
    }
    Point(double a, double b, double c) {
        x = a, y = b, z = c;
    }
    Point(const Point& p) {
        x = p.x, y = p.y, z = p.z;
    }
    void print() {
        cout << x << ' ' << y << ' ' << z << '\n';
    }
    Point operator +(Point p) {
        return Point(x + p.x, y + p.y, z + p.z);
    }
    Point operator -(Point p) {
        return Point(x - p.x, y - p.y, z - p.z);
    }
    Point operator *(double a) {
        return Point(x * a, y * a, z * a);
    }
    Point operator /(double a) {
        return Point(x / a, y / a, z / a);
    }
    Point operator *(Point p) {
        return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
    }
};

double dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
double length(Point a) {
    return sqrt(dot(a, a));
}
Point normalize(Point a) {
    return a / length(a);
}

// Global variables
struct Point pos;
struct Point look;
struct Point right_;
struct Point up;
double angleRotationZ = 45.0;

// angle of rotation
double angle = pi / 180;

// angle of rotation about axis
double rot_angle = 30 * pi/180;

// total rotation to make
double tot_rot = 0;

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   
    glEnable(GL_DEPTH_TEST);   

    pos = Point(5, 0, 2);
    look = Point(-1, 0, 0);
    right_ = Point(0, 1, 0);
    up = Point(0, 0, 1);
}
/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes() {
    //glLineWidth(3);
    glBegin(GL_LINES);
    Color color = Color::red();
    glColor3f(color.r, color.g, color.b);
    // X axis
    glVertex3f(-2, 0, 0);
    glVertex3f(2, 0, 0);
    color = Color::green();
    glColor3f(color.r, color.g, color.b);
    // Y axis
    glVertex3f(0, -2, 0);
    glVertex3f(0, 2, 0);

    color = Color::blue();
    glColor3f(color.r, color.g, color.b);
    // Z axis
    glVertex3f(0, 0, -2);
    glVertex3f(0, 0, 2);
    glEnd();
}
void drawTriangle() {
    glBegin(GL_TRIANGLES); {
        glVertex3f(1, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 1);
    }glEnd();
}
int numSegments = 36;

void drawCylinder(double height, double radius, int segments) {
    struct Point points[segments + 1];
    double offset = 70.5287794 * M_PI / 180.0;
    // double offset = 0;
    for (int i = 0; i < segments + 1; i++) {
        double theta = -offset / 2 +  i * offset / segments;
        // double theta = i * offset / segments;
        points[i].x = radius * cos(theta);
        points[i].y = radius * sin(theta);
    }

    glBegin(GL_QUADS);
    for (int i = 0; i < segments; i++) {
        glVertex3f(points[i].x, points[i].y, height / 2);
        glVertex3f(points[i].x, points[i].y, -height / 2);
        glVertex3f(points[i + 1].x, points[i + 1].y, -height / 2);
        glVertex3f(points[i + 1].x, points[i + 1].y, height / 2);
    }
    glEnd();
}

void drawCubeSphere(Color color) {
    // number of subdivisions
    int subdivision = 3;

    // generate vertices for +X face only by intersecting 2 circular planes
    // (longitudinal and latitudinal) at the given longitude/latitude angles
    const float DEG2RAD = acos(-1) / 180.0f;

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    tuple<double, double, double> vertices[pointsPerRow][pointsPerRow];

    double n1[3];        // normal of longitudinal plane rotating along Y-axis
    double n2[3];        // normal of latitudinal plane rotating along Z-axis
    double v[3];         // direction vector intersecting 2 planes, n1 x n2
    double a1;           // longitudinal angle along Y-axis
    double a2;           // latitudinal angle along Z-axis

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for (int i = 0; i < pointsPerRow; ++i) {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for (int j = 0; j < pointsPerRow; ++j) {
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
            vertices[i][j] = make_tuple(v[0], v[1], v[2]);
        }

        // draw sphere
        glBegin(GL_QUADS); {
            glColor3f(color.r, color.g, color.b);
            for (int i = 0; i < pointsPerRow - 1; ++i) {
                for (int j = 0; j < pointsPerRow - 1; ++j) {
                    tuple<double, double, double>vertex = vertices[i][j];
                    glVertex3f(get<0>(vertex), get<1>(vertex), get<2>(vertex));

                    vertex = vertices[i][j + 1];
                    glVertex3f(get<0>(vertex), get<1>(vertex), get<2>(vertex));

                    vertex = vertices[i + 1][j + 1];
                    glVertex3f(get<0>(vertex), get<1>(vertex), get<2>(vertex));

                    vertex = vertices[i + 1][j];
                    glVertex3f(get<0>(vertex), get<1>(vertex), get<2>(vertex));
                }
            }
        }glEnd();
    }
}
double sphereRadius = 0;

double maxTriangleLength = 1;
double triangleLength = 1;

double maxSphereRadius = maxTriangleLength / sqrt(3.0);
double sphereStep = maxSphereRadius / 10.0;

void drawCylinders() {
    glColor3f(1.0f, 1.0f, 0.0f);
    for (int i = 0;i < 4; i++) {
        glPushMatrix(); {
            glRotatef(45 + i * 90, 0, 1, 0);
            glTranslatef(triangleLength / sqrt(2), 0, 0);
            drawCylinder(triangleLength * sqrt(2), sphereRadius, 100);
        }glPopMatrix();

    }
    for (int i = 0; i < 4; i++) {
        glPushMatrix(); {
            glRotatef(90, 0, 0, 1);
            glRotatef(45 + i * 90, 0, 1, 0);
            glTranslatef(triangleLength / sqrt(2), 0, 0);
            drawCylinder(triangleLength * sqrt(2), sphereRadius, 100);
        }glPopMatrix();
    }
    for (int i = 0; i < 4; i++) {
        glPushMatrix(); {
            glRotatef(90, 1, 0, 0);
            glRotatef(45 + i * 90, 0, 1, 0);
            glTranslatef(triangleLength / sqrt(2), 0, 0);
            drawCylinder(triangleLength * sqrt(2), sphereRadius, 100);
        }glPopMatrix();
    }
}

void drawSphere() {
    //double r = 1-sqrt(2)*sphereRadius;
    double r = triangleLength;
    Color colors[] = {
        Color::green(),
        Color::green(),
        Color::blue(),
        Color::blue(),
        Color::red(),
        Color::red()
    };
    // 2d array of centers
    double centers[6][3] = {
        {triangleLength, 0, 0},
        {-triangleLength, 0, 0},
        {0, 0, triangleLength},
        {0, 0, -triangleLength},
        {0, triangleLength, 0},
        {0, -triangleLength, 0}
    };
    // rotation angles
    int angles[6] = { 0, 180, 270, 90, 90, 270 };
    // rotation axis
    double axis[6][3] = {
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0},
        {0, 0, 1},
        {0, 0, 1}
    };

    for (int i = 0; i < 6; i++) {
        glPushMatrix(); {
            glTranslatef(centers[i][0], centers[i][1], centers[i][2]);
            glScalef(sphereRadius, sphereRadius, sphereRadius);
            glRotatef(angles[i], axis[i][0], axis[i][1], axis[i][2]);
            drawCubeSphere(colors[i]);
            //drawSphere();
        }glPopMatrix();
    }

}

void drawPyramid(Color colors[]) {
    double diff = maxTriangleLength - triangleLength;
    diff = diff / 3.0;
    for (int i = 0;i < 4;i++) {
        glPushMatrix(); {
            glColor3f(colors[i % 2].r, colors[i % 2].g, colors[i % 2].b);
            glRotatef(90 * i, 0, 1, 0);
            glTranslatef(diff, diff, diff);
            glScaled(triangleLength, triangleLength, triangleLength);
            drawTriangle();
        }glPopMatrix();
    }
}

void drawOctaHedron() {
    Color colors[2];
    colors[0] = Color::purple();
    colors[1] = Color::cyan();
    drawPyramid(colors);
    colors[0] = Color::cyan();
    colors[1] = Color::purple();
    glPushMatrix(); {
        glRotatef(180, 1, 0, 1);
        drawPyramid(colors);
    }glPopMatrix();

}
void drawAll() {
    drawOctaHedron();
    drawAxes();
    drawSphere();
    drawCylinders();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();                       
    
    gluLookAt(pos.x, pos.y, pos.z,
        pos.x + look.x, pos.y + look.y, pos.z + look.z,
        up.x, up.y, up.z);
    // draw
    glRotatef(tot_rot, 0, 0, 1);

    drawAll();

    glutSwapBuffers();
}

void reshapeListener(GLsizei width, GLsizei height) {
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(80, 1, 1, 1000.0);
}

void rotate3D(Point& v, Point& eta, double theta) {
    v = v * cos(theta) + (eta * v) * sin(theta);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    double v = 0.1;
    double rate = 0.1;
    double s;
    switch (key) {
    case ' ':
        printf("pos: %f %f %f\n", pos.x, pos.y, pos.z);
        printf("l: %f %f %f\n", look.x, look.y, look.z);
        printf("r: %f %f %f\n", right_.x, right_.y, right_.z);
        printf("u: %f %f %f\n", up.x, up.y, up.z);
        printf("angle: %f\n", angleRotationZ);

        break;
    case 'w':
        pos.z += v;
        break;
    case 's':
        pos.z -= v;
        break;
    case '1':
        rotate3D(right_, up, angle);
        rotate3D(look, up, angle);
        break;

    case '2':
        rotate3D(right_, up, -angle);
        rotate3D(look, up, -angle);
        break;

    case '3':
        rotate3D(up, right_, angle);
        rotate3D(look, right_, angle);
        break;

    case '4':
        rotate3D(up, right_, -angle);
        rotate3D(look, right_, -angle);
        break;

    case '5':
        rotate3D(right_, look, -angle);
        rotate3D(up, look, -angle);
        break;

    case '6':
        rotate3D(right_, look, angle);
        rotate3D(up, look, angle);
        break;
    case ',':
        triangleLength -= 0.1;
        sphereRadius += sphereStep;
        if (triangleLength < 0) {
            triangleLength = 0;
            sphereRadius = maxSphereRadius;
        }
        break;
    case '.':
        triangleLength += 0.1;
        sphereRadius -= sphereStep;
        if (triangleLength > maxTriangleLength) {
            triangleLength = maxTriangleLength;
            sphereRadius = 0;
        }
        break;
    
    case 'a':
        tot_rot -= rot_angle;
        break;

    case 'd':
        tot_rot += rot_angle;
        break;

    default:
        break;
    }
    glutPostRedisplay();
}


void specialKeyListener(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP:
        pos = pos + look;
        break;
    case GLUT_KEY_DOWN:
        pos = pos - look;
        break;

    case GLUT_KEY_RIGHT:
        pos = pos + right_;
        break;
    case GLUT_KEY_LEFT:
        pos = pos - right_;
        break;

    case GLUT_KEY_PAGE_UP:
        pos = pos + up;
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos = pos - up;
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(70, 70);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Magic Cube");
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    init();
    glutMainLoop();
    return 0;
}