#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

class Point
{
public:
    int x;
    int y;
};

vector<Point> readCoordinates()
{
    ifstream inFile;
    inFile.open("my_file.txt");

    if (inFile.fail())
    {
        cerr << "Error opening a file" << endl;
        inFile.close();
        exit(1);
    }
    string line;
    int x, y;
    int num_coordinates;

    inFile >> num_coordinates;
    vector<Point> coordinates;

    int count = 0;
    while (getline(inFile, line))
    {
        inFile >> x >> y;

        Point ponto;
        ponto.x = x;
        ponto.y = y;

        coordinates.push_back(ponto);

        count++;
    }

    inFile.close();

    return coordinates;
}

float getDistanceBetweenPoints(Point p1, Point p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

int getPointPosition(Point p1, Point p2, Point p3) 
{
    int a = p2.x - p1.x;
    int b = p3.y - p1.y;
    int c = p2.y - p1.y;
    int d = p3.x - p1.x;

    return a*b - c*d;
}

int main()
{
    vector<Point> coordinates;

    coordinates = readCoordinates();

    for (int i = 0; i <= coordinates.size() - 1; i++)
    {
        cout << coordinates[i].x << " " << coordinates[i].y << endl;
    }

    Point a;
    a.x = 0;
    a.y = 0;
    Point b;
    b.x = 2;
    b.y = 2;
    Point c;
    c.x = 2;
    c.y = 2;

    cout << getPointPosition(a, b, c);
    

    return 0;
}