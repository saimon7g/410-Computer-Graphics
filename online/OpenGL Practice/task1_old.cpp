#include <bits/stdc++.h>
using namespace std;

#include <GL/glut.h>
#define pi (2*acos(0.0))

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
    static Color magenta() {
        return Color(1, 0, 1);
    }
};

struct Point {
    double x, y, z;
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

// camera position
Point pos(10, 10, 0);

// up, right_, look vector
Point up(0, 0, 1);
Point right_(-1 / sqrt(2), 1 / sqrt(2), 0);
Point look(-1 / sqrt(2), -1 / sqrt(2), 0);
// Point right_(0, -1, 0);
// Point look(0, 0, 1);

// angle of rotation
double angle = pi / 180;

// Rotate a vector v by theta about axis eta where eta is perpendicular to v
void rotate3D(Point& v, Point& eta, double theta) {
    v = v * cos(theta) + (eta * v) * sin(theta);
}

// Translation
void specialKeyListener(int key, int x, int y) {
    cout << "Special Key\n";
    switch (key) {
    case GLUT_KEY_DOWN:
        // move backward
        pos = pos - look;
        break;
    case GLUT_KEY_UP:
        // move forward
        pos = pos + look;
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

    default:
        break;
    }
    glutPostRedisplay();
}

// Rotation
void keyboardListener(unsigned char key, int x, int y) {
    cout << "Keyboard key\n";
    switch (key) {
    case '1':
        // rotate left
        rotate3D(right_, up, angle);
        rotate3D(look, up, angle);
        break;
    case '2':
        // rotate right
        rotate3D(right_, up, -angle);
        rotate3D(look, up, -angle);
        break;
    case '3':
        // rotate up
        rotate3D(up, right_, angle);
        rotate3D(look, right_, angle);
        break;
    case '4':
        // rotate down
        rotate3D(up, right_, -angle);
        rotate3D(look, right_, -angle);
        break;
    case '5':
        // tilt counterclockwise
        rotate3D(right_, look, -angle);
        rotate3D(up, look, -angle);
        break;
    case '6':
        // tilt clockwise
        rotate3D(right_, look, angle);
        rotate3D(up, look, angle);
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

void display() {
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(pos.x, pos.y, pos.z, 
			pos.x + look.x, pos.y + look.y, pos.z + look.z, 
            up.x, up.y, up.z);

    axes();
    //glColor3f(1, 1, 1);
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glRotatef(i * 90 , 1, 0, 0);
        glTranslatef(0, 0, 2);
        Color color = Color::red();
        glColor3f(color.r, color.g, color.b);
        square(2);
        glPopMatrix();   

        glPushMatrix();
        glRotatef(i * 90 , 0, 1, 0);
        glTranslatef(0, 0, 2);
        color = Color::green();
        glColor3f(color.r, color.g, color.b);
        square(2);
        glPopMatrix();  
    }
    glutSwapBuffers();
}

void init() {
    angle = (pi) / 180;

    // Load Projection Matrix Stack
    glMatrixMode(GL_PROJECTION);
    // Clear the matrix
    glLoadIdentity();
    // get perspective parameters
    gluPerspective(60, 1, 1, 70);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(480, 480);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenGL Demo");

    init();

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    glutMainLoop();
    return 0;
}