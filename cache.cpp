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


// void Cache::execute(){
    
// }

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