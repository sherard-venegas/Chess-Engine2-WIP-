#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cctype>
#include "conv_needed.hpp"
#include "engine.cpp"
using namespace std;

class Board
{
    private:
        int king_squares[2];
        int curr_side;
        int board[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
        }; //may change this to a bit board representation
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
                    board[ind] = char_To_Piece[cc];
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
                int cur_p = board[i];
                if(i % 8 == 0){
                    board_str += (char)( ( (64 - i) / 8) + '0');
                    board_str += "\t";
                    if(cur_p != 0){ board_str += "| " + piece_To_Char[cur_p] + " | "; }

                    else{ board_str += "|     | "; }
                }

                else if(i % 8 == 7){
                    if(cur_p != 0){ board_str += piece_To_Char[cur_p] + " |\n"; }

                    else{ board_str += "    |\n"; }
                }

                else{
                    if(cur_p != 0){ board_str += piece_To_Char[cur_p] + " | "; }

                    else{ board_str += "    | "; }
                }
            }

            cout << board_str;
        }

        void print_bitb(){}

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

            int piece = board[t];
            board[t] = board[f];
            board[f] = 0; 

            while( !is_legal(f, t, piece) ){
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

        void undo_move(int from, int target, int piece){
            board[from] = board[target];
            board[target] = piece;
        }

        bool is_legal(int from, int target, int piece){
            if( (piece > 0 && curr_player == white) ||
            (piece < 0 && curr_player == black) || board[from] == 0){
                undo_move(from, target, piece);
                return false;
            }
        }

        bool check_king(){ return is_atkd(king_squares[curr_player]); }

        bool is_atkd(int square){}

        bool end_game(){}

        int *get_board(){
            return board;
        }
};
