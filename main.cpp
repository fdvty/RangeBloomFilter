#include <iostream>
#include <cstring>
#include "trace.h"
#include "rangebf.h"

using namespace std;

int main(){
    loadCaida("./130000.dat"); 

    printf("-----------------------------------------------------------------\n");
    RangeBF rbf(2200000, 3);
    for(int i = 0; i < KV_NUM; ++i)
        rbf.insert(kvs[i]);
    auto it = kvset.begin();
    for(int i = 0; i < 10; ++i){
        set<pair<int, int>> ret = rbf.query(it->first.key);
        printf("flow %d, freq %d, time %lf:\n", i, it->second, *(double*)it->first.value);
        for(auto itt = ret.begin(); itt != ret.end(); ++itt){
            printf("ans:[%d, %d]\n", itt->first, itt->second);
        }
        ++it;
    }

    printf("-----------------------------------------------------------------\n");
    int count = 0;
    for(int i = 0; i < rbf.size; ++i)
        if(rbf.bf[i] == 3)
            count++;
    printf("bilateral cell ratio: %lf\n", (double)count/rbf.size);
    return 0;
}