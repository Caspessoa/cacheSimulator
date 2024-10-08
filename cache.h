#ifndef __CACHE_H_
#define __CACHE_H_

// #include <string>

using std::string;

struct Conjunto;

struct Bloco;

class Cache {
private:
    static int nsets;
    static int bsize;
    static int assoc;
    static int flag;
    static float missrate; //miss
    static float compulsory; //miss compulsório
    static float capacity; //miss capacidade
    static float conflict; //miss conflito
    static float hitrate; //hit
    static unsigned int totalAcess;
    static string subst;
    static string file;
    Cache() {} //não permite instâncias da cache

public:
    // inicializa
    static void initCache(const int& conjuntos, const int& bloco, const int& associatividade, const string substituicao, const int& flagImpressao, const string arquivo);

    // faz a execução
    static void execute();

    // mostra as configs
    static void show_config();

    // extrai a tag do endereco
    static string extraiTag(const string adress);

    // retorna o cálculo do conjunto-alvo
    static int calculaConjunto(const string adress);

    // converte string binário para decimal usando a bib <bitset>
    static unsigned long stod(const string adress);

    // verifica o hit (adressGroup -> conjunto)
    // true -> encontrou
    static bool isHit(const int& adressGroup, const string tagAdress);

    // verifica se todas as entradas do conjunto estão ocupadas por bits válidos
    // true -> são válidas
    static bool validEntries(const int& adressGroup);

    // processa o endereco para a cache utilizando as verificacoes,
    // funções anteriores e políticas de substituicao
    static void processAdress(const string adress);

    // aplica a substituição random
    static void RANDOM(const int& adressGroup);
    // aplica a substituição fifo
    static void FIFO(const int& adressGroup);
    // aplica a substituição last recently used
    static void LRU(const int& adressGroup);

    // lê o endereco binário de uma linha do arquivo
    static string readAdress(ifstream arquivo);

    // printa os resultados com base no flag interno
    static void print();
};



#endif // __CACHE_H_