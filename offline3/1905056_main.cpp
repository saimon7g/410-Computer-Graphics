#include "1905056_class.h"

using namespace std;

#ifdef __linux__
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#elif __WIN32__
#include <windows.h>
#include <glut.h>
#endif

// global variables
Vector3D cameraPos(30, 70, 30);
Vector3D cameraLook(0, 0, 0);
Vector3D cameraUp(0, 0, 1);
Vector3D cameraRight(1, 0, 0);
double cameraAngle = 80;
int windowHeight = 800;
int windowWidth = 800;

int recursionLevel = 0;
int screenDimension = 0;
int numberOfObjects = 0;
int point_light_sources = 0;
int spotlight_source = 0;
vector<Objectt *> objects;
vector<PointLight *> pointLightSources;
vector<SpotLight *> spotLightSources;
Objectt *floorObj;

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
    windowHeight = screenDimension;
    windowWidth = screenDimension;
    file >> numberOfObjects;

    string object_type;
    for (int i = 0; i < numberOfObjects; i++)
    {
        file >> object_type;
        if (object_type == "triangle")
        {
            double x1, y1, z1;
            file >> x1 >> y1 >> z1;
            double x2, y2, z2;
            file >> x2 >> y2 >> z2;
            double x3, y3, z3;
            file >> x3 >> y3 >> z3;
            double colorR, colorG, colorB;
            file >> colorR >> colorG >> colorB;
            double ambient, diffuse, specular, recursive;
            file >> ambient >> diffuse >> specular >> recursive;
            int shininess;
            file >> shininess;
            Vector3D a = Vector3D(x1, y1, z1);
            Vector3D b = Vector3D(x2, y2, z2);
            Vector3D c = Vector3D(x3, y3, z3);
            double color[3] = {colorR, colorG, colorB};
            double co_efficients[4] = {ambient, diffuse, specular, recursive};
            Objectt *triangle = new Triangle(a, b, c);
            triangle->setColor(color);
            triangle->setCoEfficients(co_efficients);
            triangle->setShine(shininess);
            objects.push_back(triangle);

            printf("Triangle pointA %lf %lf %lf \n", x1, y1, z1);
            printf("Triangle pointB %lf %lf %lf \n", x2, y2, z2);
            printf("Triangle pointC %lf %lf %lf \n", x3, y3, z3);
            printf("Triangle color %lf %lf %lf \n", colorR, colorG, colorB);
            printf("Triangle ambient %lf %lf %lf %lf \n", ambient, diffuse, specular, recursive);
            printf("Triangle shininess %d \n", shininess);
        }
        else if (object_type == "sphere")
        {

            double centerX, centerY, centerZ;
            file >> centerX >> centerY >> centerZ;
            double radius;
            file >> radius;
            double colorR, colorG, colorB;
            file >> colorR >> colorG >> colorB;
            double ambient, diffuse, specular, recursive;
            file >> ambient >> diffuse >> specular >> recursive;
            int shininess;
            file >> shininess;
            Vector3D center = Vector3D(centerX, centerY, centerZ);
            double color[3] = {colorR, colorG, colorB};
            double co_efficients[4] = {ambient, diffuse, specular, recursive};
            Objectt *sphere = new Sphere(center, radius);
            sphere->setColor(color);
            sphere->setCoEfficients(co_efficients);
            sphere->setShine(shininess);
            objects.push_back(sphere);

            printf("Sphere center %lf %lf %lf \n", centerX, centerY, centerZ);
            printf("Sphere radius %lf \n ", radius);
            printf("Sphere color %lf %lf %lf \n", colorR, colorG, colorB);
            printf("Sphere ambient %lf %lf %lf %lf \n ", ambient, diffuse, specular, recursive);
            printf("Sphere shininess %d \n", shininess);
        }
        else if (object_type == "general")
        {
            double A, B, C, D, E, F, G, H, I, J;
            file >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
            double refX, refY, refZ, length, width, height;
            file >> refX >> refY >> refZ >> length >> width >> height;
            double colorR, colorG, colorB;
            file >> colorR >> colorG >> colorB;
            double ambient, diffuse, specular, recursive;
            file >> ambient >> diffuse >> specular >> recursive;
            int shininess;
            file >> shininess;
            double co_efficients[4] = {ambient, diffuse, specular, recursive};
            double color[3] = {colorR, colorG, colorB};
            Vector3D reference_point = Vector3D(refX, refY, refZ);
            Objectt *general = new GeneralObjectt(A, B, C, D, E, F, G, H, I, J, reference_point, length, width, height);
            general->setColor(color);
            general->setCoEfficients(co_efficients);
            general->setShine(shininess);
            objects.push_back(general);

            printf("General A B C D E %lf %lf %lf %lf %lf \n", A, B, C, D, E);
            printf("General F G H I J %lf %lf %lf %lf %lf \n", F, G, H, I, J);
            printf("General refX refY refZ %lf %lf %lf \n", refX, refY, refZ);
            printf("General length width height %lf %lf %lf \n", length, width, height);
            printf("General color %lf %lf %lf \n", colorR, colorG, colorB);
            printf("General ambient %lf %lf %lf %lf \n ", ambient, diffuse, specular, recursive);
            printf("General shininess %d \n", shininess);
        }
    }
    file >> point_light_sources;
    for (int i = 0; i < point_light_sources; i++)
    {
        double positionX, positionY, positionZ;
        file >> positionX >> positionY >> positionZ;
        double colorR, colorG, colorB;
        file >> colorR >> colorG >> colorB;
        Vector3D position = Vector3D(positionX, positionY, positionZ);
        double color[3] = {colorR, colorG, colorB};
        pointLightSources.push_back(new PointLight(position, color));

        printf("Point Light Source %lf %lf %lf\n", positionX, positionY, positionZ);
        printf("Point Light Source Color %lf %lf %lf\n", colorR, colorG, colorB);
    }
    file >> spotlight_source;
    for (int i = 0; i < spotlight_source; i++)
    {
        double positionX, positionY, positionZ;
        file >> positionX >> positionY >> positionZ;
        double colorR, colorG, colorB;
        file >> colorR >> colorG >> colorB;
        double directionX, directionY, directionZ;
        file >> directionX >> directionY >> directionZ;
        double cutoff_angle;
        file >> cutoff_angle;

        Vector3D position = Vector3D(positionX, positionY, positionZ);
        Vector3D direction = Vector3D(directionX, directionY, directionZ);
        double color[3] = {colorR, colorG, colorB};
        spotLightSources.push_back(new SpotLight(position, direction, color, cutoff_angle));

        printf("Spotlight Source %lf %lf %lf \n", positionX, positionY, positionZ);
        printf("Spotlight Source Color %lf %lf %lf \n", colorR, colorG, colorB);
        printf("Spotlight Source Direction %lf %lf %lf \n", directionX, directionY, directionZ);
        printf("Spotlight Source Cutoff Angle %lf \n", cutoff_angle);
    }
    printf("Recursion Level %d\n", recursionLevel);
    printf("Screen Dimension %d\n", windowHeight);
    printf("Number of Objects %d\n", numberOfObjects);
    printf("Point Light Sources %d\n", point_light_sources);
    printf("Spotlight Source %d\n", spotlight_source);
    printf("Data Loaded\n");
    file.close();
}

