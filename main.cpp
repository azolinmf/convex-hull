#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

class Point {
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
vector<Point> readCoordinates(char* fileName) {
    ifstream inFile;
    inFile.open(fileName);

    // verifica se conseguiu abrir o arquivo
    if (inFile.fail()) {
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
    while (getline(inFile, line)) {
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
float getDistanceBetweenPoints(Point p1, Point p2) {
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
double getPointPosition(Point p1, Point p2, Point p3) {
    int firstPointX = p2.x - p1.x;
    int firstPointY = p3.y - p1.y;
    int secondPointY = p2.y - p1.y;
    int secondPointX = p3.x - p1.x;

    return (firstPointX * firstPointY) - (secondPointY * secondPointX);
}

/**
 * @param p1: primeiro ponto da reta
 * @param p2: segundo ponto da reta
 * @param p3: ponto fora da reta
 * @return distancia entre o ponto e a reta
 * */
double calculateDistanceBetweenPointAndRect(Point p1, Point p2, Point p3) {
    double position = getPointPosition(p1, p2, p3);
    double squareRoot = sqrt(pow(position, 2));
    double u = pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2);

    return squareRoot/u;
}

/**
 * @param points: vetor com todos os pontos
 * @return vetor com os dois pontos extremos
 * */
vector<Point> getEdgePoints(vector<Point> points) {
    Point minimumXpoint = points[0];
    Point maximumXpoint = points[0];

    for (int i = 0; i <= points.size() - 1; i++) {
        if (minimumXpoint.x > points[i].x || (minimumXpoint.x == points[i].x && minimumXpoint.y < points[i].y) ) {
            minimumXpoint = points[i];
           
        }    
        if (maximumXpoint.x < points[i].x || (maximumXpoint.x == points[i].x && maximumXpoint.y > points[i].y) ) {
            maximumXpoint = points[i];
        }
    }

    vector<Point> edges;
    edges.push_back(minimumXpoint);
    edges.push_back(maximumXpoint);

    return edges;
}

Point edgePoint(vector<Point> points, Point p, Point q) {
    Point foundPoint;
    double distanceFound = -1;

    for(int i = 0; i < points.size(); i++) {
        double distance = calculateDistanceBetweenPointAndRect(p, q, points[i]);
        if (distanceFound < distance) {
            distanceFound = distance;
            foundPoint = points[i];
        }
    }

    return foundPoint;
}

vector<Point> convexHull(vector<Point> points, Point p, Point q) {

    if (points.size() <= 2) {
        vector<Point> v;
        v.push_back(p);
        v.push_back(q);
        return v;
    }

    vector<Point> toTheRight;
    vector<Point> toTheLeft;

    for(int i = 0; i < points.size(); i++) {
        double position = getPointPosition(p, q, points[i]);
        if (position > 0) {
            //ponto esta a esquerda da reta pq
            toTheLeft.push_back(points[i]);
        } else if (position < 0) {
            //ponto esta a direita da reta pq
            toTheRight.push_back(points[i]);
        }
    }

    Point leftPoint = edgePoint(toTheLeft, p, q);
    convexHull(toTheLeft, p, leftPoint);

    Point rightPoint = edgePoint(toTheRight, p, q);
    convexHull(toTheRight, p, rightPoint);
}

/**
 * Verifica se existe um arquivo com o nome passado como parâmetro para a função
 */
bool fileExists(const char *fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}
  
int main(int argc, char** argv) {
    if(argc != 2 || !fileExists(argv[1])) {
        cout << "Uso: ./hull input_file.txt\n" << endl;
		return 1;
	}

    vector<Point> coordinates;

    coordinates = readCoordinates(argv[1]);

    for (int i = 0; i <= coordinates.size() - 1; i++) {
        // cout << coordinates[i].x << " " << coordinates[i].y << endl;
    }

    Point a;
    a.x = 0;
    a.y = 0;
    Point b;
    b.x = 0;
    b.y = 10;
    Point c;
    c.x = 10;
    c.y = 10;
    Point d;
    d.x = 10;
    d.y = 0;

    vector<Point> v;
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);
    v.push_back(d);

    return 0;
}