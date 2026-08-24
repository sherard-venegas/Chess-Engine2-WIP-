#include <iostream>
using namespace std;

class Engine
{
    private:
        int ahead;
        //black bitboards
        uint64_t black_pawn;
        uint64_t black_knight;
        uint64_t black_bishop;
        uint64_t black_rook;
        uint64_t black_queen;
        uint64_t black_king;
        //white bitboards
        uint64_t white_pawn;
        uint64_t white_knight;
        uint64_t white_bishop;
        uint64_t white_rook;
        uint64_t white_queen;
        uint64_t white_king;

    public:
        Engine ()
        {
            ahead = 10;
        }

        void set_piles(int piles){
            ahead = piles;
        }

        //sets the boards for the begining
        void set_boards(int board[64]){}

        //need to be able to evaluate how favourable a current position is
        int eval_pos(){}

        //a move can be encoded in binary we just need to figure out the amount of bits
        int calc_move(){}

        int generate_moves(){}

        //need functions to make and undo moves, paramaeters subject to change
        void make_move(int pos, int target){}

        void undo_move(int pos, int target){}
};