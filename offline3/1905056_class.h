#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream> // header in standard library

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

class Object
{
    private:
    string type;
    protected:
    Vector3D reference_point;
    double height, width, length;
    double color[3];
    double co_efficients[4];
    int shine;
    public:
    Object()
    {
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
    Object(double x, double y, double z, double height, double width, double length, double color[], double co_efficients[], int shine)
    {
        reference_point = Vector3D(x, y, z);
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
    virtual void draw(){
        cout << "Object Draw" << endl;
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
};

// sphere class is derived from Object class
class Sphere : public Object
{
public:
    Sphere(Vector3D center, double radius)
    {
        reference_point = center;
        length = radius;
    }
    Sphere (Vector3D center, double radius, double color[], double co_efficients[], int shine)
    {
        reference_point = center;
        length = radius;
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
        this->co_efficients[0] = co_efficients[0];
        this->co_efficients[1] = co_efficients[1];
        this->co_efficients[2] = co_efficients[2];
        this->co_efficients[3] = co_efficients[3];
        this->shine = shine;
    }
    void setCenter(Vector3D center)
    {
        reference_point = center;
    }
    void setRadius(double radius)
    {
        length = radius;
    }
    void setcolor(double color[])
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
    void draw()
    {
        printf("Sphere\n");
    }
};

class Triangle : public Object
{
    private:
    Vector3D a, b, c;
    public:
    Triangle(Vector3D a, Vector3D b, Vector3D c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }
    Triangle(Vector3D a, Vector3D b, Vector3D c, double color[], double co_efficients[], int shine)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
        this->co_efficients[0] = co_efficients[0];
        this->co_efficients[1] = co_efficients[1];
        this->co_efficients[2] = co_efficients[2];
        this->co_efficients[3] = co_efficients[3];
        this->shine = shine;
    }
    void setABC(Vector3D a, Vector3D b, Vector3D c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }
    void setcolor(double color[])
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
    void draw()
    {
        printf("Triangle\n");
    }
};

class GeneralObject : public Object
{
    private:
    double a, b, c, d, e, f, g, h, i, j;
    public:
    GeneralObject(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j)
    {
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
    }
    GeneralObject(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, double color[], double co_efficients[], int shine)
    {
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
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
        this->co_efficients[0] = co_efficients[0];
        this->co_efficients[1] = co_efficients[1];
        this->co_efficients[2] = co_efficients[2];
        this->co_efficients[3] = co_efficients[3];
        this->shine = shine;
    }
    void setcolor(double color[])
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
    void draw()
    {
        printf("General Object\n");
    }
};

class PointLight
{
    private:
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
    void draw()
    {
        printf("Point Light\n");
    }
};

class SpotLight
{
    private:
    PointLight pl;
    Vector3D direction;
    double angle;
    public:
    SpotLight(PointLight pl, Vector3D direction, double angle)
    {
        this->pl = pl;
        this->direction = direction;
        this->angle = angle;
    }
    SpotLight(Vector3D position, double color[], Vector3D direction, double angle)
    {
        this->pl = PointLight(position, color);
        this->direction = direction;
        this->angle = angle;
    }
   
    void draw()
    {
        printf("Spot Light\n");
    }
};
