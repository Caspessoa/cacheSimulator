#ifndef __CACHE_H_
#define __CACHE_H_

#include <string>
#include <fstream>

using namespace std;
using std::string;
using std::fstream;

struct Conjunto;

struct Bloco;

class Cache {
private:
    static int nSets;
    static int bSize;
    static int assoc;
    static int flag;
    static int bitsOffset;
    static int bitsIndex;
    static int bitsTag;
    static float missRate; //miss
    static float compulsoryRate; //miss compulsório
    static float capacityRate; //miss capacidade
    static float conflictRate; //miss conflito
    static float hitRate; //hit
    static unsigned int totalAccess;
    static unsigned int totalMissCompulsory;
    static unsigned int totalMissCapacity;
    static unsigned int totalMissConflict;
    static unsigned int totalMisses;
    static unsigned int totalHits;
    static string subst;
    static string file;
    static Conjunto *conjunto;
    Cache() {} //não permite instâncias da cache

public:
    // inicializa
    static void newCache(const int& conjuntos, const int& bloco, const int& associatividade, const string substituicao, const int& flagImpressao, const string arquivo); //feito

    // inicializa os campos das estruturas 
    static void initStructs(); //feito

    static void freeStructs(); //feito

    // faz a execução
    static void execute();

    // mostra as configs
    static void show_config(); //feito

    // extrai a tag do endereco
    static string extraiTag(const int& adress); //feito

    // compara tags A com B
    static bool AeqB(const string& A, const string& B); //feito

    // retorna o cálculo do conjunto-alvo
    static int calculaConjunto(const int& adress); //feito

    // converte string binário para decimal usando a bib <bitset>
    static unsigned long stod(const int& adress);

    // verifica o hit (adressGroup -> conjunto)
    // true -> encontrou
    static bool isHit(const int& adressGroup, const int& tagAdress);

    // verifica se todas as entradas do conjunto estão ocupadas por bits válidos
    // true -> são válidas
    static bool validEntries(const int& adressGroup, const int& tagAdress);

    // processa o endereco para a cache utilizando as verificacoes,
    // funções anteriores e políticas de substituicao

    // aplica a substituição random
    static void RANDOM(const int& adressGroup);
    // aplica a substituição fifo
    static void FIFO(const int& adressGroup);
    // aplica a substituição last recently used
    static void LRU(const int& adressGroup);

    // // lê o endereco binário de uma linha do arquivo
    // static string readAdress(fstream arquivo);

    // printa os resultados com base no flag interno
    static void print();
};



#endif // __CACHE_H_