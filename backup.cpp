/**
 * Projeto e Análise de Algoritmos
 * 
 * Implementação do algoritmo QuickHull para a cálculo fecho convexo a
 * partir de um conjunto de pontos.
 * 
 * Alunos:
 * 1508920 - Maria Fernanda Azolin
 * 1976744 - Flavia Gouvea Santos
 * 1942808 - Otávio Baziewicz Filho
 * 
 */

// Manipulação de arquivos
#include <iostream>
#include <fstream>

// Estrutura de dadoss e funções matemáticas
#include <vector>
#include <math.h>

// Cálculo do tempo de execução
#include <chrono>

using namespace std::chrono;
using namespace std;

class Ponto
{
public:
    float x;
    float y;

    Ponto()
    {
        this->x = 0;
        this->y = 0;
    }

    Ponto(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
};

class Reta
{
public:
    Ponto *p1;
    Ponto *p2;

    Reta(Ponto p1, Ponto p2)
    {
        this->p1 = &p1;
        this->p2 = &p2;
    }
};

/**
 * Função que verifica se o ponto está à esquerda da reta.
 * 
 * Para verificar se p3(x3,y3) está à esquerda da reta p1(x1,y1)p2(x2,y2),
 * calcula-se: 
 * f(p1,p2,p3) = (x2 − x1)(y3 − y1) − (y2 − y1)(x3 − x1);
 * 
 * se f(p1,p2,p3) > 0, p3 está à esquerda da reta p1p2.
 */
float pontoEstaAEsquerdaDaReta(Ponto p, Reta r)
{
    return (r.p2->x - r.p1->x) * (p.y - r.p1->y) - (r.p2->y - r.p1->y) * (p.x - r.p1->x);
}

/**
 * Função que calcula a distância entre um ponto e uma reta.
 * 
 * Para fazer o cálculo entre o ponto p3(x3,y3) e a reta p1(x1,y1)p2(x2,y2),
 * faz-se:
 * d(p1,p2,p3) = sqrt(((x2 − x1)(y3 − y1) − (y2 − y1)(x3 − x1))^2) / sqrt(((x2 − x1)^2 + (y2 − y1)^2))
 * 
 */
float distanciaEntrePontoEReta(Ponto p, Reta r)
{
    return (float)(sqrt(pow(((r.p2->x - r.p1->x) * (p.y - r.p1->y) - (r.p2->y - r.p1->y) * (p.x - r.p1->x)), 2)) /
                   sqrt(pow((r.p2->x - r.p1->x), 2) + pow(r.p2->y - r.p1->y, 2)));
}

/**
 * Função que ordena o conjunto de pontos com valor de x crescente,
 * e retorna um vetor `v`, onde `v[0]` é o ponto extremo à esquerda e 
 * `v[1]` é o ponto extremo à direita.
 * 
 * O critério de escolha é o maior x para o máximo e o menor x para o mínimo.
 * Em caso de empate, maior y para o menor, menor y para o maior
 * 
 *  1. Duas variáveis são criadas para armazenar os valores intermediários dos 
 *     resultados buscados.
 *  2. Percorre o vetor de pontos para buscar os extremos
 *  3. Atualiza o ponto mínimo escolhido caso o ponto em análise atenda aos 
 *     critérios. (menor x, e maior y em caso de empate).
 *  4. Atualiza o ponto máximo escolhido caso o ponto em análise atenda aos 
 *     critérios. (maior x, e menor y em caso de empate).
 *  5. Retorna um arraycom o mínimo na primeira posição e o máximo na segunda.
 */
vector<Ponto> pontosExtremos(vector<Ponto> pontos)
{
    // 1
    Ponto pontoMinimo = pontos[0], pontoMaximo = pontos[0];

    // 2
    for (const Ponto &ponto : pontos)
    {
        // 3
        if (pontoMinimo.x > ponto.x || (pontoMinimo.x == ponto.x && pontoMinimo.y < ponto.y))
        {
            pontoMinimo = ponto;
        }

        // 4
        if (pontoMaximo.x < ponto.x || (pontoMaximo.x == ponto.x && pontoMaximo.y > ponto.y))
        {
            pontoMaximo = ponto;
        }
    }

    // 5
    vector<Ponto> extremos;
    extremos.push_back(pontoMinimo);
    extremos.push_back(pontoMaximo);

    return extremos;
}

/**
 * Função que retorna o ponto presente em um conjunto, que está mais distante
 * de uma reta `r` informada.
 * 
 *  1. Duas variáveis são criadas para armazenar o ponto mais distânte da reta 
 *     a cada iteração do loop
 *  2. Percorre o vetor de pontos.
 *  3. Se a distância entre o ponto em análise e a reta for maior do que a 
 *     previamente registrada, o ponto mais distânte é atualizado, juntamente
 *     com o valor da distância.
 *  4. Retorna o ponto mais distânte da reta.
 */
Ponto pontoMaisDistanteDaReta(vector<Ponto> pontos, Reta r)
{
    // 1
    Ponto pontoMaisDistante;
    float maiorDistancia = -1;

    // 2
    for (const Ponto &ponto : pontos)
    {
        // 3
        float distancia = distanciaEntrePontoEReta(ponto, r);
        if (distancia > maiorDistancia)
        {
            maiorDistancia = distancia;
            pontoMaisDistante = ponto;
        }
    }

    // 4
    return pontoMaisDistante;
}

/**
 * Função que calcula o fecho convexo de um conjunto de pontos
 * 
 * TODO-> Finalizar função e documentar
 *  
 */
vector<Ponto> quickHull(vector<Ponto> pontos, Ponto p, Ponto q)
{
    if (pontos.size() == 2)
    {
        vector<Ponto> fechoConvexo;
        fechoConvexo.push_back(p);
        fechoConvexo.push_back(q);
        return fechoConvexo;
    }
    else if (pontos.size() == 1)
    {
        vector<Ponto> fechoConvexo;
        fechoConvexo.push_back(q);
        return fechoConvexo;
    }
    else if (pontos.size() == 0)
    {
        vector<Ponto> a;
        return a;
    }

    vector<Ponto> pontosAEsquerda, pontosADireita;

    for (const Ponto &ponto : pontos)
    {
        float result = pontoEstaAEsquerdaDaReta(ponto, Reta(p, q));
        if (result > 0)
        {
            pontosAEsquerda.push_back(ponto);
        }
        else if (result < 0)
        {
            pontosADireita.push_back(ponto);
        }
    }

    Ponto pontoAEsquerda = pontoMaisDistanteDaReta(pontosAEsquerda, Reta(p, q));
    vector<Ponto> lista1 = quickHull(pontosAEsquerda, p, pontoAEsquerda);

    if (pontosADireita.size() != 0)
    {
    }
    Ponto pontoADireita = pontoMaisDistanteDaReta(pontosADireita, Reta(p, q));
    vector<Ponto> lista2 = quickHull(pontosADireita, p, pontoADireita);

    lista1.insert(lista1.end(), lista2.begin(), lista2.end());

    return lista1;
}

/**
 * Função que abre o arquivo de entrada do programa e retorna um vetor de
 * pontos.
 * 
 * A entrada é no formato:
 * 
 * n
 * x1 y1
 * x2 y2
 * ...
 * xn yn
 * 
 * Onde `n` é a quantidade de pontos, bem como `xn` e `yn` as coordenadas de
 * cada ponto.
 * 
 *  1. Abre o arquivo de entrada no modo de leitura.
 *  2. Verifica se o arquivo foi aberto com sucesso.
 *  3. Cria as variáveis a serem utilizadas.
 *  4. Joga a primeira linha do arquivo no lixo.
 *  5. Recebe cada linha do arquivo e adiciona um ponto correspondente no
 *     vetor de pontos.
 *  6. Fecha o arquivo e retorna o vetor de pontos.
 */
vector<Ponto> lerPontosDoArquivoDeEntrada(char *nome)
{
    // 1
    ifstream arquivo(nome);

    // 2
    if (arquivo.fail())
    {
        cerr << "Erro ao abrir arquivo" << endl;
        arquivo.close();
        exit(1);
    }

    // 3
    vector<Ponto> pontos;
    string line;
    int x, y;

    // 4
    arquivo >> x;

    // 5
    while (getline(arquivo, line))
    {
        arquivo >> x >> y;
        Ponto p = Ponto(x, y);
        pontos.push_back(p);
    }

    // 6
    arquivo.close();
    return pontos;
}

/**
 * Função que recebe os pontos do fecho convexo em ordem anti-horária e 
 * cria um arquivo no formato:
 * 
 * x1 y1
 * x2 y2
 * ...
 * xn yn
 * 
 * Onde `xn` e `yn` são as coordenadas de cada ponto.
 * 
 *  1. Cria um novo arquivo no modo de escrita.
 *  2. Preenche cada linha do arquivo com um ponto correspondente do
 *     fecho convexo.
 *  3. Fecha o arquivo.
 * 
 */
void criarArquivoDeSaida(vector<Ponto> feixoConvexo, string nome)
{
    // 1
    ofstream arquivo(nome);

    // 2
    for (const Ponto &ponto : feixoConvexo)
    {
        arquivo << ponto.x << " " << ponto.y << endl;
    }

    // 3
    arquivo.close();
}

/**
 * 1. Verifica se o arquivo existe no diretório
 */
bool arquivoExiste(const char *fileName)
{
    // 1
    ifstream infile(fileName);
    return infile.good();
}

/**
 * Função de entrada do programa
 * 
 *  1. Trata a entrada do programa para aceitar o arquivo de pontos.
 *  2. Lê o arquivo de entrada e criar um vetor de pontos.
 *  3. Inicia o contador de tempo.
 *  4. Calcula os pontos extremos do conjunto de dados e executa o algoritmo 
 *     quickHull para obter o fecho convexo do vetor de pontos.
 *  5. Contabiliza o tempo decorrido e mostra no terminal.
 *  6. Cria o arquivo de saída com a representação do fecho convexo.
 */
int main(int argc, char **argv)
{
    // 1
    if (argc != 2 || !arquivoExiste(argv[1]))
    {
        cout
            << "Uso: ./hull input_file.txt\n"
            << endl;
        return 1;
    }

    // 2
    vector<Ponto> pontos = lerPontosDoArquivoDeEntrada(argv[1]);

    // 3
    auto inicio = high_resolution_clock::now();

    // 4
    vector<Ponto> extremos = pontosExtremos(pontos);
    vector<Ponto> fechoConvexo = quickHull(pontos, extremos[0], extremos[1]);

    // 5
    auto fim = high_resolution_clock::now();
    auto tempo = duration_cast<microseconds>(fim - inicio);
    cout << fixed << setprecision(6) << (float)(tempo.count()) / 1000000 << endl;

    // 6
    criarArquivoDeSaida(fechoConvexo, "fecho.txt");

    /*vector<Ponto> a;
    vector<Ponto> b;

    a.push_back(Ponto(1,1));
    b.push_back(Ponto(2,2));
    a.push_back(Ponto(3,3));
    */

    for (int i = 0; i < fechoConvexo.size(); i++)
    {
        cout << "x: " << fechoConvexo[i].x << " y: " << fechoConvexo[i].y << endl;
    }

    return 0;
}