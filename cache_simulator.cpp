#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <typeinfo>
#include <fstream>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::stoi;
using std::string; //nao usei ainda
using std::ifstream;
using std::ios_base;
using std::exception; //nao usei ainda
using std::invalid_argument;
using std::out_of_range;


void isValidArguments(int argc, char **argv){
    if (argc != 7) {
        throw invalid_argument("Quantidade de argumentos incorreta! Utilize:\n./cache_simulator <nsets> <bsize> <assoc> <substituicao> <flag_saida> enderecos/arquivo_de_entrada\n");
    } else if (stoi(argv[1]) < 1 || stoi(argv[2]) < 1 || stoi(argv[3]) < 1){
        throw invalid_argument("Valor inválido em algum dos parâmetros da cache (nsets, bsize ou assoc)\n");
    } else if (argv[4][0] != 'r' && argv[4][0] != 'l' && argv[4][0] != 'f'){
        throw invalid_argument("Politica de substituicao invalida\n");
    } else if (stoi(argv[5]) != 0 && stoi(argv[5]) != 1){
        throw invalid_argument("Valor inválido para flag de impressão. Insira apenas 0 ou 1 no sexto campo do argumento\n");
    }
}

void isValidFile(char **argv){
    ifstream arquivo;
    
    try{   
        arquivo.exceptions(ifstream::failbit | ifstream::badbit);
        arquivo.open(argv[6]);
        arquivo.close();
    } catch (const ios_base::failure &e){
        cerr << "Erro ao abrir ou ler o arquivo: " << e.what() << endl;
        exit(1);
    }
}


int main(int argc, char **argv){
    isValidArguments(argc, argv);
    //cout << "Test 1 passed!\n";
    isValidFile(argv);
    //cout << "Test 2 passed!\n";

    //criar uma cache que execute os passos fazendo as verificações necessárias
    //fazer ela retornar valores-alvo de impressao aqui na main

    return 0;
}