#include <bits/stdc++.h>
using namespace std;

ifstream input1;

struct Point
{
    double x, y, z, n;
    Point(double x, double y, double z) : x(x), y(y), z(z), n(1) {}
    Point(double x, double y, double z, double n) : x(x), y(y), z(z), n(n) {}
    Point() {}
};

struct Matrix
{
    double mat[4][4];
    Matrix()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                mat[i][j] = 0;
            }
        }
    }
};

Point eye, look, up;
double fovY, aspectRatio, near, far;

void readSceneFile();
void stage1();
void stage2();
void stage3();
void stage4();
Matrix getIdentityMatrix();
Point matrixMultiplyPoint(Matrix m, Point p);
Matrix metrixMultiplyMatrix(Matrix m1, Matrix m2);
Matrix getTranslationMatrix(Point p);
Matrix getScaleMatrix(Point p);
Matrix getRotationMatrix(Point p, double angle);
Point crossProduct(Point p1, Point p2);
double dotProduct(Point p1, Point p2);
Point normalizedVector(Point p);
Point scalarMultiply(Point p, double s);
Point rodriguesFormula(Point p, Point a, double angle);

int main()
{
    cout << "hello" << endl;
    string filepath = "IOs/1/scene.txt";
    input1.open(filepath);
    if (!input1.is_open())
    {
        cout << "error" << endl;
        return 0;
    }
    readSceneFile();
    stage1();
    input1.close();
    cout << "stage 1 done" << endl;
    stage2();
    cout << "stage 2 done" << endl;
    stage3();
    cout << "stage 3 done" << endl;
}

void readSceneFile()
{
    double x, y, z;
    input1 >> x >> y >> z;
    eye = Point(x, y, z);
    input1 >> x >> y >> z;
    look = Point(x, y, z);
    input1 >> x >> y >> z;
    up = Point(x, y, z);
    input1 >> fovY >> aspectRatio >> near >> far;
    return;
}
Matrix getIdentityMatrix()
{
    Matrix m;
    for (int i = 0; i < 4; i++)
    {
        m.mat[i][i] = 1;
    }
    return m;
}
Point matrixMultiplyPoint(Matrix m, Point p)
{
    Point newPoint;
    newPoint.x = m.mat[0][0] * p.x + m.mat[0][1] * p.y + m.mat[0][2] * p.z + m.mat[0][3] * p.n;
    newPoint.y = m.mat[1][0] * p.x + m.mat[1][1] * p.y + m.mat[1][2] * p.z + m.mat[1][3] * p.n;
    newPoint.z = m.mat[2][0] * p.x + m.mat[2][1] * p.y + m.mat[2][2] * p.z + m.mat[2][3] * p.n;
    newPoint.n = m.mat[3][0] * p.x + m.mat[3][1] * p.y + m.mat[3][2] * p.z + m.mat[3][3] * p.n;
    return newPoint;
}

Matrix metrixMultiplyMatrix(Matrix m1, Matrix m2)
{
    Matrix m;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m.mat[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                m.mat[i][j] += m1.mat[i][k] * m2.mat[k][j];
            }
        }
    }
    return m;
}
Matrix getTranslationMatrix(Point p)
{
    Matrix m = getIdentityMatrix();
    m.mat[0][3] = p.x;
    m.mat[1][3] = p.y;
    m.mat[2][3] = p.z;
    return m;
}
Matrix getScaleMatrix(Point p)
{
    Matrix m = getIdentityMatrix();
    m.mat[0][0] = p.x;
    m.mat[1][1] = p.y;
    m.mat[2][2] = p.z;
    return m;
}
Matrix getRotationMatrix(Point p, double angle)
{
    Matrix m = getIdentityMatrix();
    Point a = normalizedVector(p);
    double angleRadians = angle * M_PI / 180;
    Point c1 = rodriguesFormula(Point(1, 0, 0), a, angleRadians);
    Point c2 = rodriguesFormula(Point(0, 1, 0), a, angleRadians);
    Point c3 = rodriguesFormula(Point(0, 0, 1), a, angleRadians);
    m.mat[0][0] = c1.x;
    m.mat[0][1] = c2.x;
    m.mat[0][2] = c3.x;
    m.mat[1][0] = c1.y;
    m.mat[1][1] = c2.y;
    m.mat[1][2] = c3.y;
    m.mat[2][0] = c1.z;
    m.mat[2][1] = c2.z;
    m.mat[2][2] = c3.z;
    return m;
}
Point crossProduct(Point p1, Point p2)
{
    Point p;
    p.x = p1.y * p2.z - p1.z * p2.y;
    p.y = p1.z * p2.x - p1.x * p2.z;
    p.z = p1.x * p2.y - p1.y * p2.x;
    return p;
}
double dotProduct(Point p1, Point p2)
{
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}
Point normalizedVector(Point p)
{
    double length = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
    Point newPoint;
    newPoint.x = p.x / length;
    newPoint.y = p.y / length;
    newPoint.z = p.z / length;
    return newPoint;
}
Point scalarMultiply(Point p, double s)
{
    Point newPoint;
    newPoint.x = p.x * s;
    newPoint.y = p.y * s;
    newPoint.z = p.z * s;
    return newPoint;
}

