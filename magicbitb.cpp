#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <ostream>
using namespace std;

#define USE_32_BIT_MULTIPLICATIONS

//creates a random 64 bit number
uint64_t random_uint64(){
    uint64_t u1, u2, u3, u4;

    u1 = (uint64_t)(rand()) & 0xFFFF; u2 = (uint64_t)(rand()) & 0xFFFF;
    u3 = (uint64_t)(rand()) & 0xFFFF; u4 = (uint64_t)(rand()) & 0xFFFF;

    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

//makes low non zero bit random number
uint64_t random_uint64_fewbits(){
    return random_uint64() & random_uint64() & random_uint64();
}

int count_1s(uint64_t b){
    int r;
    for(r = 0; b; r++, b &= b - 1);
    return r;
}

const int BitTable[64] = {
    64, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
    51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
    26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
    58, 20, 37, 17, 36, 8
};

int pop_1st_bit(uint64_t* bb){
    uint64_t b = *bb ^ (*bb - 1);
    unsigned int fold = (unsigned)( (b & 0xffffffff) ^ (b >> 32));
    *bb &= (*bb - 1);
    return BitTable[(fold * 0x783a9b23) >> 26];
}

uint64_t index_to_uint64(int index, int bits, uint64_t m){
    int i, j;
    uint64_t result = 0;
    
    for(i = 0; i < bits; i++){
        j = pop_1st_bit(&m);
        if( index & (1 << i) ){ result |= (uint64_t)1 << j; }
    }

    return result;
}

uint64_t rmask(int square){
    uint64_t atk = 0;
    int tfile = square % 8;
    int trank = square / 8;

    for(int r = trank + 1; r <= 6; r++){ atk |= ((uint64_t)1 << (r * 8 + tfile)); }
    for(int r = trank - 1; r >= 1; r--){ atk |= ((uint64_t)1 << (r * 8 + tfile)); }
    for(int f = tfile + 1; f <= 6; f++){ atk |= ((uint64_t)1 << (f + trank * 8)); }
    for(int f = tfile - 1; f >= 1; f--){ atk |= ((uint64_t)1 << (f + trank * 8)); }

    return atk;
}

uint64_t bmask(int square){
    uint64_t atk = 0;
    int tfile = square % 8;
    int trank = square / 8;

    for(int r = trank + 1, f = tfile + 1; r <= 6 && f <= 6; r++, f++){ atk |= ((uint64_t)1 << (r * 8 + f)); }
    for(int r = trank + 1, f = tfile - 1; r <= 6 && f >= 1; r++, f--){ atk |= ((uint64_t)1 << (r * 8 + f)); }
    for(int r = trank - 1, f = tfile + 1; r >= 1 && f <= 6; r--, f++){ atk |= ((uint64_t)1 << (r * 8 + f)); }
    for(int r = trank - 1, f = tfile - 1; r >= 1 && f >= 1; r--, f--){ atk |= ((uint64_t)1 << (r * 8 + f)); }

    return atk;
}

uint64_t qmask(int square){
    return bmask(square) | rmask(square);
}