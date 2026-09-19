#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include "board.cpp"
using namespace std;

int main(){
    cout << "calculating magics";
    find_all_sq_magics();
    //FEN for some tests "r1bqk2r/pppp1ppp/2n5/4p3/1bB1n3/5N2/PPPP1PPP/RNBQ1RK1 w kq - 2 6"
    Board jim("K1K4Q/8/R3K3/4K3/4K2R/K7/8/8 w kq - 2 6", 2);
    Engine dave( jim.get_BK(), jim.get_BQ(), jim.get_BR(), jim.get_BB(), jim.get_BN(), jim.get_BP(), jim.get_WK(), jim.get_WQ(), jim.get_WR(), jim.get_WB(), jim.get_WN(), jim.get_WP() );
    jim.print_board();
    //cout << jim.choose_sq();
    cout << "\n\n";

    dave.print_bitb(9);
    cout << "\n";
    print_board(dave.queen_attacks(0));
    cout << "\n";
    cout << get_lsb_ind(0x0001000000000000);
    cout << "\n";
    
    //print_the(dave.slider_atk(7, 0));
    return 0;
}