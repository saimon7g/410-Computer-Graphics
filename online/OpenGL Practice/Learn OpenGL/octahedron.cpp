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

Point pos = Point(10, 0, 4);
Point l = Point(-1, 0, 0);
Point r = Point(0, 1, 0);
Point u = Point(0, 0, 1);

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

    case 'd':
        dr = (dr + 1) % 3;

    default:
        break;
    }
    glutPostRedisplay();
}

void drawAxes() {
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

void drawTriangle(double a) {
    glBegin(GL_TRIANGLES); {
        glVertex3f(a, 0, 0);
        glVertex3f(0, a, 0);
        glVertex3f(0, 0, a);
    }glEnd();
}

void drawOctahedronPart(double a, int add) {
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glRotatef(90 * i, 0, 0, 1);
        if ((i + add) % 2 == 0 & dr != 2) {
            Color color = Color::red();
            glColor3f(color.r, color.g, color.b);
            drawTriangle(a);
        }
        else if ((i + add) % 2 == 1 & dr != 1) {
            Color color = Color::green();
            glColor3f(color.r, color.g, color.b);
            drawTriangle(a);
        }
        glPopMatrix();
    }
}

void drawOctahedron(double a) {
    drawOctahedronPart(a, 1);
    glScalef(1, 1, -1);
    drawOctahedronPart(a, 0);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(pos.x, pos.y, pos.z, 
			pos.x + l.x, pos.y + l.y, pos.z + l.z, 
            u.x, u.y, u.z);

    Color color = Color::red();
    glColor3f(color.r, color.g, color.b);
    drawOctahedron(5);

    drawAxes();

    glutSwapBuffers();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
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