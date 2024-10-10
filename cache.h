#ifndef __CACHE_H_
#define __CACHE_H_

#include <ctime>
#include <random>

using namespace std;

struct Conjunto;
struct Bloco;

class Cache {
private:
    static int nSets; // Quantidade de conjuntos
    static int bSize; // Tamanho do bloco
    static int assoc; // Associatividade
    static int flag; // Indicador do tipo de exibição de estatísticas
    static int bitsOffset; // Quantidade de bits para offset
    static int bitsIndex; // Quantidade de bits para o índice
    static int bitsTag; // Quantidade de bits para tag
    static int bFree; // Quantidade de blocos livres na cache
    static float missRate; // Taxa de erros
    static float compulsoryRate; // Taxa de erros compulsórios
    static float capacityRate; // Taxa de erros de capacidade
    static float conflictRate; // Taxa de erros de conflito
    static float hitRate; // Taxa de acertos
    static unsigned int totalAccess; // Número total de acessos
    static unsigned int totalMissCompulsory; // Número total de erros compulsórios
    static unsigned int totalMissCapacity; // Número total de erros de capacidade
    static unsigned int totalMissConflict; // Número total de erros de conflito
    static unsigned int totalMisses; // Número total de erros
    static unsigned int totalHits; // Número total de acertos
    static string subst; // Algoritmo de substituição
    static string file; // Nome do arquivo
    static Conjunto *conjunto; // Estrutura da cache

    Cache() {} // Não permite instâncias da cache

public:
    // Inicializa a cache
    static void newCache(const int& conjuntos, const int& bloco, const int& associatividade, const string substituicao, const int& flagImpressao, const string arquivo);

    // Inicializa as estruturas 
    static void initStructs();

    // Libera as estruturas
    static void freeStructs();

    // Executa a simulação da cache
    static void execute();

    // Exibe a configuração da cache
    static void show_config();

    // Verifica se a cache está cheia
    static bool fullCache();

    // Verifica se houve acerto
    static bool isHit(const int& index, const int& tag);

    // Verifica o preenchimento dos blocos da cache
    static bool fullEntries(const int& index, const int& tag);

    // Algoritmo Random
    static void RANDOM(const int& index, const int& tag, mt19937& range);

    // Algoritmo First In First Out
    static void FIFO(const int& index, const int& tag);

    // Algoritimo Least Recently Used
    static void LRU(const int& index, const int& tag);

    // Calcula as estatísticas de execução da cache
    static void log();

    // Exibe as estatísticas de execução da cache
    static void print();

    // Exibe o conteúdo da cache
    static void conteudoCache();
};



#endif // __CACHE_H_