#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <bitset>
#include <vector>
#include <cmath>
#include <random>
#include <ctime>
#include "cache.h"

using namespace std;

int Cache::nSets = 0;
int Cache::bSize = 0;
int Cache::assoc = 0;
int Cache::flag = 0;
int Cache::bitsOffset = 0;
int Cache::bitsIndex = 0;
int Cache::bitsTag = 0;
int Cache::bFree = 0;
float Cache::missRate = 0.0;
float Cache::compulsoryRate = 0.0;
float Cache::capacityRate = 0.0;
float Cache::conflictRate = 0.0;
float Cache::hitRate = 0.0;
string Cache::file = "";
string Cache::subst = "";
Conjunto* Cache::conjunto = nullptr;
unsigned int Cache::totalAccess = 0;
unsigned int Cache::totalMissCompulsory = 0;
unsigned int Cache::totalMissCapacity = 0;
unsigned int Cache::totalMissConflict = 0;
unsigned int Cache::totalHits = 0;
unsigned int Cache::totalMisses = 0;

int swap_Endians(int value); // Altera o endereço de Big Endian pra Little Endian

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

void Cache::execute(){
    // Opera o arquivo em modo binário
    ifstream data(file, ios::binary);
    mt19937 range(random_device{}());
    if (data)
    {
        int address;
        while (data.read(reinterpret_cast<char*>(&address), sizeof(int)))
        {
            address = swap_Endians(address);
            int tag = address >> (bitsOffset + bitsIndex);
            int index = (address >> bitsOffset) & (nSets - 1);
            totalAccess++;
            if (!isHit(index, tag)){
                if (fullEntries(index, tag)){
                    if (subst == "r"){
                        RANDOM(index, tag, range);
                    } else if (subst == "f") {
                        FIFO(index, tag);
                    } else if (subst == "l"){
                        LRU(index, tag);
                    }
                }
            }
        }
        data.close(); // Fecha o arquivo
    }
}

void Cache::show_config(){
    cout <<
    "\nConjuntos: " << nSets << 
    "\nTamanho do bloco: " << bSize << 
    "\nAssociatividade: " << assoc << 
    "\nSubstituicao: " << subst << 
    "\nArquivo lido: " << file << 
    endl;
}



struct Bloco {
    int tag;
    bool val;
};

struct Conjunto {
    Bloco *bloco;
    unsigned int nextBlock; //para FIFO
    vector<int> priorityLRU; 
};

