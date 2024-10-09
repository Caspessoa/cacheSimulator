#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <vector>
#include <cmath>
#include "cache.h"

using namespace std;

int Cache::nSets = 0;
int Cache::bSize = 0;
int Cache::assoc = 0;
int Cache::flag = 0;
int Cache::bitsOffset = 0;
int Cache::bitsIndex = 0;
int Cache::bitsTag = 0; //bits p tag
float Cache::missRate = 0.0; //miss
float Cache::compulsoryRate = 0.0; //miss compulsório
float Cache::capacityRate = 0.0; //miss capacidade
float Cache::conflictRate = 0.0; //miss conflito
float Cache::hitRate = 0.0; //hit
unsigned int Cache::totalAccess = 0;
unsigned int Cache::totalMissCompulsory = 0;
unsigned int Cache::totalMissCapacity = 0;
unsigned int Cache::totalMissConflict = 0;
unsigned int Cache::totalHits = 0;
unsigned int Cache::totalMisses = 0;
string Cache::file = "";
string Cache::subst = "";
Conjunto* Cache::conjunto = nullptr;


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




void Cache::execute(){
    ifstream data(file, ios::binary); // pra ler arquivo binário em modo binário
    if (data)
    {
        int address;
        while (data.read(reinterpret_cast<char*>(&address), sizeof(int)))
        {
            int tag = address >> (bitsOffset + bitsIndex);
            int index = (address >> bitsOffset) & ((1 << bitsIndex) - 1);
            totalAccess++;
            if (!isHit(index, tag)){
                if (!validEntries(index, tag)){
                    if (subst == "r"){
                        RANDOM(index);
                    } else if (subst == "f") {
                        FIFO(index);
                    } else if (subst == "l"){
                        LRU(index);
                    } //identificar cada tipo de miss e adicionar acessos
                }
            }
        }
        data.close();  // Fecha o arquivo
    }

}
    // dividir os bits pra tag e validade
    // fazer endereço (do arquivo) módulo número de conjuntos (blocos/via) = conjunto correspondente

    // buscar em cada entrada do conjunto 
    // --> verificar se todas as entradas possuem validade 0
    // --> se existirem entradas com validade 1, comparar tag
    // --> se nao encontrar, substituir nas posições baseadas na lógica de substituição
    // (exemplo 2 conjuntos com 2 vias -> 4 blocos)
    // --> podemos criar métodos separados pra cada parte que contenha loop, isso deixaria o código mais limpo e fácil de ler e apresentar



    //(MÉTODO ISHIT)
    // --> em forma de loop porque podem existir inúmeros blocos em cada conjunto
    // wasFound = false;
    // (loop) -->verificar hit
    // se conjunto[conjunto correspondente].bloco[i].val == true {
    //     se conjunto[conjunto correspondente].bloco[i].tag == tagEndereco {
    //         ++hit;
    //         wasFound = true;
    //         break;
    //     }
    // }



    // (MÉTODO VALIDENTRIES)
    // --> agora verificar se foi encontrado o endereço na cache, se não, aplicar algoritmo de substituição CASO todas as entradas estejam ocupadas
    // se wasFound == false{
    //     isInvalid = false;
    //     (loop)
    //     se conjunto[conjunto correspondente].bloco[i].val == false {
    //         ++miss compulsório;
    //         conjunto[conjunto correspondente].bloco[i].tag == tagEndereco;
    //         conjunto[conjunto correspondente].bloco[i].val == true;
    //         isInvalid = true; --> significa que foi encontrada uma posição vazia
    //         break;
    //     }
    // }


    // (MÉTODO PROCESSADRESS)
    // --> confirma que todas as posições estão com o bit de validade true (1)
    // se isInvalid == false { 
    //     (loop)
    //     --> ...aqui aplicar as lógicas de substituição...
    //     se Cache::subst == "r" Cache::RANDOM(conjunto correspondente);
    //     senão se Cache::subst == "f" Cache::FIFO(conjunto correspondente);
    //     senão se Cache::subst == "l" Cache::LRU(conjunto correspondente);


    //     --> em cada uma delas apontar o miss de conflito
    //     --> apontar o miss de capacidade de alguma forma também
    // }
    // --> garantindo que todas as posições estão preenchidas com valores válidos, podemos aplicar as funções sem que precise fazer novas verificações, daí dá pra aplicar o algoritmo em si.

    // -->por fim, printar os resultados com base no flag
// }
// void Cache::execute(){

// }




void Cache::show_config(){
    cout << "\nConjuntos: " << nSets << "\nTamanho do bloco: " << bSize << "\nAssociatividade: " << assoc << "\nSubstituicao: " << subst << "\nArquivo lido: " << file << endl;
}

