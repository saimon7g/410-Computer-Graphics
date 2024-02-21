#include <bits/stdc++.h>
using namespace std;

#include <GL/glut.h>
#define pi (2 * acos(0.0))

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

// camera position
Point pos(10, 10, 0);

// up, right_, look vector
Point u(0, 0, 1);
Point r(-1 / sqrt(2), 1 / sqrt(2), 0);
Point l(-1 / sqrt(2), -1 / sqrt(2), 0);

// translation rate
double tr = 0.1;

// angle of rotation
double theta = pi/180;

// Rotate a vector v by theta about axis eta
void rotate3D(Point &v, Point &eta, double theta) {
    v = eta * dot(v, eta) * (1 - cos(theta)) + v * cos(theta) + (eta * v) * sin(theta);
}

// what to draw
int dr = 0;

// how far the center should be
double dist = 1;

// Translation
void specialKeyListener(int key, int x, int y) {
    //cout << "Special Key\n";
    switch (key) {
    case GLUT_KEY_DOWN:
        // move backward
        pos = pos - l * tr;
        break;
    case GLUT_KEY_UP:
        // move forward
        pos = pos + l * tr;
        break;

    case GLUT_KEY_RIGHT:
        pos = pos + r * tr;
        break;
    case GLUT_KEY_LEFT:
        pos = pos - r * tr;
        break;

    case GLUT_KEY_PAGE_UP:
        pos = pos + u * tr;
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos = pos - u * tr;
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

// Rotation
void keyboardListener(unsigned char key, int x, int y) {
    //cout << "Keyboard key\n";
    switch (key) {
    case '1':
        // rotate left
        rotate3D(r, u, theta);
        rotate3D(l, u, theta);
        break;
    case '2':
        // rotate right
        rotate3D(r, u, -theta);
        rotate3D(l, u, -theta);
        break;
    case '3':
        // rotate up
        rotate3D(u, r, theta);
        rotate3D(l, r, theta);
        break;
    case '4':
        // rotate down
        rotate3D(u, r, -theta);
        rotate3D(l, r, -theta);
        break;
    case '5':
        // tilt counterclockwise
        rotate3D(r, l, -theta);
        rotate3D(u, l, -theta);
        break;
    case '6':
        // tilt clockwise
        rotate3D(r, l, theta);
        rotate3D(u, l, theta);
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

void axes() {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES); {
        glVertex3f(100, 0, 0);
        glVertex3f(-100, 0, 0);

        glVertex3f(0, -100, 0);
        glVertex3f(0, 100, 0);

        glVertex3f(0, 0, 100);
        glVertex3f(0, 0, -100);
    }glEnd();
}

void square(double a) {
    glBegin(GL_QUADS);
    {
        glVertex3f(a, a, 0);
        glVertex3f(a, -a, 0);
        glVertex3f(-a, -a, 0);
        glVertex3f(-a, a, 0);
    }glEnd();
}

void drawCube(double x) {
    if (dr != 2) {
        for (int i = 0; i < 4; i++) {
            glPushMatrix();
            glRotatef(i * 90 , 1, 0, 0);
            glTranslatef(0, 0, x);
            Color color = Color::red();
            glColor3f(color.r, color.g, color.b);
            square(x);
            glPopMatrix();
        }
    }
    if (dr != 1) {
        for (int i = 0; i < 4; i++) {
            glPushMatrix();
            glRotatef(i * 90 , 0, 1, 0);
            glTranslatef(0, 0, x);
            Color color = Color::green();
            glColor3f(color.r, color.g, color.b);
            square(x);
            glPopMatrix();
        }
    }
}

void drawAll() {
    axes();
    drawCube(2);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    cout << "dist: " << dist << '\n';

    gluLookAt(pos.x, pos.y, pos.z, 
			pos.x + l.x, pos.y + l.y, pos.z + l.z, 
            u.x, u.y, u.z);

    drawAll();
    
    glutSwapBuffers();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // glViewport(0, 0, 600, 800);

    // // Load Projection Matrix Stack
    // glMatrixMode(GL_PROJECTION);
    // // Clear the matrix
    // glLoadIdentity();
    // // get perspective parameters
    // gluPerspective(60, 1, 1, 70);
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

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(70, 70);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Camera Position");
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    init();
    glutMainLoop();
    return 0;
}