void Cache::initStructs(){
    conjunto = new Conjunto[nSets];

    for (int i = 0; i < nSets; ++i){
        conjunto[i].bloco = new Bloco[assoc];

        //inicializa o próximo bloco a ser substituido pelo FIFO como 0
        conjunto[i].nextBlock = 0;
        for (int j = 0; j < assoc; ++j){
            conjunto[i].bloco[j].tag = 0;
            conjunto[i].bloco[j].val = false;

            //organiza a ordem do LRU com o índice dos blocos
            conjunto[i].priorityLRU.push_back(j);
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

bool Cache::isHit(const int& index, const int& tag){
    for (int i = 0; i < assoc; ++i){
        if (conjunto[index].bloco[i].val == true){
            if (conjunto[index].bloco[i].tag == tag){

                //encontra o indice (do tipo iterator) do bloco que recém foi acessado
                auto iterador = find(conjunto[index].priorityLRU.begin(), conjunto[index].priorityLRU.end(), i);

                //evita o acesso a elementos que não existem
                if (iterador != conjunto[index].priorityLRU.end()){
                    //atualiza a ordem do LRU
                    conjunto[index].priorityLRU.erase(iterador);
                    conjunto[index].priorityLRU.push_back(i);
                }

                ++totalHits;
                return true;
            }
        }
    }
    return false;
}

bool Cache::fullEntries(const int& index, const int& tag){
    for (int i = 0; i < assoc; ++i){
        if (conjunto[index].bloco[i].val == false){
            ++totalMissCompulsory;
            conjunto[index].bloco[i].tag = tag;
            conjunto[index].bloco[i].val = true;
            return false;
        }
    }
    return true;
}

void Cache::log(){
    totalMisses = totalMissCapacity + totalMissCompulsory + totalMissConflict;
    hitRate = (float)totalHits / (float)totalAccess;
    missRate = (float)totalMisses / (float)totalAccess;
    compulsoryRate = (float)totalMissCompulsory / (float)totalMisses;
    capacityRate = (float)totalMissCapacity / (float)totalMisses;
    conflictRate = (float)totalMissConflict / (float)totalMisses;
}

void Cache::RANDOM(const int& index, const int& tag, std::mt19937& range){
    uniform_int_distribution<int> distribution(0, assoc - 1);
    int randomValue = distribution(range);

    conjunto[index].bloco[randomValue].tag = tag;

    if(fullCache()){
        ++totalMissCapacity;
    } else ++totalMissConflict;
    
}

// aplica a substituição fifo
void Cache::FIFO(const int& index, const int& tag){
    Conjunto& actualSet = conjunto[index];
    Bloco& substBlock = actualSet.bloco[actualSet.nextBlock];
    
    substBlock.tag = tag;

    actualSet.nextBlock = (actualSet.nextBlock + 1) % assoc;

    
    if(fullCache()){
        ++totalMissCapacity;
    } else ++totalMissConflict;
    
}

// aplica a substituição least recently used
void Cache::LRU(const int& index, const int& tag){
    //conjunto atual
    Conjunto& actualSet = conjunto[index];

    //primeiro bloco é o menos recentemente usado
    int substBlockIndex = actualSet.priorityLRU.front();
    Bloco& substBlock = actualSet.bloco[substBlockIndex];

    //atualiza a tag do bloco da cache
    substBlock.tag = tag;

    //atualiza a lista LRU
    actualSet.priorityLRU.erase(actualSet.priorityLRU.begin());
    actualSet.priorityLRU.push_back(substBlockIndex);
    
    if(fullCache()){
        ++totalMissCapacity;
    } else ++totalMissConflict;

}

bool Cache::fullCache(){
    if (totalMissCompulsory == (nSets * assoc)) return true;
    return false;
}

void Cache::print(){
    if (flag == 0){
        cout << "\t\t__________________________________________________\n\t\t__________________________________________________\n\t\tRESULTADOS                                             \n\t\t__________________________________________________\n\t\t__________________________________________________\n";
        cout << "\n\t\t--------------------------------------------------\n";
        cout <<   "\t\t                     TAXAS                        \n";
        cout <<   "\t\t--------------------------------------------------\n\n";
        printf(   "\t\t[taxa de hits]. . . . . . . . . . . . . . . %.4f\n", hitRate);
        printf(   "\t\t[taxa de miss]. . . . . . . . . . . . . . . %.4f\n", missRate);
        printf(   "\t\t[taxa de miss compulsorios] . . . . . . . . %.4f\n", compulsoryRate);
        printf(   "\t\t[taxa de miss de capacidade]. . . . . . . . %.4f\n", capacityRate);
        printf(   "\t\t[taxa de miss de conflito]  . . . . . . . . %.4f\n\n", conflictRate);
        cout << "\n\t\t--------------------------------------------------\n";
        cout <<   "\t\t                    ACESSOS                       \n";
        cout <<   "\t\t--------------------------------------------------\n\n";
        cout << "\t\t[total de acessos] . . . . . . . . . . . . . . " << totalAccess << endl;
        cout << "\n\t\t--------------------------------------------------\n";
        cout <<   "\t\t                VALORES ABSOLUTOS                 \n";
        cout <<   "\t\t--------------------------------------------------\n\n";
        cout << "\t\t[hits totais] . . . . . . . . . . . . . . . . . " << totalHits << endl;
        cout << "\t\t[misses totais] . . . . . . . . . . . . . . . . " << totalMisses << endl;
        cout << "\t\t[misses compulsorios] . . . . . . . . . . . . . " << totalMissCompulsory << endl;
        cout << "\t\t[misses de capacidade]. . . . . . . . . . . . . " << totalMissCapacity << endl;
        cout << "\t\t[misses de conflito]. . . . . . . . . . . . . . " << totalMissConflict << "\n\n" << endl
    } else if (flag == 1) {
        printf("\n%d, %.2f, %.2f, %.2f, %.2f, %.2f\n", totalAccess, hitRate, missRate, compulsoryRate, capacityRate, conflictRate);
    } else if (flag == 2) {
        cout << "\t\t__________________________________________________\n\t\t__________________________________________________\n\t\t                  CONFIGURACAO                                             \n\t\t__________________________________________________\n\t\t__________________________________________________\n" <<
        "\n\t\tConjuntos: " << nSets << 
        "\n\t\tTamanho do bloco: " << bSize << 
        "\n\t\tAssociatividade: " << assoc << 
        "\n\t\tSubstituicao: " << subst << 
        "\n\t\tArquivo lido: " << file << 
        endl << endl;
        cout << "\t\t__________________________________________________\n\t\t__________________________________________________\n\t\tRESULTADOS                                             \n\t\t__________________________________________________\n\t\t__________________________________________________\n";
        cout << "\n\t\t--------------------------------------------------\n";
        cout <<   "\t\t                     TAXAS                        \n";
        cout <<   "\t\t--------------------------------------------------\n\n";
        printf(   "\t\t[taxa de hits]. . . . . . . . . . . . . . . %.4f\n", hitRate);
        printf(   "\t\t[taxa de miss]. . . . . . . . . . . . . . . %.4f\n", missRate);
        printf(   "\t\t[taxa de miss compulsorios] . . . . . . . . %.4f\n", compulsoryRate);
        printf(   "\t\t[taxa de miss de capacidade]. . . . . . . . %.4f\n", capacityRate);
        printf(   "\t\t[taxa de miss de conflito]  . . . . . . . . %.4f\n\n", conflictRate);
        cout << "\n\t\t--------------------------------------------------\n";
        cout <<   "\t\t                    ACESSOS                       \n";
        cout <<   "\t\t--------------------------------------------------\n\n";
        cout << "\t\t[total de acessos] . . . . . . . . . . . . . . " << totalAccess << endl;
        cout << "\n\t\t--------------------------------------------------\n";
        cout <<   "\t\t                VALORES ABSOLUTOS                 \n";
        cout <<   "\t\t--------------------------------------------------\n\n";
        cout << "\t\t[hits totais] . . . . . . . . . . . . . . . . . " << totalHits << endl;
        cout << "\t\t[misses totais] . . . . . . . . . . . . . . . . " << totalMisses << endl;
        cout << "\t\t[misses compulsorios] . . . . . . . . . . . . . " << totalMissCompulsory << endl;
        cout << "\t\t[misses de capacidade]. . . . . . . . . . . . . " << totalMissCapacity << endl;
        cout << "\t\t[misses de conflito]. . . . . . . . . . . . . . " << totalMissConflict << "\n\n" << endl << endl;
        conteudoCache();
    }
    // else
}

void Cache::conteudoCache(){
    for (int i = 0; i < nSets; ++i){
        for (int j = 0; j < assoc; ++j){ 
            printf("Indice [%d] Bloco [%d]: tag -> %d val -> %d\n", i, j, conjunto[i].bloco[j].tag, conjunto[i].bloco[j].val);
        }
        printf("\n");
    }
}

int swap_Endians(int value){ 
    int leftmost_byte; 
    int left_middle_byle; 
    int right_middle_byte; 
    int rightmost_byte; 
    int result; 
 
    leftmost_byte = (value & 0x000000FF) >> 0; 

    left_middle_byle = (value & 0x0000FF00) >> 8; 
 
    right_middle_byte = (value & 0x00FF0000) >> 16; 
 
    rightmost_byte = (value & 0xFF000000) >> 24; 

    leftmost_byte <<= 24; 
 
    left_middle_byle <<= 16; 
 
    right_middle_byte <<= 8; 
 
    rightmost_byte <<= 0; 
 
    result = (leftmost_byte | left_middle_byle | right_middle_byte | rightmost_byte); 
 
    return result; 
} 