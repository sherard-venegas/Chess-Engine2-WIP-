#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cctype>
#include "engine.cpp"
using namespace std;

class Board
{
    private:
        int king_squares[2];
        int curr_side;
        //black bitboards
        uint64_t black_pawn = 0x00000000;
        uint64_t black_knight = 0x00000000;
        uint64_t black_bishop = 0x00000000;
        uint64_t black_rook = 0x00000000;
        uint64_t black_queen = 0x00000000;
        uint64_t black_king = 0x00000000;
        //white bitboards
        uint64_t white_pawn = 0x00000000;
        uint64_t white_knight = 0x00000000;
        uint64_t white_bishop = 0x00000000;
        uint64_t white_rook = 0x00000000;
        uint64_t white_queen = 0x00000000;
        uint64_t white_king = 0x00000000;
        //not rank, the first bit next to the semi colon is the LSB
        uint64_t not_A_file = 0b1111111011111110111111101111111011111110111111101111111011111110;
        uint64_t not_B_file = 0b1111110111111101111111011111110111111101111111011111110111111101;
        uint64_t not_G_file = 0b1011111110111111101111111011111110111111101111111011111110111111;
        uint64_t not_H_file = 0b0111111101111111011111110111111101111111011111110111111101111111;

        int castle_rights[4];
        int curr_player = white;
        Engine bob;

    public:
        /*
        example FEN: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"
        starts from the 8th rank and then goes down to the 1st
        we are not using the half move clock porition 
        */

        Board(string fen, int players)
        {
            vector <string> FENsplit; // to contain the split up fen instead of having multiple different variables
            // containing each part, can be accessed by index, vector so that it is dynamic

            string part; //used to get the specific part and store it so it can be added to the vector

            istringstream iss(fen);

            while (getline(iss, part, ' '))
            {
                FENsplit.push_back(part);
            }

            //build the board from fen
           
            string board_portion = FENsplit.at(0);
            int ind = 0;
            
            for(int j = 0; j < board_portion.length(); j++){
                //use a switch statement if its a bitboard, ind will still have a role
                char cc = board_portion[j];
                if( cc == '/' ){ continue; }

                else if( cc >= '0' && cc <= '9'){ ind += (int)(cc - '0'); }

                else{
                    switch(cc){
                        case 'K':
                            king_squares[0] = ind;
                            white_king += (uint64_t)1 << ind;
                            break;
                        case 'Q':
                            white_queen += (uint64_t)1 << ind;
                            break;
                        case 'R':
                            white_rook += (uint64_t)1 << ind;
                            break;
                        case 'B':
                            white_bishop += (uint64_t)1 << ind;
                            break;
                        case 'N':
                            white_knight += (uint64_t)1 << ind;
                            break;
                        case 'P':
                            white_pawn += (uint64_t)1 << ind;
                            break;
                        case 'k':
                            king_squares[1] = ind;
                            black_king += (uint64_t)1 << ind;
                            break;
                        case 'q':
                            black_queen += (uint64_t)1 << ind;
                            break;
                        case 'r':
                            black_rook += (uint64_t)1 << ind;
                            break;
                        case 'b':
                            black_bishop += (uint64_t)1 << ind;
                            break;
                        case 'n':
                            black_knight += (uint64_t)1 << ind;
                            break;
                        case 'p':
                            black_pawn += (uint64_t)1 << ind;
                    }

                    ind++;
                }
            }
            
            //initialise the chess engine if the amount of players is 1
            if( players == 1 ){ bob.set_piles(6); }
        }

        //the ability to see the board will be important
        void print_board(){
            string board_str = "";

            for(int i = 0; i < 8; i++){
                char curr_char = (char)(i + 'A');
                if(i == 0){ board_str += " \t|  A  | "; }
                else{
                    board_str += " ";
                    board_str += curr_char;
                    board_str += "  | ";
                }
            }

            board_str += "\n\n";

            for(int i = 0; i < 64; i++){
                int cur_p = id_piece(i);
                
                if(i % 8 == 0){
                    board_str += (char)( ( (64 - i) / 8) + '0');
                    board_str += "\t";
                    if(cur_p != 0){ board_str += "| " + piece_To_Char[cur_p] + " | "; }

                    else{ board_str += "|     | "; }
                }

                else if(i % 8 == 7){
                    if(cur_p != 0){ board_str += piece_To_Char[cur_p] + " |\n";}

                    else{ board_str += "    |\n"; }
                }

                else{
                    if(cur_p != 0){ board_str += piece_To_Char[cur_p] + " | ";}

                    else{ board_str += "    | "; }
                }
            }

            cout << board_str;
        }

        int id_piece(int square){
            uint64_t pos = (uint64_t)1 << square;

            if( (black_king & pos) ){ return k; }
            else if( (black_queen & pos) ){ return q; }
            else if( (black_rook & pos) ){ return r; }
            else if( (black_bishop & pos) ){ return b; }
            else if( (black_knight & pos) ){ return n; }
            else if( (black_pawn & pos) ){ return p; }
            else if( (white_king & pos) ){ return K; }
            else if( (white_queen & pos) ){ return Q; }
            else if( (white_rook & pos) ){ return R; }
            else if( (white_bishop & pos) ){ return B; }
            else if( (white_knight & pos) ){ return N; }
            else if( (white_pawn & pos) ){ return P; }
            
            else{
                return 0;
            }
        }

