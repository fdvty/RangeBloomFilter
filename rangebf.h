#ifndef RANGEBF_H
#define RANGEBF_H

#include <iostream>
#include <cstring>
#include "BOBHash32.h"
#include "trace.h"


struct RangeBF{
    int size;
    int K;                
    int *bf;
    BOBHash32 *hashes;

    RangeBF(int size_, int K_){
        size = size_;
        K = K_;
        hashes = new BOBHash32[K];
        srand((int)(time(NULL)));
        for(int i = 0; i < K; ++i){
            uint32_t seed = rand()%MAX_PRIME32;
            hashes[i].initialize(seed);
        }
        bf = new int[size];
    }
    ~RangeBF(){
        delete [] bf;
        delete [] hashes;
    }

    int hash(const char* key, int k, int left, int right){
        int range = right - left;
        if(range == 0) return left;
        return (hashes[k].run(key, KEY_LEN*sizeof(char)) % range) + left;
    }

    void insert(const KVpair& kv){
        const char* key = kv.key;
        double value = *(double*)kv.value;
        int left = 0;
        int right = size-1;
        for(int k = 0; k < K; ++k){
            int position = hash(key, k, left, right);
            if(value < position){
                switch (bf[position]){
                    case 1: bf[position] = 3; break;
                    case 3: break;
                    default: bf[position] = 2; break;
                }
                right = max(0, position-1);
            }
            else{
                switch (bf[position]){
                    case 2: bf[position] = 3; break;
                    case 3: break;
                    default: bf[position] = 1; break;
                }
                left = min(size-1, position+1);
            }
        }
    }

    set<pair<int, int>> query(const char* key, int k = 0, int left = 0, int right = -1){
        if(right == -1)
            right = size-1;
        set<pair<int, int>> ret;
        if(k == K){
            ret.insert(make_pair(left, right+1));
            return ret;
        }
        set<pair<int, int>> left_ret, right_ret;
        int position = hash(key, k, left, right);
        if(bf[position] == 1 || bf[position] == 3){
            int left_ = min(size-1, position+1);
            right_ret = query(key, k+1, left_, right);
        }
        if(bf[position] == 2 || bf[position] == 3){
            int right_ = max(0, position-1);
            left_ret = query(key, k+1, left, right_);
        }
        // if bf[position] == 0, return empty set
        ret.insert(left_ret.begin(), left_ret.end());
        ret.insert(right_ret.begin(), right_ret.end());
        return ret;
    }
};



#endif