void init()
{

    // initialize all variables
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(cameraAngle, 1, 1, 1000);
    cameraLook.x = (cameraRight.y * cameraUp.z) - (cameraUp.y * cameraRight.z);
    cameraLook.y = (cameraRight.z * cameraUp.x) - (cameraUp.z * cameraRight.x);
    cameraLook.z = (cameraRight.x * cameraUp.y) - (cameraUp.x * cameraRight.y);
    floorObj = new Floor(1000, 20);
}

void drawaxesAndFloor()
{
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-50, 0, 0);
        glVertex3f(50, 0, 0);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0, -50, 0);
        glVertex3f(0, 50, 0);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0, 0, -50);
        glVertex3f(0, 0, 50);
    }
    glEnd();
    floorObj->draw();
}

void capture()
{
    int imageWidth = windowWidth;
    int imageHeight = windowHeight;
    bitmap_image image(imageWidth, imageWidth);
    double planeDistance = (windowHeight / 2.0) / tan((cameraAngle / 2.0) * (pi / 180));
    double x = cameraPos.x + (cameraLook.x * planeDistance) - (cameraRight.x * windowWidth) / 2 + (cameraUp.x * windowHeight) / 2;
    double y = cameraPos.y + (cameraLook.y * planeDistance) - (cameraRight.y * windowWidth) / 2 + (cameraUp.y * windowHeight) / 2;
    double z = cameraPos.z + (cameraLook.z * planeDistance) - (cameraRight.z * windowWidth) / 2 + (cameraUp.z * windowHeight) / 2;
    Vector3D topleft = Vector3D(x, y, z);
    double du = (double)(windowWidth * 1.0) / (imageWidth * 1.0);
    double dv = (double)(windowHeight * 1.0) / (imageHeight * 1.0);
    topleft = topleft + cameraRight * (0.5 * du) - cameraUp * (0.5 * dv);

    int nearest;
    double t, tMin;
    for (int i = 0; i < imageHeight; i++)
    {
        for (int j = 0; j < imageWidth; j++)
        {
            image.set_pixel(i, j, 0, 0, 0);
            Vector3D curPixel = topleft + (cameraRight * i * du) - (cameraUp * j * dv);
            Vector3D rayDir = curPixel - cameraPos;
            Ray ray = Ray(cameraPos, rayDir);
            double *color = new double[3];
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
            nearest = -1;
            tMin = INT16_MAX;
            for (int k = 0; k < objects.size(); k++)
            {
                t = objects[k]->intersect(ray, color, 0);
                if (t > 0 && t < tMin)
                {
                    tMin = t;
                    nearest = k;
                }
            }
            t = floorObj->intersect(ray, color, 0);
            if (t > 0 && t < tMin)
            {
                tMin = t;
                nearest = -2;
            }

            if (nearest == -2)
            {
                double temp=floorObj->intersect(ray, color, 1);
                if(temp>0){
                    image.set_pixel(i, j, 255, 255, 255);
                }
                else {
                    image.set_pixel(i, j, 0, 0, 0);
                }
            }
            else if (nearest != -1 && nearest != -2)
            {
                tMin = objects[nearest]->intersect(ray, color, 1);
                double *color = objects[nearest]->getColor();
                image.set_pixel(i, j, color[0] * 255, color[1] * 255, color[2] * 255);
            }

            delete[] color;
        }
    }

    image.save_image("output.bmp");
    printf("Image Captured\n");
}
void clearMemory()
{
    for (int i = 0; i < objects.size(); i++)
    {
        delete objects[i];
    }
    for (int i = 0; i < pointLightSources.size(); i++)
    {
        delete pointLightSources[i];
    }
    for (int i = 0; i < spotLightSources.size(); i++)
    {
        delete spotLightSources[i];
    }
    delete floorObj;
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

    drawaxesAndFloor();
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i]->draw();
    }
    for (int i = 0; i < pointLightSources.size(); i++)
    {
        pointLightSources[i]->draw();
    }
    for (int i = 0; i < spotLightSources.size(); i++)
    {
        spotLightSources[i]->draw();
    }
    glutSwapBuffers();
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
        // printf("1 pressed\n");
        cameraRight.x = (cameraRight.x * cos(-rotation_angle)) + (cameraLook.x * sin(-rotation_angle));
        cameraRight.y = (cameraRight.y * cos(-rotation_angle)) + (cameraLook.y * sin(-rotation_angle));
        cameraRight.z = (cameraRight.z * cos(-rotation_angle)) + (cameraLook.z * sin(-rotation_angle));

        cameraLook.x = (cameraLook.x * cos(-rotation_angle)) - (cameraRight.x * sin(-rotation_angle));
        cameraLook.y = (cameraLook.y * cos(-rotation_angle)) - (cameraRight.y * sin(-rotation_angle));
        cameraLook.z = (cameraLook.z * cos(-rotation_angle)) - (cameraRight.z * sin(-rotation_angle));
        break;
    case '2':
        // printf("2 pressed\n");
        cameraRight.x = (cameraRight.x * cos(rotation_angle)) + (cameraLook.x * sin(rotation_angle));
        cameraRight.y = (cameraRight.y * cos(rotation_angle)) + (cameraLook.y * sin(rotation_angle));
        cameraRight.z = (cameraRight.z * cos(rotation_angle)) + (cameraLook.z * sin(rotation_angle));

        cameraLook.x = (cameraLook.x * cos(rotation_angle)) - (cameraRight.x * sin(rotation_angle));
        cameraLook.y = (cameraLook.y * cos(rotation_angle)) - (cameraRight.y * sin(rotation_angle));
        cameraLook.z = (cameraLook.z * cos(rotation_angle)) - (cameraRight.z * sin(rotation_angle));
        break;

    case '3':
        // printf("3 pressed\n");
        cameraLook.x = (cameraLook.x * cos(rotation_angle)) + (cameraUp.x * sin(rotation_angle));
        cameraLook.y = (cameraLook.y * cos(rotation_angle)) + (cameraUp.y * sin(rotation_angle));
        cameraLook.z = (cameraLook.z * cos(rotation_angle)) + (cameraUp.z * sin(rotation_angle));

        cameraUp.x = (cameraUp.x * cos(rotation_angle)) - (cameraLook.x * sin(rotation_angle));
        cameraUp.y = (cameraUp.y * cos(rotation_angle)) - (cameraLook.y * sin(rotation_angle));
        cameraUp.z = (cameraUp.z * cos(rotation_angle)) - (cameraLook.z * sin(rotation_angle));
        break;
    case '4':
        // printf("4 pressed\n");
        cameraLook.x = (cameraLook.x * cos(-rotation_angle)) + (cameraUp.x * sin(-rotation_angle));
        cameraLook.y = (cameraLook.y * cos(-rotation_angle)) + (cameraUp.y * sin(-rotation_angle));
        cameraLook.z = (cameraLook.z * cos(-rotation_angle)) + (cameraUp.z * sin(-rotation_angle));

        cameraUp.x = (cameraUp.x * cos(-rotation_angle)) - (cameraLook.x * sin(-rotation_angle));
        cameraUp.y = (cameraUp.y * cos(-rotation_angle)) - (cameraLook.y * sin(-rotation_angle));
        cameraUp.z = (cameraUp.z * cos(-rotation_angle)) - (cameraLook.z * sin(-rotation_angle));
        break;
    case '5':
        // printf("5 pressed\n");
        cameraUp.x = (cameraUp.x * cos(rotation_angle)) + (cameraRight.x * sin(rotation_angle));
        cameraUp.y = (cameraUp.y * cos(rotation_angle)) + (cameraRight.y * sin(rotation_angle));
        cameraUp.z = (cameraUp.z * cos(rotation_angle)) + (cameraRight.z * sin(rotation_angle));

        cameraRight.x = (cameraRight.x * cos(rotation_angle)) - (cameraUp.x * sin(rotation_angle));
        cameraRight.y = (cameraRight.y * cos(rotation_angle)) - (cameraUp.y * sin(rotation_angle));
        cameraRight.z = (cameraRight.z * cos(rotation_angle)) - (cameraUp.z * sin(rotation_angle));
        break;
    case '6':
        // printf("6 pressed\n");
        cameraUp.x = (cameraUp.x * cos(-rotation_angle)) + (cameraRight.x * sin(-rotation_angle));
        cameraUp.y = (cameraUp.y * cos(-rotation_angle)) + (cameraRight.y * sin(-rotation_angle));
        cameraUp.z = (cameraUp.z * cos(-rotation_angle)) + (cameraRight.z * sin(-rotation_angle));

        cameraRight.x = (cameraRight.x * cos(-rotation_angle)) - (cameraUp.x * sin(-rotation_angle));
        cameraRight.y = (cameraRight.y * cos(-rotation_angle)) - (cameraUp.y * sin(-rotation_angle));
        cameraRight.z = (cameraRight.z * cos(-rotation_angle)) - (cameraUp.z * sin(-rotation_angle));
        break;
    case '0':
        // printf("0 pressed\n");
        capture();
        break;
    case 'q':
        // printf("q pressed\n");
        clearMemory();
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
        // printf("UP pressed");
        cameraPos.x += cameraLook.x;
        cameraPos.y += cameraLook.y;
        cameraPos.z += cameraLook.z;
        break;
    case GLUT_KEY_DOWN:
        // printf("DOWN pressed");
        cameraPos.x -= cameraLook.x;
        cameraPos.y -= cameraLook.y;
        cameraPos.z -= cameraLook.z;
        break;
    case GLUT_KEY_LEFT:
        // printf("LEFT pressed");
        cameraPos.y += cameraRight.y;
        cameraPos.x += cameraRight.x;
        cameraPos.z += cameraRight.z;
        break;
    case GLUT_KEY_RIGHT:
        // printf("RIGHT pressed");
        cameraPos.x -= cameraRight.x;
        cameraPos.y -= cameraRight.y;
        cameraPos.z -= cameraRight.z;
        break;
    case GLUT_KEY_PAGE_DOWN:
        // printf("PAGE_DOWN pressed");
        cameraPos.x -= cameraUp.x;
        cameraPos.y -= cameraUp.y;
        cameraPos.z -= cameraUp.z;
        break;
    case GLUT_KEY_PAGE_UP:
        // printf("PAGE_UP pressed");
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
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(200, 200);
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