#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include "board.cpp"
using namespace std;

int main(){
    //FEN for some tests "r1bqk2r/pppp1ppp/2n5/4p3/1bB1n3/5N2/PPPP1PPP/RNBQ1RK1 w kq - 2 6"
    Board jim("8/K7/8/8/4K3/8/8/8 w kq - 2 6", 2);
    Engine dave;
    jim.print_board();
    //cout << jim.choose_sq();
    cout << "\n\n";
    //dave.set_boards( jim.get_board() );
    //dave.print_bitb(6);
    cout << "\n";
    //dave.print_board( dave.king_attacks(0) );
    return 0;
}
