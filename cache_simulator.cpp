#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <typeinfo>
#include <fstream>
#include "cache.h"

using namespace std;

void isValidArguments(int argc, char **argv);

int main(int argc, char **argv){
    isValidArguments(argc, argv);

    int conjuntos = stoi(argv[1]);
    int bloco = stoi(argv[2]);
    int associatividade = stoi(argv[3]);
    int flag = stoi(argv[5]);
    string substituicao = argv[4];
    string arquivo = argv[6];

    Cache::newCache(conjuntos, bloco, associatividade, substituicao, flag, arquivo);
    Cache::execute();
    Cache::log();
    Cache::print();

    return 0;
}

void isValidArguments(int argc, char **argv){
    if (argc != 7) {
        throw invalid_argument("Quantidade de argumentos incorreta! Utilize:\n./cache_simulator <nsets> <bsize> <assoc> <substituicao> <flag_saida> arquivo_de_entrada\n");
    } else if (stoi(argv[1]) < 1 || stoi(argv[2]) < 1 || stoi(argv[3]) < 1){
        throw invalid_argument("Valor inválido em algum dos parâmetros da cache (nsets, bsize ou assoc)\n");
    } else if (argv[4][0] != 'r' && argv[4][0] != 'l' && argv[4][0] != 'f'){
        throw invalid_argument("Politica de substituicao invalida\n");
    } else if (stoi(argv[5]) != 0 && stoi(argv[5]) != 1 && stoi(argv[5]) != 2){
        throw invalid_argument("Valor inválido para flag de impressão. Insira apenas 0 ou 1 no sexto campo do argumento\n");
    }
}