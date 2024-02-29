#include <bits/stdc++.h>
#include "bitmap_image.hpp"

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
#define epsilon .000001

class Ray;
class Objectt;
class Sphere;
class Triangle;
class GeneralObjectt;
class Floor;
class PointLight;
class SpotLight;
class Vector3D;

extern vector<Objectt *> objects;
extern vector<PointLight *> pointLightSources;
extern vector<SpotLight *> spotLightSources;
extern int recursionLevel;

class Vector3D
{
public:
    double x, y, z;

    Vector3D(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3D()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    Vector3D operator+(Vector3D v)
    {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }
    Vector3D operator-(Vector3D v)
    {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }
    Vector3D operator*(double d)
    {
        return Vector3D(x * d, y * d, z * d);
    }
    Vector3D operator/(double d)
    {
        return Vector3D(x / d, y / d, z / d);
    }
    double dot(Vector3D v)
    {
        return x * v.x + y * v.y + z * v.z;
    }
    Vector3D cross(Vector3D v)
    {
        return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    double length()
    {
        return sqrt(x * x + y * y + z * z);
    }
    Vector3D normalize()
    {
        return *this / length();
    }
};

class Ray
{
    Vector3D start, dir;

public:
    Ray(Vector3D start, Vector3D dir)
    {
        this->start = start;
        this->dir = dir;
    }
    Ray()
    {
        start = Vector3D(0, 0, 0);
        dir = Vector3D(0, 0, 0);
    }
    Vector3D getStart()
    {
        return start;
    }
    Vector3D getDir()
    {
        return dir;
    }
    Vector3D normalize()
    {
        return dir / dir.length();
    }
};

class PointLight
{
public:
    Vector3D position;
    double color[3];

public:
    PointLight()
    {
        position = Vector3D(0, 0, 0);
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
    }
    PointLight(Vector3D position, double color[])
    {
        this->position = position;
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
    }


    // destructor for point light
    ~PointLight()
    {
        // delete color;
        delete this;
    }



    void draw()
    {
        glPointSize(5);
        glBegin(GL_POINTS);
        {
            glColor3f(color[0], color[1], color[2]);
            glVertex3f(position.x, position.y, position.z);
        }
        glEnd();
    }
};

class SpotLight : public PointLight
{
public:
    Vector3D direction;
    double angle;

public:
    SpotLight(Vector3D position, Vector3D direction, double color[], double angle)
    {
        this->position = position;
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
        this->direction = direction;
        this->angle = angle;
    }

    // destructor for spot light
    ~SpotLight()
    {
        delete this;
    }

    void draw()
    {
        glPointSize(10);
        glBegin(GL_POINTS);
        {
            glColor3f(color[0], color[1], color[2]);
            glVertex3f(position.x, position.y, position.z);
        }
        glEnd();
    }
};

class Objectt
{
protected:
    string type;
    Vector3D reference_point;
    double length, width, height;
    double color[3];
    double co_efficients[4];
    int shine;

public:
    Objectt()
    {
        type = "";
        reference_point = Vector3D(0, 0, 0);
        height = 0;
        width = 0;
        length = 0;
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        co_efficients[0] = 0;
        co_efficients[1] = 0;
        co_efficients[2] = 0;
        co_efficients[3] = 0;
        shine = 0;
    }
    Objectt(Vector3D reference_point, double length, double width, double height, double color[], double co_efficients[], int shine)
    {
        type = "";

        this->reference_point = reference_point;
        this->height = height;
        this->width = width;
        this->length = length;
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
        this->co_efficients[0] = co_efficients[0];
        this->co_efficients[1] = co_efficients[1];
        this->co_efficients[2] = co_efficients[2];
        this->co_efficients[3] = co_efficients[3];
        this->shine = shine;
    }
    Objectt(Vector3D reference_point, double length, double width, double height)
    {
        type = "";

        this->reference_point = reference_point;
        this->height = height;
        this->width = width;
        this->length = length;
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        co_efficients[0] = 0;
        co_efficients[1] = 0;
        co_efficients[2] = 0;
        co_efficients[3] = 0;
        shine = 0;
    }

    // destructor for object
    virtual ~Objectt()
    {
        delete this;
    }

    virtual void draw()
    {
        cout << "Objectt Draw" << endl;
    }
    virtual double intersect(Ray r, double current_color[], int level)
    {
        cout << "Objectt Intersect" << endl;
        return 0;
    }
    void setColor(double color[])
    {
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
    }
    void setCoEfficients(double co_efficients[])
    {
        this->co_efficients[0] = co_efficients[0];
        this->co_efficients[1] = co_efficients[1];
        this->co_efficients[2] = co_efficients[2];
        this->co_efficients[3] = co_efficients[3];
    }
    void setShine(int shine)
    {
        this->shine = shine;
    }
    // return getcolor 1 array of 3 elements
    double *getColor()
    {
        return color;
    }
};

// sphere class is derived from Objectt class
class Sphere : public Objectt
{
public:
    Sphere(Vector3D center, double radius)
    {
        type = "Sphere";
        reference_point = center;
        length = radius;
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        co_efficients[0] = 0;
        co_efficients[1] = 0;
        co_efficients[2] = 0;
        co_efficients[3] = 0;
        shine = 0;
    }

    // destructor for sphere
    ~Sphere()
    {
        // delete color;
        // delete co_efficients;
        delete this;
    }

    void draw()
    {
        // draw a sphere center and radious
        glColor3f(color[0], color[1], color[2]);
        glPushMatrix();
        {
            glTranslatef(reference_point.x, reference_point.y, reference_point.z);
            glutSolidSphere(length, 100, 100);
        }
        glPopMatrix();
    }

    double intersect(Ray r, double current_color[], int level)
    {
        Vector3D start = r.getStart();
        Vector3D dir = r.getDir();
        Vector3D oc = start - reference_point;
        double a = dir.dot(dir);
        double b = 2 * oc.dot(dir);
        double c = oc.dot(oc) - length * length;
        double d = b * b - 4 * a * c;
        if (d < 0)
        {
            return -1;
        }
        d = sqrt(d);
        double t1 = (-b + d) / (2 * a);
        double t2 = (-b - d) / (2 * a);
        double t = min(t1, t2);
        if (t < 0)
        {
            t = max(t1, t2);
        }
        if (t < 0)
        {
            return -1;
        }
        if (level == 0)
        {
            return t;
        }
        return t;
    }
};

class Triangle : public Objectt
{
private:
    Vector3D a, b, c;

public:
    Triangle(Vector3D a, Vector3D b, Vector3D c)
    {
        type = "Triangle";
        this->a = a;
        this->b = b;
        this->c = c;
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        co_efficients[0] = 0;
        co_efficients[1] = 0;
        co_efficients[2] = 0;
        co_efficients[3] = 0;
        shine = 0;
    }
    // destructor for triangle
    ~Triangle()
    {
        // delete color;
        // delete co_efficients;
        delete this;
    }



    void draw()
    {
        // draw triangle
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
    }
    double intersect(Ray r, double current_color[], int level)
    {
        Vector3D start = r.getStart();
        Vector3D dir = r.getDir();
        Vector3D normal = (b - a).cross(c - a).normalize();
        double t = normal.dot(a - start) / normal.dot(dir);

        if (t < 0)
            return -1;

        Vector3D intersection = start + dir * t;
        Vector3D cross1 = (b - a).cross(intersection - a);
        Vector3D cross2 = (c - b).cross(intersection - b);
        Vector3D cross3 = (a - c).cross(intersection - c);

        if (normal.dot(cross1) > 0 && normal.dot(cross2) > 0 && normal.dot(cross3) > 0)
        {
            if (level == 0)
                return t;
        }

        return -1;
    }
};

class GeneralObjectt : public Objectt
{
private:
    double a, b, c, d, e, f, g, h, i, j;

public:
    GeneralObjectt(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, Vector3D ref, double length, double width, double height)
    {
        type = "GeneralObjectt";
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
        this->e = e;
        this->f = f;
        this->g = g;
        this->h = h;
        this->i = i;
        this->j = j;
        this->reference_point = ref;
        this->length = length;
        this->width = width;
        this->height = height;
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        co_efficients[0] = 0;
        co_efficients[1] = 0;
        co_efficients[2] = 0;
        co_efficients[3] = 0;
        shine = 0;
    }
    // destructor for general object
    ~GeneralObjectt()
    {
        // delete color;
        // delete co_efficients;
        delete this;
    }
    void draw()
    {
        // printf("General Objectt\n");
    }
    double intersect(Ray r, double current_color[], int level)
    {

        double A = a * r.getDir().x * r.getDir().x + b * r.getDir().y * r.getDir().y + c * r.getDir().z * r.getDir().z + d * r.getDir().x * r.getDir().y + e * r.getDir().y * r.getDir().z + f * r.getDir().z * r.getDir().x;
        double B = 2 * (a * r.getStart().x * r.getDir().x + b * r.getStart().y * r.getDir().y + c * r.getStart().z * r.getDir().z + d * r.getStart().x * r.getDir().y + e * r.getStart().y * r.getDir().z + f * r.getStart().z * r.getDir().x + g * r.getDir().x + h * r.getDir().y + i * r.getDir().z);
        double C = a * r.getStart().x * r.getStart().x + b * r.getStart().y * r.getStart().y + c * r.getStart().z * r.getStart().z + d * r.getStart().x * r.getStart().y + e * r.getStart().y * r.getStart().z + f * r.getStart().z * r.getStart().x + g * r.getStart().x + h * r.getStart().y + i * r.getStart().z + j;
        double D = B * B - 4 * A * C;
        if (D < 0)
        {
            return -1;
        }
        D = sqrt(D);
        double t1 = (-B + D) / (2 * A);
        double t2 = (-B - D) / (2 * A);
        double t = min(t1, t2);
        if (t < 0)
        {
            t = max(t1, t2);
        }
        if (t < 0)
        {
            return -1;
        }
        if (level == 0)
        {
            return t;
        }
        return t;
    }
};

class Floor : public Objectt
{
public:
    Floor(double floorWidth, double tileWidth)
    {
        reference_point = Vector3D(-floorWidth / 2, -floorWidth / 2, 0);
        length = floorWidth;
        width = tileWidth;
        height = 0;
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        co_efficients[0] = 0;
        co_efficients[1] = 0;
        co_efficients[2] = 0;
        co_efficients[3] = 0;
        shine = 0;
    }
    // destructor for floor
    ~Floor()
    {
        // delete color;
        // delete co_efficients;
        delete this;

    }
    void draw()
    {

        int tileCount = length / width;
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
                    glVertex3f(i * width, j * width, 0);
                    glVertex3f(i * width + width, j * width, 0);
                    glVertex3f(i * width + width, j * width + width, 0);
                    glVertex3f(i * width, j * width + width, 0);
                }
                glEnd();
            }
        }
    }
    double intersect(Ray r, double current_color[], int level)
    {
        Vector3D normal = Vector3D(0, 0, 1);
        double dotProduct = normal.dot(r.getDir());
        if (dotProduct == 0)
        {
            return -1;
        }
        double t = normal.dot(reference_point - r.getStart()) / dotProduct;
        Vector3D intersection = r.getStart() + r.getDir() * t;

        // if intersection is white tiles then return t otherwise -1
        if (intersection.x < reference_point.x || intersection.x > reference_point.x + length || intersection.y < reference_point.y || intersection.y > reference_point.y + length)
        {
            return -1;
        }
        if (level == 0)
        {
            return t;
        }
        int tileCount = length / width;
        int i = (intersection.x - reference_point.x) / width;
        int j = (intersection.y - reference_point.y) / width;
        if ((i + j) % 2 == 0)
        {
            return t;
        }
        else
        {
            return -1;
        }
        return t;
    }
};
