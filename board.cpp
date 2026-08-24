#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include <sstream>
#include "conv_needed.hpp"
#include "engine.cpp"
using namespace std;

class Board
{
    private:
        int king_squares[2];
        int curr_side;
        int board[64]; //may change this to a bit board representation
        int castle_rights[4];
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
                char cc = board_portion[j];
                if( cc == '/' ){ continue; }

                else if( cc >= '0' && cc <= '9'){ ind += (int)(cc - '0'); }

                else{ board[ind] = char_To_Piece[cc]; }
                
            }
            
            //initialise the chess engine if the amount of players is 1
            if( players == 1 ){ bob.set_piles(6); }
            
        }

        //the ability to see the board will be important
        void print_board(){}

        //move making, undoing and other stuff to check if move is legal
        void make_move(int from, int target){
            /*
            posibly dont need an undo move function if we are choosing to use a bit board
            we can just copy the state of the bitboard being used andd restore it
            */
        }

        bool check_king(){}

        bool is_atkd(int square){}

        bool end_game(){}
};