void Cache::newCache(const int& conjuntos, const int& bloco, const int& associatividade, const string substituicao, const int& flagImpressao, const string arquivo){ 
    nSets = conjuntos;
    bSize = bloco;
    assoc = associatividade;
    subst = substituicao;
    flag = flagImpressao;
    file = arquivo;
    bitsOffset = log2(bSize);
    bitsIndex = log2(nSets);
    bitsTag = 32 - bitsOffset - bitsIndex;
    initStructs();
}

struct Bloco {
    int tag;
    bool val;
    unsigned int priority;
};

struct Conjunto {
    //vetor de campos [número de conjuntos que for necessário]
    Bloco *bloco;
};

// string Cache::readAdress(fstream arquivo){
    
// }

int Cache::calculaConjunto(const int& adress){
    // cout << "Conjunto referente ao endereco " << adress << ": "<< (adress % nsets) << endl; //debug

    return adress % nSets;
}

void Cache::initStructs(){
    conjunto = new Conjunto[nSets];

    for (int i = 0; i < nSets; ++i){
        conjunto[i].bloco = new Bloco[assoc];
        for (int j = 0; j < bSize; ++j){
            conjunto[i].bloco[j].tag = 0;
            conjunto[i].bloco[j].val = false;
        }
    }
}
void Cache::freeStructs(){
    for (int i = 0; i < nSets; ++i) {
        delete[] conjunto[i].bloco;
    }
    delete[] conjunto;
    conjunto = nullptr;
}

bool Cache::isHit(const int& adressGroup, const int& tagAdress){
    for (int i = 0; i < assoc; ++i){
        if (conjunto[adressGroup].bloco[i].val == true){
            if (conjunto[adressGroup].bloco[i].tag == tagAdress){
                ++totalHits;
                return true;
            }
        }
    }
    return false;
}

bool Cache::validEntries(const int& adressGroup, const int& tagAdress){
    //utilizar o isHit antes, usando este como "else" subsequente
    for (int i = 0; i < assoc; ++i){
        if (conjunto[adressGroup].bloco[i].val == false){
            ++totalMissCompulsory;
            conjunto[adressGroup].bloco[i].tag = tagAdress;
            conjunto[adressGroup].bloco[i].val = true;
            //adicionar acessos
            return true;
        }
    }
    return false;
}


void Cache::print(){
    if (flag == 0){
        cout << "\t\t__________________________________________________\n\t\t__________________________________________________\n\t\tRESULTADOS                                             \n\t\t__________________________________________________\n\t\t__________________________________________________\n";
        cout << "\n\t\t--------------------------------------------------\n";
        cout <<   "\t\t                     TAXAS                        \n";
        cout <<   "\t\t--------------------------------------------------\n\n";
        cout << "\t\t[taxa de hits]. . . . . . . . . . . . . . . . . " << hitRate << endl;
        cout << "\t\t[taxa de miss]. . . . . . . . . . . . . . . . . " << missRate << endl;
        cout << "\t\t[taxa de miss compulsorios] . . . . . . . . . . " << compulsoryRate << endl;
        cout << "\t\t[taxa de miss de capacidade]. . . . . . . . . . " << capacityRate << endl;
        cout << "\t\t[taxa de miss de conflito]  . . . . . . . . . . " << conflictRate << "\n\n\n";
        cout << "\n\t\t--------------------------------------------------\n";
        cout <<   "\t\t                    ACESSOS                       \n";
        cout <<   "\t\t--------------------------------------------------\n\n";
        cout << "\t\t[total de acessos]. . . . . . . . . . . . . . . " << totalAccess << endl;
        cout << "\t\t[acessos a cache] . . . . . . . . . . . . . . . " << "none" << endl;
        cout << "\t\t[acessos a memoria principal] . . . . . . . . . " << "none" << endl;
        cout << "\n\t\t--------------------------------------------------\n";
        cout <<   "\t\t                VALORES ABSOLUTOS                 \n";
        cout <<   "\t\t--------------------------------------------------\n\n";
        cout << "\t\t[hits totais] . . . . . . . . . . . . . . . . . " << totalHits << endl;
        cout << "\t\t[misses totais] . . . . . . . . . . . . . . . . " << totalMisses << endl;
        cout << "\t\t[misses compulsorios] . . . . . . . . . . . . . " << totalMissCompulsory << endl;
        cout << "\t\t[misses de capacidade]. . . . . . . . . . . . . " << totalMissCapacity << endl;
        cout << "\t\t[misses de conflito]. . . . . . . . . . . . . . " << totalMissConflict << "\n\n" << endl;
        
        return;
    }
    
    // else
    cout << "\n" << totalAccess << ", " << hitRate << ", " << missRate << ", " << compulsoryRate << ", " << capacityRate << ", " << conflictRate << "\n" << endl;
}

void Cache::RANDOM(const int& adressGroup){

}
// aplica a substituição fifo
void Cache::FIFO(const int& adressGroup){

}
// aplica a substituição last recently used
void Cache::LRU(const int& adressGroup){

}