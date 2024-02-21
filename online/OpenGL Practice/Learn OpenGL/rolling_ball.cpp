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

struct Plane {
    double a, b, c, d;
    Plane() {
        a = 0, b = 0, c = 0, d = 0;
    }
    Plane(double a_, double b_, double c_, double d_) {
        a = a_, b = b_, c = c_, d = d_;
    }
    int isPart(Point p) {
        if (a * p.x + b * p.y + c * p.z >= d) {
            return 1;
        }
        return 0;
    }
    void print() {
        cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
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
// Camera points
Point pos;
Point l;
Point r;
Point u;

// position of sphere
Point sphere_positon;

// axis of translation
Point trl;

// axis of rotation
Point rot;

// axis of rotation of axis
Point rot_axis;

// boundary length
double boundary = 1;

// radius
double radius = 0.2;

// translation rate
double tr = 0.1;

// angle of rotation
double theta = pi/180;

// vertices of sphere
vector<vector<Point>> vertices_global;

// angle of rotation for ball axis
double angleBallAxis = 10 * pi / 180;

// angle of rotation for ball
double angleBall = 15 * pi / 180;

// plane equations
Plane planes[4];

// plane normals
Point normals[4];

// ball mode -> 0 is normal, 1 is simulation
int mode = 0;

// Rotate a vector v by theta about axis eta
void rotate3D(Point &v, Point &eta, double theta) {
    v = eta * dot(v, eta) * (1 - cos(theta)) + v * cos(theta) + (eta * v) * sin(theta);
}

void reflect3D(Point& v, Point& eta) {
    v = v - eta * dot(v, eta) * 2;
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

    case 'j':
        rotate3D(trl, rot_axis, angleBallAxis);
        rot = rot_axis * trl;
        break;
    case 'l':
        rotate3D(trl, rot_axis, -angleBallAxis);
        rot = rot_axis * trl;
        break;
    
    case 'i':
        for (int i = 0; i < vertices_global.size(); i++) {
            for (int j = 0; j < vertices_global[i].size(); j++) {
                rotate3D(vertices_global[i][j], rot, angleBall);
            }
        }
        sphere_positon = sphere_positon + trl * (angleBall * radius);
        for (int i = 0; i < 4; i++) {
            Point points[] = { Point(radius, 0, 0), Point(-radius, 0, 0), Point(0, radius, 0), Point(0, -radius, 0) };
            for (int j = 0; j < 4; j++) {
                Point front = sphere_positon + points[j];
                if (planes[i].isPart(front)) {
                    reflect3D(trl, normals[i]);
                    rot = rot_axis * trl;
                }
            }
        }
        break;
    
    case 'k':
        for (int i = 0; i < vertices_global.size(); i++) {
            for (int j = 0; j < vertices_global[i].size(); j++) {
                rotate3D(vertices_global[i][j], rot, -angleBall);
            }
        }
        sphere_positon = sphere_positon - trl * (angleBall * radius);
        for (int i = 0; i < 4; i++) {
            Point points[] = { Point(radius, 0, 0), Point(-radius, 0, 0), Point(0, radius, 0), Point(0, -radius, 0) };
            for (int j = 0; j < 4; j++) {
                Point front = sphere_positon + points[j];
                if (planes[i].isPart(front)) {
                    reflect3D(trl, normals[i]);
                    rot = rot_axis * trl;
                }
            }
        }
        break;

    case 'd':
        dr = (dr + 1) % 3;

    default:
        break;
    }
    glutPostRedisplay();
}

void drawAxes() {
    //glLineWidth(3);
    glBegin(GL_LINES);{
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
    }glEnd();
}

void drawCheckerBoard() {
    Color color[2];
    color[0] = Color::black();
    color[1] = Color::white();
    glBegin(GL_QUADS); {
        for (int i = -1000; i < 1000; i++) {
            for (int j = -1000; j < 1000; j++) {
                glColor3f(color[(i + j + 200) % 2].r, color[(i + j + 200) % 2].g, color[(i + j + 200) % 2].b);
                glVertex3f(i * 0.25, j * 0.25, 0);
                glVertex3f(i * 0.25, (j + 1) * 0.25, 0);
                glVertex3f((i + 1) * 0.25, (j + 1) * 0.25, 0);
                glVertex3f((i + 1) * 0.25, j * 0.25, 0);
            }
        }
    }glEnd();
}

void drawRectangle(double a) {
    Color color = Color::red();
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_QUADS); {
        glVertex3f(a, a, 0.5);
        glVertex3f(a, -a, 0.5);
        glVertex3f(a, -a, -0.5);
        glVertex3f(a, a, -0.5);
    }glEnd();
}

void drawBoundaries(double a) {
    for (int i = 0; i < 4; i++) {
        glPushMatrix(); {
            glRotatef(90 * i, 0, 0, 1);
            drawRectangle(a);
        }glPopMatrix();
    }
}

