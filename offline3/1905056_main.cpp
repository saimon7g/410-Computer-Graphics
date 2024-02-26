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
int point_light_sources = 0;
int spotlight_source = 0;

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
    for (int i = 0; i < numberOfObjects; i++)
    {
        file >> object_type;
        if (object_type == "triangle")
        {
            //             triangle
            // -20.0 -20.0 0.0	- x1, y1, z1
            // 20.0 -20.0 0.0	- x2, y2, z2
            // 0.0 0.0 20.0	- x3, y3, z3
            // 1.0 0.0 0.0	- color
            // 0.4 0.2 0.1 0.3	- ambient, diffuse, specular, recursive reflection coefficient
            // 5		- shininess
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
            printf("Triangle pointA %lf %lf %lf \n", x1, y1, z1);
            printf("Triangle pointB %lf %lf %lf \n", x2, y2, z2);
            printf("Triangle pointC %lf %lf %lf \n", x3, y3, z3);
            printf("Triangle color %lf %lf %lf \n", colorR, colorG, colorB);
            printf("Triangle ambient %lf %lf %lf %lf \n", ambient, diffuse, specular, recursive);
            printf("Triangle shininess %d \n", shininess);

        }
        else if (object_type == "sphere")
        {
            // sphere
            // 40.0 0.0 10.0	- center
            // 10.0		- radius
            // 0.0 1.0 0.0	- color
            // 0.4 0.2 0.2 0.2	- ambient, diffuse, specular, recursive reflection coefficient
            // 5		- shininess

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
            printf("Sphere center %lf %lf %lf \n", centerX, centerY, centerZ);
            printf("Sphere radius %lf \n ", radius);
            printf("Sphere color %lf %lf %lf \n", colorR, colorG, colorB);
            printf("Sphere ambient %lf %lf %lf %lf \n ", ambient, diffuse, specular, recursive);
            printf("Sphere shininess %d \n", shininess);

        }
        else if (object_type == "general")
        {
            // general
            // 1 1 1 0 0 0 -20 -20 -20 200	- A B C D E F G H I J
            // 0 0 0 0 0 5	- cube reference point, length, width, height (0 indicates no clipping along this dimension)
            // 0.0 0.0 1.0	- color
            // 0.4 0.2 0.1 0.3	- ambient, diffuse, specular, recursive reflection coefficient
            // 3		- shininess
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
        //         4 point light sources
        // 70.0 70.0 70.0	- position of the 1st point light source
        // 1.0 0.0 0.0	- color of the 1st point light source
        // -70 70 70	- position of the 2nd point light source
        // 0.0 0.0 1.0	- color of the 2nd point light source
        // 70 -70 70	- position of the 3rd point light source
        // 1 0 0.0		- color of the 3rd point light source
        // -70 -70 70	- position of the 4th point light source
        // 0 1.0 0		- color of the 4th point light source

        double positionX, positionY, positionZ;
        file >> positionX >> positionY >> positionZ;
        double colorR, colorG, colorB;
        file >> colorR >> colorG >> colorB;
        printf("Point Light Source %lf %lf %lf %lf %lf %lf \n ", positionX, positionY, positionZ, colorR, colorG, colorB);
    }
    //     1 spotlight source
    // 100 100 -200	- position of the 1st spotlight source
    // 0 1.0 0.0	- color of the 1st spotlight source
    // 0 0.0 1		- direction of the 1st spotlight source
    // 12		- cutoff angle (in degree) of the 1st spotlight source
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
        printf("Spotlight Source %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf \n " , positionX, positionY, positionZ, colorR, colorG, colorB, directionX, directionY, directionZ, cutoff_angle);
    }


    printf("Recursion Level %d\n", recursionLevel);
    printf("Screen Dimension %d\n", screenDimension);
    printf("Number of Objects %d\n", numberOfObjects);
    printf("Point Light Sources %d\n", point_light_sources);
    printf("Spotlight Source %d\n", spotlight_source);
    printf("Data Loaded\n");

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
void floor()
{

    //     There will be a floor along the XY-plane
    // FloorWidth can be 1000 (from origin 500 across each side)
    // Each Tile Width can be 20
    // Color should be alternating

    int floorWidth = 1000;
    int tileWidth = 20;
    int tileCount = floorWidth / tileWidth;
    for (int i = -tileCount / 2; i < tileCount / 2; i++)
    {
        for (int j = -tileCount / 2; j < tileCount / 2; j++)
        {
            if ((i + j) % 2 == 0)
            {
                glColor3f(1, 1, 1);
            }
            else
            {
                glColor3f(0, 0, 0);
            }
            glBegin(GL_QUADS);
            {
                glVertex3f(i * tileWidth, j * tileWidth, 0);
                glVertex3f(i * tileWidth + tileWidth, j * tileWidth, 0);
                glVertex3f(i * tileWidth + tileWidth, j * tileWidth + tileWidth, 0);
                glVertex3f(i * tileWidth, j * tileWidth + tileWidth, 0);
            }
            glEnd();
        }
    }
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
    floor();
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

    // initialize all variables

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
    case 'q':
        // printf("q pressed\n");
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
    glutInitWindowSize(900, 900);
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