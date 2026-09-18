#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include "board.cpp"
using namespace std;

uint64_t bob[64][4096];

void print_the(uint64_t num){
    uint64_t com = 1;
    int i = 0;
    uint64_t bor = num;
    while( i < 64){
        uint64_t res = (com & bor);
        if(res){ cout << " 1 "; }
        else{ cout << " 0 "; }
        if(i % 8 == 7){ cout << "\n"; }
        i++;
        com = (com << 1);
    }
}

int get_lsb_ind(uint64_t board){
	uint64_t lsb_val = board ^ (board - 1);
	unsigned int fold = (unsigned)( (lsb_val & 0xffffffff) ^ (lsb_val >> 32));
	return BitTable[(fold * 0x783a9b23) >> 26]; // uses perfect hashing to find the specific position of the LSB
}

int main(){
    //FEN for some tests "r1bqk2r/pppp1ppp/2n5/4p3/1bB1n3/5N2/PPPP1PPP/RNBQ1RK1 w kq - 2 6"
    Board jim("K7/8/8/8/4K3/8/8/8 w kq - 2 6", 2);
    Engine dave( jim.get_BK(), jim.get_BQ(), jim.get_BR(), jim.get_BB(), jim.get_BN(), jim.get_BP(), jim.get_WK(), jim.get_WQ(), jim.get_WR(), jim.get_WB(), jim.get_WN(), jim.get_WP() );
    //jim.print_board();
    //cout << jim.choose_sq();
    cout << "\n\n";

    dave.print_bitb(6);
    cout << "\n";
    dave.print_board( dave.king_attacks(0) );
    cout << "\n";
    dave.print_board(~jim.get_WK());
    cout << "\n";


    cout << get_lsb_ind(0x0001000000000000);
    return 0;
}