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

/*
* A função lê as linhas de um arquivo txt, onde a primeira linha é a quantidade de pontos
* e em sequência, as coordenadas x e y dos pontos, e salva numa struct que guarda as duas
* coordenadas
*
* saída: Retorna um vetor de pontos, com as coordenadas x e y
*/
vector<Point> readCoordinates()
{
    ifstream inFile;
    inFile.open("input.txt");

    // verifica se conseguiu abrir o arquivo
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

        Point point;
        point.x = x;
        point.y = y;

        coordinates.push_back(point);

        count++;
    }

    inFile.close();

    return coordinates;
}

/*
* Calcula a distância entre dois pontos pelo Teorema e Pitagoras
*
* entrada: dois Pontos com suas coordenadas
*
* saída: Retorna um vetor de pontos, com as coordenadas x e y
*/
// TODO: vamos usar?
float getDistanceBetweenPoints(Point p1, Point p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

/*
* Retorna a posição de um ponto em relação a outros dois
*
* entrada: três Pontos com suas coordenadas
*
* saída: Retorna um número
*        se > 0, p3 está À esquerda de p2p1
*        se < 0, p3 está À direita de p2p1
*        se = 0, p3 é coincidente a p2p1
*/
int getPointPosition(Point p1, Point p2, Point p3)
{
    int twoOneX = p2.x - p1.x;
    int threeOneY = p3.y - p1.y;
    int twoOneY = p2.y - p1.y;
    int threeOneX = p3.x - p1.x;

    return (twoOneX * threeOneY) - (twoOneY * threeOneX);
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