Point rodriguesFormula(Point x, Point a, double angleRadians)
{
    Point v1 = scalarMultiply(x, cos(angleRadians));
    Point v2 = scalarMultiply(crossProduct(a, x), sin(angleRadians));
    Point v3 = scalarMultiply(a, dotProduct(a, x) * (1 - cos(angleRadians)));
    Point v = Point(v1.x + v2.x + v3.x, v1.y + v2.y + v3.y, v1.z + v2.z + v3.z);
    return v;
}

void stage1()
{
    ofstream output("stage1.txt");
    output<<fixed<<setprecision(7);
    string command;
    stack<Matrix> S;
    Matrix M = getIdentityMatrix();
    S.push(M);

    while (1)
    {
        input1 >> command;
        if (command == "end")
        {
            output.close();
            return;
        }
        else if (command == "triangle")
        {
            double x, y, z;
            input1 >> x >> y >> z;
            Point p1(x, y, z);
            input1 >> x >> y >> z;
            Point p2(x, y, z);
            input1 >> x >> y >> z;
            Point p3(x, y, z);

            Matrix m = S.top();
            Point p1new, p2new, p3new;
            p1new = matrixMultiplyPoint(m, p1);
            p2new = matrixMultiplyPoint(m, p2);
            p3new = matrixMultiplyPoint(m, p3);

            output << p1new.x << " " << p1new.y << " " << p1new.z << endl;
            output << p2new.x << " " << p2new.y << " " << p2new.z << endl;
            output << p3new.x << " " << p3new.y << " " << p3new.z << endl
                    << endl;
        }
        else if (command == "translate")
        {
            double x, y, z;
            input1 >> x >> y >> z;
            Matrix m1 = getTranslationMatrix(Point(x, y, z));
            Matrix m2 = S.top();
            Matrix m3 = metrixMultiplyMatrix(m2, m1);
            S.pop();
            S.push(m3);
        }
        else if (command == "scale")
        {
            double x, y, z;
            input1 >> x >> y >> z;
            Matrix m1 = getScaleMatrix(Point(x, y, z));
            Matrix m2 = S.top();
            Matrix m3 = metrixMultiplyMatrix(m2, m1);
            S.pop();
            S.push(m3);
        }
        else if (command == "rotate")
        {
            double x, y, z, angle;
            input1 >> angle >> x >> y >> z;
            Matrix m1 = getRotationMatrix(Point(x, y, z), angle);
            Matrix m2 = S.top();
            Matrix m3 = metrixMultiplyMatrix(m2, m1);
            S.pop();
            S.push(m3);
        }
        else if (command == "push")
        {
            Matrix m = S.top();
            S.push(m);
        }
        else if (command == "pop")
        {
            if (S.size() == 1)
            {
                cout << "error" << endl;
                return;
            }
            S.pop();
        }
        else
        {
            cout << "error" << endl;
            return;
        }
    }
    return;
}
void stage2()
{
    ifstream input2("stage1.txt");
    ofstream output("stage2.txt");
    output<<fixed<<setprecision(7);
    Point l = Point(look.x - eye.x, look.y - eye.y, look.z - eye.z);
    l = normalizedVector(l);
    Point r = crossProduct(l, up);
    r = normalizedVector(r);
    Point u = crossProduct(r, l);
    Matrix T = getIdentityMatrix();
    T.mat[0][3] = -eye.x;
    T.mat[1][3] = -eye.y;
    T.mat[2][3] = -eye.z;
    Matrix R = getIdentityMatrix();
    R.mat[0][0] = r.x;
    R.mat[0][1] = r.y;
    R.mat[0][2] = r.z;
    R.mat[1][0] = u.x;
    R.mat[1][1] = u.y;
    R.mat[1][2] = u.z;
    R.mat[2][0] = -l.x;
    R.mat[2][1] = -l.y;
    R.mat[2][2] = -l.z;
    Matrix V = metrixMultiplyMatrix(R, T);
    int linecount = 0;
    while (1)
    {
        double x, y, z;
        input2 >> x >> y >> z;
        if (input2.eof())
        {
            output.close();
            input2.close();
            return;
        }
        Point p = Point(x, y, z);
        Point pnew = matrixMultiplyPoint(V, p);
        output << pnew.x << " " << pnew.y << " " << pnew.z << endl;
        linecount++;
        if (linecount == 3)
        {
            output << endl;
            linecount = 0;
        }
    }
    return;
}

void stage3()
{
    ifstream input3("stage2.txt");
    ofstream output("stage3.txt");
    output<<fixed<<setprecision(7);
    double fovX = fovY * aspectRatio;
    double theta_t = fovY* M_PI / 360;
    double theta_r = fovX* M_PI / 360;
    double t = near * tan(theta_t);
    double r = near * tan(theta_r);


    Matrix P = getIdentityMatrix();
    P.mat[0][0] = near / r;
    P.mat[1][1] = near / t;
    P.mat[2][2] = -(far + near) / (far - near);
    P.mat[2][3] = -(2 * far * near) / (far - near);
    P.mat[3][2] = -1;
    P.mat[3][3] = 0;
    int linecount = 0;
    while (1)
    {
        double x, y, z;
        input3 >> x >> y >> z;
        if (input3.eof())
        {
            output.close();
            input3.close();
            return;
        }
        Point p = Point(x, y, z);
        Point pnew = matrixMultiplyPoint(P, p);
        double w = pnew.n;
        output << pnew.x / w << " " << pnew.y / w << " " << pnew.z / w << endl;
        linecount++;
        if (linecount == 3)
        {
            output << endl;
            linecount = 0;
        }
    }
    return;
}
