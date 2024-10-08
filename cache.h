#ifndef __CACHE_H_
#define __CACHE_H_

#include <string>

using std::string;

struct Conjunto;

class Cache {
private:
    static int nsets;
    static int bsize;
    static int assoc;
    static string subst;
    static string file;
    Cache() {} //não permite instâncias da cache

public:
    static void initCache(int& conjuntos, int& bloco, int& associatividade, string substituicao, string arquivo);

    // static void execute();
    static void show_config();
};



#endif // __CACHE_H_