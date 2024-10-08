#include <iostream>
#include <string>
#include <fstream>
#include "cache.h"
using std::ifstream;
using std::ios_base;
using std::cout;
using std::endl;

int Cache::nsets = 0;
int Cache::bsize = 0;
int Cache::assoc = 0;
std::string Cache::file = "";
std::string Cache::subst = "";


// ESTRUTURA DA CACHE
//  _______________________________
// |       |          |            |
// |  TAG  | VALIDADE | INFORMAÇÃO |
// |_______|__________|____________|
// (nesse caso a informaçao não sabemos, então é teórica)
//
// CADA ENDEREÇO LIDO: 32 BITS
// 
// pensei em criar uma estrutura auxiliar para a cache

// com os 4 campos efetivos: tag, validade e em qual via está
// estrutura pra via e estrutura pro conjunto

// a chamada das estruturas no fim vai ser
// Ex. definição: Conjunto conjunto[4]; --> se for por ex. 8 blocos/2 vias
// inicialização: conjunto[0].bloco[0].tag
// inicialização: conjunto[3].bloco[1024].tag


/* 

_______________________________________________________________________________________________

void Cache::execute(){
    ler endereço (leitura chata de arquivo)
    dividir os bits pra tag e validade
    fazer endereço (do arquivo) módulo número de conjuntos (blocos/via) = conjunto correspondente

    buscar em cada entrada do conjunto 
    --> verificar se todas as entradas possuem validade 0
    --> se existirem entradas com validade 1, comparar tag
    --> se nao encontrar, substituir nas posições baseadas na lógica de substituição
    (exemplo 2 conjuntos com 2 vias -> 4 blocos)
    --> podemos criar métodos separados pra cada parte que contenha loop, isso deixaria o código mais limpo e fácil de ler e apresentar


    --> em forma de loop porque podem existir inúmeros blocos em cada conjunto
    wasFound = false;
    (loop) -->verificar hit
    se conjunto[conjunto correspondente].bloco[i].val == true {
        se conjunto[conjunto correspondente].bloco[i].tag == tagEndereco {
            ++hit;
            wasFound = true;
            break;
        }
    }


    --> agora verificar se foi encontrado o endereço na cache, se não, aplicar algoritmo de substituição CASO todas as entradas estejam ocupadas
    se wasFound == false{
        isInvalid = false;
        (loop)
        se conjunto[conjunto correspondente].bloco[i].val == false {
            ++miss compulsório;
            conjunto[conjunto correspondente].bloco[i].tag == tagEndereco;
            conjunto[conjunto correspondente].bloco[i].val == true;
            isInvalid = true; --> significa que foi encontrada uma posição vazia
            break;
        }
    }

    --> confirma que todas as posições estão com o bit de validade true (1)
    se isInvalid == false { 
        (loop)
        --> ...aqui aplicar as lógicas de substituição...
        se Cache::subst == "r" Cache::RANDOM(conjunto correspondente);
        senão se Cache::subst == "f" Cache::FIFO(conjunto correspondente);
        senão se Cache::subst == "l" Cache::LRU(conjunto correspondente);


        --> em cada uma delas apontar o miss de conflito
        --> apontar o miss de capacidade de alguma forma também
    }
    --> garantindo que todas as posições estão preenchidas com valores válidos, podemos aplicar as funções sem que precise fazer novas verificações, daí dá pra aplicar o algoritmo em si.
    _______________________________________________________________________________________________

*/

void Cache::show_config(){
    cout << "\nConjuntos: " << nsets << "\nTamanho do bloco: " << bsize << "\nAssociatividade: " << assoc << "\nSubstituicao: " << subst << "\nArquivo lido: " << file << endl;
}

void Cache::initCache(int& conjuntos, int& bloco, int& associatividade, std::string substituicao, std::string arquivo){ 
    nsets = conjuntos;
    bsize = bloco;
    assoc = associatividade;
    subst = substituicao;
    file = arquivo;
}

struct Bloco {
    string tag;
    bool val;
};

struct Conjunto {
    //vetor de campos [número de conjuntos que for necessário]
    Bloco *campo;
};