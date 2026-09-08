#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ostream>
using namespace std;

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