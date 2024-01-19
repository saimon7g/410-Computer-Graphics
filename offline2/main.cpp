#include <bits/stdc++.h>
#include "bitmap_image.hpp"
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
vector<Point> rearrangrTrianglePoints(vector<Point> triangle);
static unsigned long int g_seed = 1;
inline int randNum()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

int main()
{
    cout << "hello" << endl;
    string filepath = "TestCase/1/scene.txt";
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
    stage4();
    cout << "stage 4 done" << endl;
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
vector<Point> rearrangrTrianglePoints(vector<Point> triangle)
{
    vector<Point> newTriangle;
    Point p1 = triangle[0];
    Point p2 = triangle[1];
    Point p3 = triangle[2];
    Point topmost;
    Point middle;
    Point bottommost;
    if (p1.y >= p2.y && p1.y >= p3.y)
    {
        topmost = p1;
        if (p2.y >= p3.y)
        {
            middle = p2;
            bottommost = p3;
        }
        else
        {
            middle = p3;
            bottommost = p2;
        }
    }
    else if (p2.y >= p1.y && p2.y >= p3.y)
    {
        topmost = p2;
        if (p1.y >= p3.y)
        {
            middle = p1;
            bottommost = p3;
        }
        else
        {
            middle = p3;
            bottommost = p1;
        }
    }
    else
    {
        topmost = p3;
        if (p1.y >= p2.y)
        {
            middle = p1;
            bottommost = p2;
        }
        else
        {
            middle = p2;
            bottommost = p1;
        }
    }

    newTriangle.push_back(topmost);
    newTriangle.push_back(middle);
    newTriangle.push_back(bottommost);
    return newTriangle;
}
void stage1()
{
    ofstream output("stage1.txt");
    output << fixed << setprecision(7);
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
    output << fixed << setprecision(7);
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
    output << fixed << setprecision(7);
    double fovX = fovY * aspectRatio;
    double theta_t = fovY * M_PI / 360;
    double theta_r = fovX * M_PI / 360;
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
void stage4()
{
    ifstream input4("stage3.txt");
    ifstream input5("TestCase/1/config.txt");
    ofstream output("z_buffer.txt");
    output << fixed << setprecision(7);
    int screenHeight, screenWidth;
    input5 >> screenWidth >> screenHeight;
    // cout<<screenWidth<<" "<<screenHeight<<endl;
    input5.close();

    double RightLimit = 1.0;
    double LeftLimit = -1.0;
    double TopLimit = 1.0;
    double BottomLimit = -1.0;
    double zMax = 1.0;
    double zMin = -1.0;

    double dx = (RightLimit - LeftLimit) / screenWidth;
    double dy = (TopLimit - BottomLimit) / screenHeight;
    double topY = TopLimit - dy / 2.0;
    double bottomY = BottomLimit + dy / 2.0;
    double leftX = LeftLimit + dx / 2.0;
    double rightX = RightLimit - dx / 2.0;

    cout << "dx: " << dx << " dy: " << dy << endl;

    double **zbuffer = new double *[screenHeight];
    for (int i = 0; i < screenHeight; i++)
    {
        zbuffer[i] = new double[screenWidth];
        for (int j = 0; j < screenWidth; j++)
        {
            zbuffer[i][j] = zMax;
        }
    }
    bitmap_image image(screenWidth, screenHeight);
    image.set_all_channels(0, 0, 0);

    vector<vector<Point>> triangles;

    while (1)
    {
        double x1, y1, z1, x2, y2, z2, x3, y3, z3;
        if (input4.eof())
        {
            input4.close();
            break;
        }
        input4 >> x1 >> y1 >> z1;
        input4 >> x2 >> y2 >> z2;
        input4 >> x3 >> y3 >> z3;
        Point p1 = Point(x1, y1, z1);
        Point p2 = Point(x2, y2, z2);
        Point p3 = Point(x3, y3, z3);
        vector<Point> triangle;
        triangle.push_back(p1);
        triangle.push_back(p2);
        triangle.push_back(p3);
        triangles.push_back(triangle);
    }

    for (int i = 0; i < triangles.size(); i++)
    {
        vector<Point> triangle = rearrangrTrianglePoints(triangles[i]);
        Point p1 = triangle[0];
        Point p2 = triangle[1];
        Point p3 = triangle[2];

        int r = randNum() % 256;
        int g = randNum() % 256;
        int b = randNum() % 256;

        // cout << p1.x << " " << p1.y << " " << p1.z << endl;
        // cout << p2.x << " " << p2.y << " " << p2.z << endl;
        // cout << p3.x << " " << p3.y << " " << p3.z << endl<< endl;

        double minX = min(p1.x, min(p2.x, p3.x));
        double maxX = max(p1.x, max(p2.x, p3.x));
        double minY = min(p1.y, min(p2.y, p3.y));
        double maxY = max(p1.y, max(p2.y, p3.y));

        minX = max(minX, leftX);
        maxX = min(maxX, rightX);
        minY = max(minY, bottomY);
        maxY = min(maxY, topY);

        // upperLine and lowerLine are the row numbers of the topmost and bottommost pixels of the triangle
        int upperLine = round((topY - maxY) / dy);
        int lowerLine = round((topY - minY) / dy);

        cout << upperLine << " " << lowerLine << endl;

        for (int row = upperLine; row <= lowerLine; row++)
        {
            double y = topY - row * dy;
            double x_a, x_b;
            double z_a, z_b;
            double temp_a, temp_b;
            x_a = x_b = 0;
            z_a = z_b = 0;
            temp_a = temp_b = 0;

            bool flag = true;
            for (int i = 0; i < 3; i++)
            {
                int j = (i + 1) % 3;
                if (triangle[i].y == triangle[j].y)
                {
                    continue;
                }
                if (y >= min(triangle[i].y, triangle[j].y) && y <= max(triangle[i].y, triangle[j].y))
                {
                    if (flag)
                    {

                        temp_a = triangle[i].x - (triangle[i].x - triangle[j].x) * (triangle[i].y - y) / (triangle[i].y - triangle[j].y);
                        z_a = triangle[i].z - (triangle[i].z - triangle[j].z) * (triangle[i].y - y) / (triangle[i].y - triangle[j].y);
                        flag = false;
                    }

                    else
                    {
                        temp_b = triangle[i].x - (triangle[i].x - triangle[j].x) * (triangle[i].y - y) / (triangle[i].y - triangle[j].y);
                        z_b = triangle[i].z - (triangle[i].z - triangle[j].z) * (triangle[i].y - y) / (triangle[i].y - triangle[j].y);
                    }
                }
            }

            x_a=temp_a;
            x_b=temp_b;
            if(x_a<minX){
                x_a=minX;
            }
            else if(x_a>maxX){
                x_a=maxX;
            }

            if(x_b<minX){
                x_b=minX;
            }
            else if(x_b>maxX){
                x_b=maxX;
            }

            // cout<<"x_a: "<<x_a<<" x_b: "<<x_b<<endl;

            z_a = z_b-(z_b-z_a)*(temp_b-x_a)/(temp_b-temp_a);
            z_b = z_b-(z_b-z_a)*(temp_b-x_b)/(temp_b-temp_a);

            if(x_a>x_b){
                swap(x_a,x_b);
                swap(z_a,z_b);
            }



            // int count = 0;
            // for (int i = 0; i < 3; i++)
            // {
            //     int j = (i + 1) % 3;

            //     if (triangle[i].y == triangle[j].y)
            //     {
            //         continue;
            //     }
            //     if (y >= min(triangle[i].y, triangle[j].y) && y <= max(triangle[i].y, triangle[j].y))
            //     {
            //         x[count] = triangle[i].x - (triangle[i].x - triangle[j].x) * (triangle[i].y - y) / (triangle[i].y - triangle[j].y);
            //         x[count] = triangle[i].z - (triangle[i].z - triangle[j].z) * (triangle[i].y - y) / (triangle[i].y - triangle[j].y);
            //     }
            // }

            // double temp[2];
            // temp[0]=x[0];
            // temp[1]=x[1];

            // for (int i = 0; i < 2; i++)
            // {
            //     // clip x on both sides
            //     if (x[i] < minX)
            //         x[i] = minX;
            //     if (x[i] > maxX)
            //         x[i] = maxX;
            // }

            // // re-adjust
            // z[0] = z[1] - (z[1] - z[0]) * (temp[1] - x[0]) / (temp[1] - temp[0]);
            // z[1] = z[1] - (z[1] - z[0]) * (temp[1] - x[1]) / (temp[1] - temp[0]);
            // double x_a,x_b;
            // double z_a,z_b;

            // if(x[0]>=x[1]){
            //     x_a=x[1];
            //     x_b=x[0];
            //     z_a=z[1];
            //     z_b=z[0];
            // }
            // else{
            //     x_a=x[0];
            //     x_b=x[1];
            //     z_a=z[0];
            //     z_b=z[1];
            // }

            int leftPixel = round((x_a - leftX) / dx);
            int rightPixel = round((x_b - leftX) / dx);

            // cout<<"leftPixel: "<<leftPixel<<" rightPixel: "<<rightPixel<<endl;

            for (int col = leftPixel; col <= rightPixel; col++)
            {
                double x = leftX + col * dx;
                double z = z_b-(z_b-z_a)*(x-x_b)/(x_a-x_b);
                // cout<<"x: "<<x<<" y: "<<y<<" z: "<<z<<endl;
                if(z<zMin){
                    continue;   
                }
                if (z < zbuffer[row][col])
                {
                    zbuffer[row][col] = z;
                    image.set_pixel(col, row, r, g, b);
                }
            }
        }
    }
    image.save_image("output1.bmp");
    for (int i = 0; i < screenHeight; i++)
    {
        for (int j = 0; j < screenWidth; j++)
        {
            output << zbuffer[i][j] << " ";
        }
        output << endl;
    }
    output.close();
    input4.close();
    // free memory
    for (int i = 0; i < screenHeight; i++)
    {
        delete[] zbuffer[i];
    }
    delete[] zbuffer;

    return;
}
