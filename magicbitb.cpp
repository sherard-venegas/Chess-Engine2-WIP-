#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include "conv_needed.hpp"
using namespace std;

#define USE_32_BIT_MULTIPLICATIONS

//creates a psuedo random 64 bit number
uint64_t random_uint64(){
    uint64_t u1, u2, u3, u4;

    u1 = (uint64_t)(rand()) & 0xFFFF; u2 = (uint64_t)(rand()) & 0xFFFF;
    u3 = (uint64_t)(rand()) & 0xFFFF; u4 = (uint64_t)(rand()) & 0xFFFF;

    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

//makes low non zero bit psuedo random number by ceating multiple random numbers and doing a bitwise &, serves as a candidate magic number
uint64_t random_uint64_fewbits(){
    return random_uint64() & random_uint64() & random_uint64();
}

//counts how many bits are 1 in a 64 bit integer, basically counts and the lsb and gets rid of it
int count_1s(uint64_t b){
    int r;
    for(r = 0; b; r++, b &= b - 1);
    return r;
}

//removes the LSB and returns the corresponding square / index of the bit
int pop_1st_bit(uint64_t* bb){
    uint64_t b = *bb ^ (*bb - 1);  // gets the actual values of the bit
    unsigned int fold = (unsigned)( (b & 0xffffffff) ^ (b >> 32)); // if the LSB is beyond the 32 bits, it moves it down othwerwise keeps it there
    *bb &= (*bb - 1);  // removes LSB from the actual 64 bit int 
    return BitTable[(fold * 0x783a9b23) >> 26];// uses perfect hashing to find the specific position of the LSB
}

//sets occupancies 
uint64_t index_to_uint64(int index, int bits, uint64_t m){
    int i, j;
    //start of the occupancy map
    uint64_t result = 0;
    
    //for all the relevant bits, we pop the LSB and if the index & (1 << i) != 0, we set the current bit in the occupancy map to 1
    for(i = 0; i < bits; i++){
        j = pop_1st_bit(&m);
        if( index & (1 << i) ){ result |= (uint64_t)1 << j; }
    }

    return result;
}

//mask functions create the mask to get the potential blockers for both the rook or the bishops
//the edge ranks and files do not get checked as it doesnt matter if there is a blocker or not
uint64_t rmask(int square){
    uint64_t mask = 0;
    int tfile = square % 8;
    int trank = square / 8;

    for(int r = trank + 1; r <= 6; r++){ mask |= ((uint64_t)1 << (r * 8 + tfile)); }
    for(int r = trank - 1; r >= 1; r--){ mask |= ((uint64_t)1 << (r * 8 + tfile)); }
    for(int f = tfile + 1; f <= 6; f++){ mask |= ((uint64_t)1 << (f + trank * 8)); }
    for(int f = tfile - 1; f >= 1; f--){ mask |= ((uint64_t)1 << (f + trank * 8)); }

    return mask;
}

uint64_t bmask(int square){
    uint64_t mask = 0;
    int tfile = square % 8;
    int trank = square / 8;

    for(int r = trank + 1, f = tfile + 1; r <= 6 && f <= 6; r++, f++){ mask |= ((uint64_t)1 << (r * 8 + f)); }
    for(int r = trank + 1, f = tfile - 1; r <= 6 && f >= 1; r++, f--){ mask |= ((uint64_t)1 << (r * 8 + f)); }
    for(int r = trank - 1, f = tfile + 1; r >= 1 && f <= 6; r--, f++){ mask |= ((uint64_t)1 << (r * 8 + f)); }
    for(int r = trank - 1, f = tfile - 1; r >= 1 && f >= 1; r--, f--){ mask |= ((uint64_t)1 << (r * 8 + f)); }

    return mask;
}

//using the blocker mask, we make an unsigned 64 bit int which shows us where the rook or bishop can attack up to
uint64_t rAtk(int square, uint64_t blocks){
    uint64_t atk = 0;
    int tfile = square % 8;
    int trank = square / 8;

    for(int r = trank + 1; r <= 7; r++){
        atk |= ((uint64_t)1 << (r * 8 + tfile));
        if( blocks & ((uint64_t)1 << (r * 8 + tfile)) ){ break; }
    }

    for(int r = trank - 1; r >= 0; r--){
        atk |= ((uint64_t)1 << (r * 8 + tfile));
        if( blocks & ((uint64_t)1 << (r * 8 + tfile)) ){ break; }
    }

    for(int f = tfile + 1; f <= 7; f++){
        atk |= ((uint64_t)1 << (f + trank * 8));
        if( blocks & ((uint64_t)1 << (trank* 8 + f)) ){ break; }
    }

    for(int f = tfile - 1; f >= 0; f--){
        atk |= ((uint64_t)1 << (f + trank * 8));
        if( blocks & ((uint64_t)1 << (trank * 8 + f)) ){ break; }
    }

    return atk;
}

uint64_t bAtk(int square, uint64_t blocks){
    uint64_t atk = 0;
    int tfile = square % 8;
    int trank = square / 8;

    for(int r = trank + 1, f = tfile + 1; r <= 7 && f <= 7; r++, f++){
        atk |= ((uint64_t)1 << (r * 8 + f));
        if(blocks & ((uint64_t)1 << (r * 8 + f))){ break; }
    }

    for(int r = trank + 1, f = tfile - 1; r <= 7 && f >= 0; r++, f--){
        atk |= ((uint64_t)1 << (r * 8 + f));
        if(blocks & ((uint64_t)1 << (r * 8 + f))){ break; }
    }
    for(int r = trank - 1, f = tfile + 1; r >= 0 && f <= 7; r--, f++){
        atk |= ((uint64_t)1 << (r * 8 + f));
        if(blocks & ((uint64_t)1 << (r * 8 + f))){ break; }
    }
    for(int r = trank - 1, f = tfile - 1; r >= 0 && f >= 0; r--, f--){
        atk |= ((uint64_t)1 << (r * 8 + f));
        if(blocks & ((uint64_t)1 << (r * 8 + f))){ break; }
    }

    return atk;
}

//changes the board into the specific index using the magic
int transform(uint64_t b, uint64_t magic, int bits){
    #if defined(USE_32_BIT_MANIPULATIONS)
        return (unsigned)( (int)b * (int)magic ^ (int)(b >> 32) * (int)(magic >> 32)) >> (32 - bits);
    #else
        return (int)((b * magic) >> (64 - bits));
    #endif
}

//looks for magics by making all possible blocker baord combos and finding the best suited magic number

uint64_t find_magic(int sq, int m, int bishop){
    // a is the attacks, used is the usd attacks and b is the occupancies
    uint64_t mask, used[4096], magic;
    uint64_t* a = bishop ? b_atk_tables[sq] : r_atk_tables[sq];
    uint64_t* o = bishop ? b_occ_tables[sq] : r_occ_tables[sq];
    int i, j, k, n, fail;
    //figures out which mask to use based on if we are using the bihop board or not and counts how many
    //1s are present in the mask for the sq
    mask = bishop ? bmask(sq) : rmask(sq);
    n = count_1s(mask); //counts how many relevant bits are present
    //generates every possible occupancy board for the given square and creates an attack board for it
    for(i = 0; i < (1 << n); i++){ // 1 << n is the occupancy indices
        o[i] = index_to_uint64(i, n, mask); //crates an occupancy map for the current index of the occupancies array
        a[i] = bishop ? bAtk(sq, o[i]) : rAtk(sq, o[i]);
    }
    //this process here then creates a low pseudo random number for the array of magic bit boards and attacks so that we create a perfect hash
    //the right magic number to always lead to the correct attack board
    for(k = 0; k < 100000000; k++){
        magic = random_uint64_fewbits();
        if(count_1s((mask * magic) & 0xFF00000000000000ULL) < 6){ continue; }
        for(i = 0; i < 4096; i++){ used[i] = 0ULL; }
        for(i = 0, fail = 0; !fail && i < (1 << n); i++){
            j = transform(o[i], magic, m);
            if(used[j] == 0ULL){ used[j] = a[i]; }
            else if(used[j] != a[i]){ fail = 1; }
        }
        if(!fail){ return magic; }
    }
    printf("***Failed***\n");
    return 0ULL;
}

void find_all_sq_magics(int bishop){
    int square;
    uint64_t* magics = bishop ? b_magics : r_magics;
    for(square = 0; square < 2; square++){
        magics[square] = find_magic(square, RBits[square], 0);
    }
}