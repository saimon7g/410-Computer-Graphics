#include <GL/glut.h>
#include <cmath>

const float sunRadius = 5.0f;
const float mercuryRadius = 1.0f;
const float venusRadius = 1.5f;
const float earthRadius = 2.0f;
const float moonRadius = 0.5f;

float mercuryAngle = 0.0f;
float venusAngle = 0.0f;
float earthAngle = 0.0f;
float moonAngle = 0.0f;

void drawSphere(float radius, GLfloat red, GLfloat green, GLfloat blue) {
    glColor3f(red, green, blue);
    glutSolidSphere(radius, 50, 50);
}

void drawOrbit(float radius) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159265358979323846 / 180.0;
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
}

void drawSolarSystem() {
    // Draw Sun
    drawSphere(sunRadius, 1.0f, 1.0f, 0.0f);

    // Draw orbits
    glColor3f(0.5f, 0.5f, 0.5f);
    drawOrbit(10.0f);  // Orbit of Mercury
    drawOrbit(15.0f);  // Orbit of Venus
    drawOrbit(20.0f);  // Orbit of Earth

    // Draw Mercury
    glPushMatrix();
    glRotatef(mercuryAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(10.0f, 0.0f, 0.0f);
    drawSphere(mercuryRadius, 0.5f, 0.5f, 0.5f);
    glPopMatrix();

    // Draw Venus
    glPushMatrix();
    glRotatef(venusAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(15.0f, 0.0f, 0.0f);
    drawSphere(venusRadius, 0.8f, 0.5f, 0.2f);
    glPopMatrix();

    // Draw Earth
    glPushMatrix();
    glRotatef(earthAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(20.0f, 0.0f, 0.0f);
    drawSphere(earthRadius, 0.0f, 0.0f, 1.0f);

    // Draw Moon (rotate around Earth)
    glRotatef(moonAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(5.0f, 0.0f, 0.0f);
    drawSphere(moonRadius, 0.7f, 0.7f, 0.7f);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 40, 0, 0, 0, 0, 1, 0);  // Adjust the viewing position

    drawSolarSystem();

    glutSwapBuffers();
}

void update(int value) {
    mercuryAngle += 1.0f;
    venusAngle += 0.8f;
    earthAngle += 0.5f;
    moonAngle += 2.0f;

    if (mercuryAngle > 360.0f) mercuryAngle -= 360.0f;
    if (venusAngle > 360.0f) venusAngle -= 360.0f;
    if (earthAngle > 360.0f) earthAngle -= 360.0f;
    if (moonAngle > 360.0f) moonAngle -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Solar System");
    glutInitWindowSize(800, 600);

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutMainLoop();

    return 0;
}
