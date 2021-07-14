#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std::chrono;
using namespace std;

struct Ponto
{
    float x, y;

    Ponto()
    {
        x = 0, y = 0;
    }

    Ponto(const float px, const float py)
    {
        x = px, y = py;
    }

    friend bool operator==(const Ponto &p1, const Ponto &p2)
    {
        return p1.x == p2.x && p1.y == p2.y;
    }
};

struct Reta
{
    Ponto p1, p2;

    Reta()
    {
        this->p1 = Ponto();
        this->p2 = Ponto();
    }

    Reta(Ponto p1, Ponto p2)
    {
        this->p1 = Ponto(p1.x, p1.y);
        this->p2 = Ponto(p2.x, p2.y);
    }
};

// TODO -> Fazer ela local e passar como parâmetro
vector<Ponto> fechoConvexo;

int pontoEstaAEsquerdaDaReta(Ponto p, Reta r)
{
    float area = ((r.p2.x - r.p1.x) * (p.y - r.p1.y)) - ((r.p2.y - r.p1.y) * (p.x - r.p1.x));

    if (area > 0)
        return 1; //esquerda
    if (area < 0)
        return -1; //direita
    return 0;      //colinear
}

float distanciaEntrePontoEReta(Ponto p, Reta r)
{
    float t1 = ((r.p2.x - r.p1.x) * (p.y - r.p1.y)) - ((r.p2.y - r.p1.y) * (p.x - r.p1.x));
    float t2 = (r.p2.x - r.p1.x);
    float t3 = (r.p2.y - r.p1.y);

    return sqrtf(t1 * t1) / sqrtf((t2 * t2) + (t3 * t3));
}

vector<Ponto> pontosExtremos(vector<Ponto> pontos)
{
    Ponto pMax = pontos[0];
    Ponto pMin = pontos[0];

    for (const Ponto &ponto : pontos)
    {
        if (ponto.x > pMax.x || (ponto.x == pMax.x && ponto.y < pMax.y))
        {
            pMax = ponto;
        }

        if (ponto.x < pMin.x || (ponto.x == pMin.x && ponto.y > pMin.y))
        {
            pMin = ponto;
        }
    }

    vector<Ponto> extremos;
    extremos.push_back(pMin);
    extremos.push_back(pMax);
    return extremos;
}

void findHull(vector<Ponto> pontos, Reta r)
{
    if (pontos.size() == 0)
        return;

    float maxDist = 0;
    Ponto maxPoint;
    for (const Ponto &ponto : pontos)
    {
        float pointDist = distanciaEntrePontoEReta(ponto, r);
        if (pointDist > maxDist)
        {
            maxDist = pointDist;
            maxPoint = ponto;
        }
    }

    vector<Ponto>::iterator it = find(fechoConvexo.begin(), fechoConvexo.begin() + fechoConvexo.size(), r.p1);
    fechoConvexo.insert(it + 1, maxPoint);

    vector<Ponto> s1; //direita de (r.p1, maxPoint)
    for (const Ponto &ponto : pontos)
    {
        int lado = pontoEstaAEsquerdaDaReta(ponto, Reta(r.p1, maxPoint));
        if (lado == -1)
            s1.push_back(ponto);
    }

    vector<Ponto> s2; //direita de (maxPoint, r.p2)
    for (const Ponto &ponto : pontos)
    {
        int lado = pontoEstaAEsquerdaDaReta(ponto, Reta(maxPoint, r.p2));
        if (lado == -1)
            s2.push_back(ponto);
    }

    findHull(s1, Reta(r.p1, maxPoint));
    findHull(s2, Reta(maxPoint, r.p2));
}

void quickHull(vector<Ponto> pontos)
{
    vector<Ponto> extremos = pontosExtremos(pontos);

    fechoConvexo.insert(fechoConvexo.begin(), extremos[0]);
    fechoConvexo.insert(fechoConvexo.begin() + 1, extremos[1]);

    vector<Ponto> s1; //direita de Reta(extremos[0], extremos[1])
    for (const Ponto &ponto : pontos)
    {
        int val = pontoEstaAEsquerdaDaReta(ponto, Reta(extremos[0], extremos[1]));

        if (val == -1)
            s1.push_back(ponto);
    }

    vector<Ponto> s2; //direita de Reta(extremos[1], extremos[0])
    for (const Ponto &ponto : pontos)
    {
        int val = pontoEstaAEsquerdaDaReta(ponto, Reta(extremos[1], extremos[0]));

        if (val == -1)
            s2.push_back(ponto);
    }

    findHull(s1, Reta(extremos[0], extremos[1]));
    findHull(s2, Reta(extremos[1], extremos[0]));
}

vector<Ponto> lerPontosDoArquivoDeEntrada(char *nome)
{
    ifstream arquivo(nome);

    if (arquivo.fail())
    {
        cerr << "Erro ao abrir arquivo" << endl;
        arquivo.close();
        exit(1);
    }

    vector<Ponto> pontos;
    string line;
    int x, y;

    arquivo >> x;

    while (getline(arquivo, line))
    {
        arquivo >> x >> y;
        Ponto p = Ponto(x, y);
        pontos.push_back(p);
    }

    arquivo.close();
    return pontos;
}

void criarArquivoDeSaida(vector<Ponto> fechoConvexo, string nome)
{
    ofstream arquivo(nome);

    for (const Ponto &ponto : fechoConvexo)
    {
        arquivo << ponto.x << " " << ponto.y << endl;
    }

    arquivo.close();
}

int main(int argc, char **argv)
{
    // TODO -> Implementar contagem de tempo
    // TODO -> Implementar tratamento de entrada

    vector<Ponto> pontos = lerPontosDoArquivoDeEntrada(argv[1]);

    quickHull(pontos);

    criarArquivoDeSaida(fechoConvexo, "fecho.txt");

    return 0;
}