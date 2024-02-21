#include <bits/stdc++.h>
using namespace std;

#include <GL/glut.h>
#define pi (2 * acos(0.0))

struct Point{
    double x, y;
    Point() {
        x = 0, y = 0;
    }
    Point(double a, double b) {
        x = a, y = b;
    }
};

vector<Point> points;

double theta = pi/180;

int idx;

vector<Point> getCirclePoints() {
    vector<Point> circlePoints(360 * 5);
    for (int i = 0; i < 1800; i++) {
        circlePoints[i] = Point(cosf(i * theta/5), sinf(i * theta/5));
    }
    return circlePoints;
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glEnable(GL_DEPTH_TEST);

    idx = 0;
    points = getCirclePoints();
}

void drawCircle() {
    glBegin(GL_LINES);{
        glColor3f(0, 0, 1);
        for (int i = 0; i < points.size(); i++) {
            glVertex2f(points[i].x, points[i].y);
            glVertex2f(points[(i + 1) % 1800].x, points[(i + 1) % 1800].y);
        }
    }glEnd();
}

void drawRadius() {
    glBegin(GL_LINES);{
        glColor3f(1, 1, 1);
        glVertex2f(0, 0);
        glVertex2f(points[idx % 1800].x, points[idx % 1800].y);
    }glEnd();
}

void drawLine() {
    glBegin(GL_LINES);{
        glColor3f(1, 1, 1);
        glVertex2f(points[idx % 1800].x, points[idx % 1800].y);
        glVertex2f(2, points[idx % 1800].y);
    }glEnd();
}

void drawSineCurve() {
    glBegin(GL_LINES);{
        for (int i = 0; i < 1500; i++) {
            glVertex2f(2 + i * theta/5, points[(idx + i) % 1800].y);
            glVertex2f(2 + (i + 1) * theta/5, points[(idx + i + 1) % 1800].y);
            // glVertex2f(i * theta/5, points[(idx + i) % 1800].y);
            // glVertex2f((i + 1) * theta/5, points[(idx + i + 1) % 1800].y);
        }
    }glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
    glLoadIdentity();               // Reset the model-view matrix
    glOrtho(-4, 4, -4, 4, -4, 4);
    drawCircle();
    drawRadius();
    glPointSize(5);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);{
        glVertex2f(points[idx % 1800].x, points[idx % 1800].y);
    }glEnd();
    drawLine();
    glPushMatrix(); {
        
        //glTranslatef(2, 0, 0);
        //glScalef(1, -1, 0);
        drawSineCurve();
    }glPopMatrix();

    glutSwapBuffers();
}

void animate() {
    idx++;
    cout << idx << '\n';
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(70, 70);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Simple Triangle");
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    init();
    glutMainLoop();

    return 0;
}