vector<vector<Point>> getSphereVertices(int stackCount, int sectorCount, float radius) {
    float x, y, z, xy;

    float sectorStep = 2 * pi / sectorCount;
    float stackStep = pi / stackCount;
    float sectorAngle, stackAngle;

    vector<vector<Point>> vertices(stackCount + 1);

    for (int i = 0; i <= stackCount; i++) {
        stackAngle = pi/2 - i * stackStep;          // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        vertices[i] = vector<Point>(sectorCount + 1);
        for (int j = 0; j <= sectorCount; j++) {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi
            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices[i][j].x = x;
            vertices[i][j].y = y;
            vertices[i][j].z = z;
        }
    }
    return vertices;
}

void drawSphere(vector<vector<Point>> vertices, int stackCount, int sectorCount) {
    Color colors[2];
    colors[0] = Color::red();
    colors[1] = Color::green();
    glBegin(GL_TRIANGLES); {
        for (int i = 0; i < stackCount; i++) {
            for (int j = 0; j < sectorCount; j++) {
                Color cur = colors[(i + j) % 2];
                glColor3f(cur.r, cur.g, cur.b);
                if (i != 0) {
                    glVertex3f(vertices[i][j].x, vertices[i][j].y, vertices[i][j].z);
                    glVertex3f(vertices[i + 1][j].x, vertices[i + 1][j].y, vertices[i + 1][j].z);
                    glVertex3f(vertices[i][j + 1].x, vertices[i][j + 1].y, vertices[i][j + 1].z);
                }
                if (i != stackCount - 1) {
                    glVertex3f(vertices[i][j + 1].x, vertices[i][j + 1].y, vertices[i][j + 1].z);
                    glVertex3f(vertices[i + 1][j].x, vertices[i + 1][j].y, vertices[i + 1][j].z);
                    glVertex3f(vertices[i + 1][j + 1].x, vertices[i + 1][j + 1].y, vertices[i + 1][j + 1].z);
                }
            }
        }
    }glEnd();
}

void drawArrow() {
    Color color = Color::blue();
    glLineWidth(5);
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_LINES); {
        glVertex3f(0, 0, 0);
        glVertex3f(trl.x/2, trl.y/2, 0);
    }glEnd();
    
    glBegin(GL_TRIANGLES); {
        Point vertex1 = trl * 0.98 + rot * 0.05;
        Point vertex2 = trl * 0.98 - rot * 0.05;
        Point vertex3 = trl * 1.05;
        glVertex3f(vertex1.x/2, vertex1.y/2, vertex1.z/2);
        glVertex3f(vertex2.x/2, vertex2.y/2, vertex2.z/2);
        glVertex3f(vertex3.x/2, vertex3.y/2, vertex3.z/2);
    }glEnd();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(pos.x, pos.y, pos.z, 
			pos.x + l.x, pos.y + l.y, pos.z + l.z, 
            u.x, u.y, u.z);

    // drawSphere(getSphereVertices(10, 10, radius), 10, 10);

    // drawAxes();
    drawCheckerBoard();

    drawBoundaries(1);

    glPushMatrix(); {
        glTranslatef(sphere_positon.x, sphere_positon.y, sphere_positon.z);
        glTranslatef(0, 0, 0.2);
        drawSphere(vertices_global, 10, 10);
        drawArrow();
    }glPopMatrix();

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

void animate() {
    if (mode == 1) {
        for (int i = 0; i < vertices_global.size(); i++) {
            for (int j = 0; j < vertices_global[i].size(); j++) {
                rotate3D(vertices_global[i][j], rot, angleBall);
            }
        }
        sphere_positon = sphere_positon + trl * (angleBall * radius);
        for (int i = 0; i < 4; i++) {
            Point points[] = { Point(radius, 0, 0), Point(-radius, 0, 0), Point(0, radius, 0), Point(0, -radius, 0) };
            for (int j = 0; j < 4; j++) {
                Point front = sphere_positon + points[j];
                if (planes[i].isPart(front)) {
                    reflect3D(trl, normals[i]);
                    rot = rot_axis * trl;
                }
            }
        }
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    pos = Point(1, 0, 2);
    l = Point(-1 / sqrt(11), -1 / sqrt(11), -3 / sqrt(11));
    r = normalize(Point(-1, 0, 1 / 3));
    u = l * r;

    sphere_positon = Point(0, 0, 0);

    trl = Point(1, 0, 0);
    rot = Point(0, 1, 0);
    rot_axis = Point(0, 0, 1);

    mode = 1;

    vertices_global = getSphereVertices(10, 10, radius);

    planes[0] = Plane(1, 0, 0, 1);
    normals[0] = Point(-1, 0, 0);
    planes[1] = Plane(0, 1, 0, 1);
    normals[1] = Point(0, -1, 0);
    planes[2] = Plane(-1, 0, 0, 1);
    normals[2] = Point(1, 0, 0);
    planes[3] = Plane(0, -1, 0, 1);
    normals[3] = Point(0, 1, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(70, 70);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Sphere");
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    init();
    glutMainLoop();
    return 0;
}