        //move making, undoing and other stuff to check if move is legal
        void make_move(){

            /*
            posibly dont need an undo move function if we are choosing to use a bit board
            we can just copy the state of the bitboard being used andd restore it
            */
            cout << "enter the square of the piece you would like to move: ";
            int f = choose_sq();
            
            cout << "enter the square you like to move it to: ";
            int t = choose_sq();

            uint64_t* piece_board = figure_board(f);
            uint64_t* taken_board = figure_board(f);

            while( !is_legal(f, t, piece_board, taken_board) ){
                cout << "enter the source square: ";
                f = choose_sq();

                cout << "enter the target square, enter same square as source to reselect";
                t = choose_sq();
            }
        }

        int choose_sq(){
            string sq;
            getline(cin, sq);

            if(sq.length() > 0){
                //base() apparently changes reverse indicators back to forward indicators
                sq.erase(sq.begin(), std::find_if(sq.begin(), sq.end(), [](int c) {return !std::isspace(c);}));
                sq.erase(std::find_if(sq.rbegin(), sq.rend(), [](int c) {return !std::isspace(c);}).base(), sq.end());
                sq[0] = tolower(sq[0]);
            }

            while( !valid_sq(sq) ){
                sq = "";
                cout << "re-enter the coordinate of the piece to move using chess notation: ";
                cin.clear();
                cin.sync();
                getline(cin, sq);

                if(sq.length() != 0){
                    sq.erase(sq.begin(), std::find_if(sq.begin(), sq.end(), [](int c) {return !std::isspace(c);}));
                    sq.erase(std::find_if(sq.rbegin(), sq.rend(), [](int c) {return !std::isspace(c);}).base(), sq.end());
                    sq[0] = tolower(sq[0]);
                }
            }

            return coord.at(sq);
        }

        //this is to figure out the address of thetarget board and the piece to be moved board
        uint64_t* figure_board(uint64_t from){
            uint64_t target_sq = 1 << from;

            if(target_sq & white_pawn > 0){ return &white_pawn; }

            else if(target_sq & white_pawn > 0){ return &white_pawn; }

            else if(target_sq & white_knight > 0){ return &white_knight; }

            else if(target_sq & white_bishop > 0){ return &white_bishop; }

            else if(target_sq & white_rook > 0){ return &white_rook; }

            else if(target_sq & white_queen > 0){ return &white_queen; }

            else if(target_sq & white_king > 0){ return &white_king; }

            else if(target_sq & black_pawn > 0){ return &black_pawn; }

            else if(target_sq & black_knight > 0){ return &black_knight; }

            else if(target_sq & black_bishop > 0){ return &black_bishop; }

            else if(target_sq & black_rook > 0){ return &black_rook; }

            else if(target_sq & black_queen > 0){ return &black_queen; }

            else if(target_sq & black_king > 0){ return &black_king; }

            else{
                return nullptr;
            }
        }

        bool valid_sq(string sq){
            try{
                coord.at(sq);
                return true;
            }
            catch(exception& e){
                cout << "not a valid square buddy \n";
                return false;
            }
        }

        void undo_move(int from, int target, uint64_t* piece_board, uint64_t* taken_board){
            *piece_board -= (1 << target);
            *piece_board += (1 << from);

            if(taken_board != nullptr){ *taken_board += (1 << target); }
        }

        bool is_legal(int from, int target, uint64_t* piece_board, uint64_t* taken_board){
            if(piece_board == nullptr){ return false; }

            else if(taken_board != nullptr){
                if(piece_board == taken_board){ return false; }

                else{ return same_team(taken_board); }
            }
        }

        bool same_team(uint64_t* taken_board){
            uint64_t* pawn_board = curr_player ? &white_pawn : &black_pawn;
            uint64_t* knight_board = curr_player ? &white_knight : &black_knight;
            uint64_t* bishop_board = curr_player ? &white_bishop : &black_bishop;
            uint64_t* rook_board = curr_player ? &white_rook : &black_rook;
            uint64_t* queen_board = curr_player ? &white_queen : &black_queen;
            uint64_t* king_board = curr_player ? &white_king : &black_king;

            if(taken_board == pawn_board || taken_board == knight_board || taken_board == bishop_board ||
            taken_board == rook_board || taken_board == queen_board || taken_board == king_board){
                return true;
            }
            else{ return false; }
        }

        bool check_king(){ return is_atkd(king_squares[curr_player]); }

        bool is_atkd(int square){}

        bool end_game(){}

        //get functions for the bitboards for the engine testing

        uint64_t get_BK(){ return black_king; }
        uint64_t get_BQ(){ return black_queen; }
        uint64_t get_BR(){ return black_rook; }
        uint64_t get_BB(){ return black_bishop; }
        uint64_t get_BN(){ return black_knight; }
        uint64_t get_BP(){ return black_pawn; }
        uint64_t get_WK(){ return white_king; }
        uint64_t get_WQ(){ return white_queen; }
        uint64_t get_WR(){ return white_rook; }
        uint64_t get_WB(){ return white_bishop; }
        uint64_t get_WN(){ return white_knight; }
        uint64_t get_WP(){ return white_pawn; }
};
