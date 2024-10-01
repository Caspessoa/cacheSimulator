#ifndef __CACHE_H_
#define __CACHE_H_

#include <string>

class Cache {
private:
    static int nsets;
    static int bsize;
    static int assoc;
    static std::string subst;
    static std::string file;
    Cache() {} //não permite instâncias da cache

public:
    static void initCache(int& conjuntos, int& bloco, int& associatividade, std::string substituicao, std::string arquivo);

    // static void execute();
    static void show_config();
}; 

#endif // __CACHE_H_