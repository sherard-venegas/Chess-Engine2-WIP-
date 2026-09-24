#include "board.cpp"
using namespace std;

int main(){
    cout << "calculating magics\n";
    //find_all_sq_magics();
    load_all_tables();
    //FEN for some tests "r1bqk2r/pppp1ppp/2n5/4p3/1bB1n3/5N2/PPPP1PPP/RNBQ1RK1 w kq - 2 6"
    Board b("K1K4Q/8/R3K3/4K3/P3K2R/P7/PP6/8 w kq - 2 6", 2);
    Engine eng( b.get_BK(), b.get_BQ(), b.get_BR(), b.get_BB(), b.get_BN(), b.get_BP(), b.get_WK(), b.get_WQ(), b.get_WR(), b.get_WB(), b.get_WN(), b.get_WP() );
    //b.print_board();
    //cout << b.choose_sq();
    //cout << "\n\n";

    //eng.print_bitb(11);
    //cout << "\n";
    //print_board(eng.pawn_moves(0));
    //cout << "\n";
    //cout << get_lsb_ind(0x0001000000000000);
    //cout << "\n";
    eng.rook_attacks(0);
    
    //print_the(eng.slider_atk(7, 0));
    return 0;
}