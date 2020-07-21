#ifndef TRACE_H
#define TRACE_H

#include <iostream>
#include <cstring>
#include <set>
#include <map>

const int KEY_LEN = 13;
const int VAL_LEN = 8;
const int KV_NUM = 1000000;

struct KVpair{
    char key[KEY_LEN];
    char value[VAL_LEN];
    KVpair(){
        memset(key, 0, sizeof(key));
        memset(value, 0, sizeof(value));
    }
    KVpair(const KVpair& kv){
        memcpy(key, kv.key, sizeof(key));
        memcpy(value, kv.value, sizeof(value));
    }
};

KVpair kvs[KV_NUM];

struct KVcmp{
    bool operator() (const KVpair& kv1, const KVpair& kv2) const{
        return (memcmp(kv1.key, kv2.key, KEY_LEN) < 0);
    }
};

std::map<KVpair, int, KVcmp> kvset;

void loadCaida(const char* filename, int len = KV_NUM){
    FILE* pf = fopen(filename, "rb");
    if(!pf){
        printf("%s cannot found.\n", filename);
        exit(-1);
    }
    int i = 0;
    char tmp[50];
    KVpair kv;
    double beginTime;
    while(fread(tmp, 1, KEY_LEN+VAL_LEN, pf)){
        memcpy(kv.key, tmp, KEY_LEN*sizeof(char));
        memcpy(kv.value, tmp+KEY_LEN, VAL_LEN*sizeof(char));
        if(i == 0)
            beginTime = *(double*)kv.value;
        double nowTime = (*(double*)kv.value - beginTime)*1000000;
        memcpy(kv.value, &nowTime, VAL_LEN*sizeof(char));
        kvs[i++] = kv;
        if(kvset.count(kv) != 0)
            kvset[kv]++;
        else kvset[kv] = 1;
        // kvset.insert(kv);
        if(i%(KV_NUM/10) == 0)
            printf("%7d \t %lf\n", i, *(double*)kv.value);
        if(i >= len)
            break;
    }
    fclose(pf);
    printf("read %d items from CAIDA2018, of which %lu are different.\n", i, kvset.size());
}


#endif
