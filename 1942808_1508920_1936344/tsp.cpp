/**
 * Projeto e Análise de Algoritmos
 * 
 * Implementação do algoritmo QuickHull para a calculo fecho convexo a
 * partir de um conjunto de pontos.
 * 
 * Alunos:
 * 1508920 - Maria Fernanda Azolin
 * 1936344 - Flavia Gouvea Santos
 * 1942808 - Otávio Baziewicz Filho
 * 
 */

// Manipulação de arquivos
#include <iostream>
#include <fstream>
#include <iomanip>
// Estrutura de dados e funções matemáticas
#include <vector>
#include <numeric>
#include <math.h>
// Cálculo do tempo
#include <chrono>

using namespace std::chrono;
using namespace std;

/**
 * Estrutura de dados que representa um ponto com coordenadas x e y
 */
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

/**
 * Estrutura de dados que representa uma reta direcionada de um ponto p1 até
 * um ponto p2
 */
struct Reta
{
    Ponto p1, p2;

    Reta()
    {
        this->p1 = Ponto(), this->p2 = Ponto();
    }

    Reta(Ponto p1, Ponto p2)
    {
        this->p1 = p1, this->p2 = p2;
    }
};

float distanciaEuclidiana(Ponto p1, Ponto p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

//dentro do fecho é pk
float somaDistanciasTripla(Ponto pi, Ponto pj, Ponto pk)
{
    float dij = distanciaEuclidiana(pi, pj);
    float dik = distanciaEuclidiana(pi, pk);
    float djk = distanciaEuclidiana(pj, pk);

    return dik + djk - dij;
}

/**
 * Função que verifica se o ponto está à esquerda da reta, à direita ou sobre a reta.
 *
 * Como apenas é calculado operações matemáticas com constantes e não há nenhum loop,
 * o algoritmo é linear, portanto pertence a O(1)
 * 
 * Para verificar se p(x3,y3) está à esquerda da reta p1(x1,y1)p2(x2,y2),
 * calcula-se: 
 * f(p1,p2,p) = (x2 − x1)(y3 − y1) − (y2 − y1)(x3 − x1);
 * 
 * se f(p1,p2,p) > 0, p3 está à esquerda da reta p1p2. Retorna 1
 * se f(p1,p2,p) < 0, p3 está à direita da reta p1p2. Retorna -1
 * se f(p1,p2,p) = 0, p3 está sobre a reta p1p2. Retorna 0
 */
int posicaoPontoDaReta(Ponto p, Reta r)
{
    float area = ((r.p2.x - r.p1.x) * (p.y - r.p1.y)) - ((r.p2.y - r.p1.y) * (p.x - r.p1.x));

    if (area > 0)
        return 1;
    if (area < 0)
        return -1;
    return 0;
}

/**
 * Função que calcula a distância entre um ponto e uma reta.
 *
 * Como apenas é calculado operações matemáticas com constantes e não há nenhum loop,
 * o algoritmo é linear, portanto pertence a O(1)
 * 
 * Para fazer o cálculo entre o ponto p(x3,y3) e a reta p1(x1,y1)p2(x2,y2),
 * faz-se:
 * d(p1,p2,p) = sqrt(((x2 − x1)(y3 − y1) − (y2 − y1)(x3 − x1))^2) / sqrt(((x2 − x1)^2 + (y2 − y1)^2))
 */
float distanciaEntrePontoEReta(Ponto p, Reta r)
{
    float t1 = ((r.p2.x - r.p1.x) * (p.y - r.p1.y)) - ((r.p2.y - r.p1.y) * (p.x - r.p1.x));
    float t2 = (r.p2.x - r.p1.x);
    float t3 = (r.p2.y - r.p1.y);

    return sqrtf(t1 * t1) / sqrtf((t2 * t2) + (t3 * t3));
}

/**
 * Função que retorna os pontos mais extremos dentro de um conjunto de pontos.
 *
 * Para pegar os pontos mais extremos, é necessário percorrer o vetor de pontos,
 * de tamanho n, por completo para verificar todos, portanto pertence a O(n)
 * 
 * O critério de escolha é o maior x para o máximo e o menor x para o mínimo.
 * Em caso de empate, maior y para o menor, menor y para o maior.
 */
vector<Ponto> pontosExtremos(vector<Ponto> pontos)
{
    Ponto pontoMaximo = pontos[0];
    Ponto pontoMinimo = pontos[0];

    for (const Ponto &ponto : pontos)
    {
        if (ponto.x > pontoMaximo.x || (ponto.x == pontoMaximo.x && ponto.y < pontoMaximo.y))
        {
            pontoMaximo = ponto;
        }

        if (ponto.x < pontoMinimo.x || (ponto.x == pontoMinimo.x && ponto.y > pontoMinimo.y))
        {
            pontoMinimo = ponto;
        }
    }

    vector<Ponto> extremos;
    extremos.push_back(pontoMinimo);
    extremos.push_back(pontoMaximo);
    return extremos;
}

/**
 * Função que encontra os pontos pertencentes ao fecho convexo dentre todos os pontos
 * para algum lado da reta
 *
 * Primeiro é calculado o ponto mais distante da reta, para isso, é necessário percorrer o vetor
 * uma vez [O(n)]. Ao encontrar esse ponto, ele já estará no fecho convexo, então ele é
 * inserido no fechoConvexo na posição correta linearmente [O(n)]. Com esse ponto e a reta, temos um poligono.
 * Analisamos quais pontos estão dentro do polígono, para descartá-los e quais estão fora [O(n)].
 * Estes que estão fora, são enviados como parâmetro novamente para se repetir o processo, até
 * que todos sejam analisados [O(n^2)].
 * No melhor caso, se os pontos estão balanceados, ou seja, que possuem o mesmo número de pontos
 * à direita e à esquerda da reta de pontos extremos, a complexidade pertence a O(n lg n). Pois temos
 * T(n) = 2T(n/2) + O(n), onde o 2T(n/2) representa a recursão e o O(n) representa as buscas. Usando o
 * Teorema Mestre, vemos que a = 2, b = 2 e d = 1, então caimos no caso que a = b^d, portanto
 * T(n) pertence a O(n^d * log(b) n), ou seja O(n lg n)
 * No pior caso, temos os pontos desbalanceados, ou seja, todos os pontos estão em apenas um dos lados,
 * e isso seria T(n) = T(n-1) + O(n), que resulta em O(n^2), pelo Método da Substituição.
 * 
 * 
 * Corretude:
 * Como calculamos as ditancias entre ponto e a reta para todos os pontos do 
 * conjunto, e vamos substituindo na variavel distanciaMaxima, ao fim do primeiro for
 * sempre temos garantidamente o ponto mais distante da reta, que pertence ao fecho.
 * No segundo for, criamos duas novas retas, que formam um poligono. Sabendo a posicao
 * dessa reta, conseguimos descartar pontos em cima dela (para ter sempre o menor conjunto)
 * e pontos dentro do poligono, que nao pertencem ao fecho. Fazendo isso recursivamente, garantimos 
 * que nao teremos nenhum ponto mais distante ainda fora do poligono.
 * 
 *
 * Recebe o ponteiro que guardará os pontos do fecho convexo, a lista com todos os
 * pontos a serem analisados (à direita ou à esquerda da reta) e a reta com dois pontos.
 *
 */
void findHull(vector<Ponto> *fechoConvexo, vector<Ponto> pontos, Reta r)
{
    if (pontos.size() == 0)
        return;

    float distanciaMaxima = 0;
    Ponto pontoMaisDistante;
    for (const Ponto &ponto : pontos)
    {
        float distancia = distanciaEntrePontoEReta(ponto, r);
        if (distancia > distanciaMaxima)
        {
            distanciaMaxima = distancia;
            pontoMaisDistante = ponto;
        }
    }

    vector<Ponto>::iterator it = find(fechoConvexo->begin(), fechoConvexo->begin() + fechoConvexo->size(), r.p1);

    fechoConvexo->insert(it + 1, pontoMaisDistante);

    vector<Ponto> s1; //direita de (r.p1, pontoMaisDistante)
    vector<Ponto> s2; //direita de (pontoMaisDistante, r.p2)

    for (const Ponto &ponto : pontos)
    {
        int lado1 = posicaoPontoDaReta(ponto, Reta(r.p1, pontoMaisDistante));
        if (lado1 == -1)
            s1.push_back(ponto);

        int lado2 = posicaoPontoDaReta(ponto, Reta(pontoMaisDistante, r.p2));
        if (lado2 == -1)
            s2.push_back(ponto);
    }

    findHull(fechoConvexo, s1, Reta(r.p1, pontoMaisDistante));
    findHull(fechoConvexo, s2, Reta(pontoMaisDistante, r.p2));
}

/**
 * Função acha os pontos que compõe o fecho convexo de um conjunto de pontos
 *
 * Primeiramente, encontra os pontos extremos do conjunto [O(n)], esses pontos
 * já são inseridos na resposta, pois fazem parte do fecho convexo. Em seguida,
 * percorre o vetor para separar em pontos à direita e à esquerda da reta formada
 * pelos pontos extremos [O(n)]. Por fim, é chamado o findHull [O(n^2)].
 * Portanto, essa função é O(n) +  O(n^2), resultando em O(n^2), pois é a de maior
 * ordem.
 * 
 * Corretude:
 * A hipótese é de que, recebendo um vetor de pontos, retornamos o fecho
 * convexo formado com o menor número de pontos.
 * O primeiro for garante que dividimos corretamente o vetor para aplicar
 * o metodo de divisao e conquista, diminuindo nosso problema a uma proposta mais simples.
 * Com os dois conjuntos, juntamos os dois poligonos retornados pelo findHull, que já formam
 * cada um, a metade do fecho, e resultam no fecho convexo final.
 * 
 *
 * Recebe um vetor de pontos e retorna um vetor de pontos do fechoConvexo ordenado
 * anti-horário
 */
vector<Ponto> quickHull(vector<Ponto> pontos)
{
    vector<Ponto> fechoConvexo;
    vector<Ponto> extremos = pontosExtremos(pontos);

    fechoConvexo.insert(fechoConvexo.begin(), extremos[0]);
    fechoConvexo.insert(fechoConvexo.begin() + 1, extremos[1]);

    vector<Ponto> s1; //direita de Reta(extremos[0], extremos[1])
    vector<Ponto> s2; //direita de Reta(extremos[1], extremos[0])

    for (const Ponto &ponto : pontos)
    {
        int val = posicaoPontoDaReta(ponto, Reta(extremos[0], extremos[1]));

        if (val == -1)
            s1.push_back(ponto);
        else if (val == 1)
            s2.push_back(ponto);
    }

    findHull(&fechoConvexo, s1, Reta(extremos[0], extremos[1]));
    findHull(&fechoConvexo, s2, Reta(extremos[1], extremos[0]));

    return fechoConvexo;
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
    ifstream arquivo(nome);

    if (arquivo.fail())
    {
        cerr << "Erro ao abrir arquivo" << endl;
        arquivo.close();
        exit(1);
    }

    vector<Ponto> pontos;
    string line;
    int n, x, y;

    arquivo >> n;
    if (n <= 2)
    {
        cerr << "Arquivo de entrada inválido" << endl;
        arquivo.close();
        exit(1);
    }

    while (getline(arquivo, line))
    {
        arquivo >> x >> y;
        pontos.push_back(Ponto(x, y));
    }

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
void criarArquivoDeSaida(vector<Ponto> pontos, string nome)
{
    ofstream arquivo(nome);

    for (const Ponto &ponto : pontos)
    {
        arquivo << ponto.x << " " << ponto.y << endl;
    }

    arquivo.close();
}

/**
 * Função que verifica se o arquivo existe no diretório
 */
bool arquivoExiste(const char *nome)
{
    ifstream arquivo(nome);
    return arquivo.good();
}

/**
 * Função que procura a melhor tripla entre os pontos do ciclo atual, sendo que
 * uma tripla é um conjunto de tres pontos formado por dois pontos que sao uma reta 
 * do ciclo e um ponto a ser inserido entre eles. A melhor tripla é a que possui
 * a menor distancia resultante da soma do ponto 1 ao ponto que queremos inserir e
 * do ponto que queremos inserir ao ponto 2.
 * Essa funcao recebe o ciclo atual e os pontos que pertencem a entrada, mas ainda
 * nao foram colocados no ciclo.
 * 
 * 
 * Complexidade: 
 * A complexidade é O(n*m), sendo m o tamanho do ciclo, e n o tamanho da lista de pontos
 * que ainda nao fazem parte do ciclo.
 * Como temos um for iterando entre os n pontos da lista, e dentro dele, 
 * outro for interando entre todos os m pontos do ciclo, temos n*m.
 * As demais operacoes dentro dos for nao alteram a complexidade,
 * pois são apenas comparacoes (ifs e elses) e calculos, portante, tem tempos constantes.
 * 
 * 
 * Corretude:
 * O algoritmo funciona pois todos os pontos da lista sao analisados e as triplas que 
 * podemos formar com eles, computadas. Sabendo o valor da distancia das triplas, armazenamos
 * em uma variavel que é sempre substituida quando se acha um valor menor de distancia, ao mesmo tempo
 * que se atualiza o valor da possivel melhor tripla.
 **/

vector<int> achaMelhorTripla(vector<Ponto> ciclo, vector<Ponto> C)
{
    float distancia = 0;
    float menorDistancia = somaDistanciasTripla(ciclo[0], ciclo[1], C[0]);
    int index = 0;

    vector<int> tripla;
    //index dos pontos que sao a melhor tripla

    for (int i = 0; i < C.size(); i++)
    {
        for (int j = 0; j < ciclo.size(); j++)
        {
            if (j < ciclo.size() - 1)
            {
                index = j + 1;
            }
            else
            {
                index = 0;
            }

            distancia = somaDistanciasTripla(ciclo[j], ciclo[index], C[i]);

            if (distancia < menorDistancia)
            {
                tripla.clear();

                menorDistancia = distancia;

                //pra retornar o index dos tres pontos que sao a melhor tripla
                tripla.push_back(j);
                tripla.push_back(index);
                tripla.push_back(i);
            }
        }
    }

    return tripla;
}

/**
 * Função que determina quais são os pontos que não pertencem ao fecho convexo.
 * Recebe os pontos do arquivo de entrada (todos os pontos) e a lista que é o fecho.
 * 
 * Complexidade:
 * A complexidade é O(n*m), sendo n o tamanho do pontos, e m o tamanho do fecho.
 * Como temos um for iterando entre todos os n pontos de pontos, e dentro dele, outro for
 * iterando entre os m pontos do fecho, temos m*n.
 * 
 * Corretude:
 * Como percorremos o vetor inteiro de pontos da entrada, e fazemos a verificação um a um
 * para checar se pertencem ao fecho, garantidamente no fim, teremos nosso vetor resultado só
 * com os pontos desejados (os que nao pertencem ao fecho).
 * 
 * */
vector<Ponto> pontosForaDoFecho(vector<Ponto> pontos, vector<Ponto> fecho)
{

    vector<Ponto> C;

    for (int i = 0; i < pontos.size(); i++)
    {
        //percorre todos os pontos da entrada
        bool achou = false; //usa a booleana para verificar se precisa adicionar ao
                            //vetor C de saida ou nao

        for (int j = 0; j < fecho.size(); j++)
        {
            if (pontos[i] == fecho[j])
            {
                //se o ponto pertence ao fecho, nao adicionaremos ao C
                achou = true;
                break;
            }
        }
        if (!achou)
        {
            //se nao achou, é pq nao pertence ao fecho
            C.push_back(pontos[i]);
        }
        achou = false;
    }

    return C;
}

/**
 * 
 * Funcao que determina um possivel caminho para ser percorrido no ciclo, com
 * garantia que percorrera todos os pontos.  
 * O algoritmo nao necessariamente computa a solucao otima, mas sim uma solucao que possa
 * ser computada rapidamente mesmo para entradas grandes.
 * 
 * Complexidade:
 * A complexidade é O((nˆ2)*m), já que a funcao acharMelhorTripla tem complexidade
 * O(n*m), e ela está sendo chamada constantemente dentro de um while que executa n vezes.
 * A funcao pontosForaDoFecho tem complexidade O(n*m), como ela é menor que
 * O((nˆ2)*m), prevalece a complexidade da maior (a do while). 
 * 
 * Corretude:
 * Funciona pois toda vez que achamos a melhor tripla do conjunto, inserimos
 * o ponto correspondente no ciclo, e removemos da lista de pontos que ainda estao fora do ciclo,
 * evitando que seja inserido mais de uma vez. Fazemos isso ate que nao hajam mais pontos na lista.
 * */

vector<Ponto> caixeiroViajante(vector<Ponto> fechoConvexo, vector<Ponto> pontos)
{
    // vector<Ponto> ciclo = quickHull(pontos);
    vector<Ponto> C = pontosForaDoFecho(pontos, fechoConvexo);

    while (C.size() > 0)
    {
        vector<int> tripla = achaMelhorTripla(fechoConvexo, C);
        int indexK = tripla[2];
        int indexI = tripla[0];
        int indexJ = tripla[1];

        //insere ponto no fecho e retira do conjunto C
        fechoConvexo.insert(fechoConvexo.begin() + (indexI + 1), C[indexK]);
        C.erase(C.begin() + indexK);
    }

    return fechoConvexo;
}

/**
 * Função que calcula o custo do ciclo gerado como saida do algoritmo de simulação
 * do caixeiro viajante. Ele executa a função de cálculo da distância euclidiana.
 * 
 * Complexidade:
 * A complexidade da função é O(n), já que a mesma percorre o vetor de Pontos, chamado 
 * de caixeiro e executa uma operação aritmética, adicionando o resultado em um acumulador.
 *
 * Corretude:
 * A função funciona pois a cada ciclo de iteração, a função interna distanciaEuclidiana
 * recebe como parâmetro o ponto atual "i" e o ponto subsequente "i+1", calculando a sua distância.
 * Após o cálculo, "i" passa a ser "i+1", e o processo é repetido até o final do vetor. No final,
 * o resultado é o custo do ciclo.
 * 
 */
float calculaCustoDoCicloComputado(vector<Ponto> caixeiro)
{
    float sum = 0;
    for (int i = 0; i < caixeiro.size() - 1; i++)
    {
        sum += distanciaEuclidiana(caixeiro[i], caixeiro[i + 1]);
    }
    return sum;
}

/**
 * Função que calcula um algoritmo de aproximacao do caixeiro viajante de uma lista de 
 * pontos de um arquivo .txt e salva a resposta em dois arquivos .txt (fecho e ciclo) 
 * de saída e calcula o tempo que o algoritmo demorou para gerar a saída.
 * 
 * A função quickHull está sendo chamada antes do while presente na função caixeiroViajante,
 * tendo complexidade de O(nˆ2). Como ela é menor, prevalece a complexidade da maior
 * (caixeiro viajante), e é O((n^2)*m)
 */
int main(int argc, char **argv)
{
    if (argc != 2 || !arquivoExiste(argv[1]))
    {
        cout << "Uso: ./tsp input_file.txt" << endl;
        return 1;
    }

    vector<Ponto> pontos = lerPontosDoArquivoDeEntrada(argv[1]);

    auto inicio = high_resolution_clock::now();

    vector<Ponto> fechoConvexo = quickHull(pontos);
    vector<Ponto> caixeiro = caixeiroViajante(fechoConvexo, pontos);

    auto fim = high_resolution_clock::now();
    auto tempo = duration_cast<microseconds>(fim - inicio);

    caixeiro.push_back(caixeiro[0]);
    auto custo = calculaCustoDoCicloComputado(caixeiro);

    cout << fixed << std::setprecision(6) << (float)(tempo.count()) / 1000000 << " " << (float)(custo) << endl;

    criarArquivoDeSaida(caixeiro, "ciclo.txt");
    criarArquivoDeSaida(fechoConvexo, "fecho.txt");

    return 0;
}