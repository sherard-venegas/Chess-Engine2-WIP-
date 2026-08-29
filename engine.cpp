#include <iostream>
using namespace std;

class Engine
{
    private:
        int ahead;
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

    public:
        Engine ()
        {
            ahead = 10;
        }

        void set_piles(int piles){
            ahead = piles;
        }

        //sets the boards for the begining
        void set_boards(int board[64]){
            for(int i = 0; i < 64; i++){
                int curr_piece = board[i];
                switch(curr_piece){
                    case k:
                        black_king += (( (uint64_t) 1 ) << i);
                        break;
                    case q:
                        black_queen += (( (uint64_t) 1 ) << i);
                        break;
                    case b:
                        black_bishop += (( (uint64_t) 1 ) << i);
                        break;
                    case n:
                        black_knight += (( (uint64_t) 1 ) << i);
                        break;
                    case r:
                        black_rook += (( (uint64_t) 1 ) << i);
                        break;
                    case p:
                        black_pawn += (( (uint64_t) 1 ) << i);
                        break;
                    case K:
                        white_king += (( (uint64_t) 1 ) << i);
                        break;
                    case Q:
                        white_queen += (( (uint64_t) 1 ) << i);
                        break;
                    case B:
                        white_bishop += (( (uint64_t) 1 ) << i);
                        break;
                    case N:
                        white_knight += (( (uint64_t) 1 ) << i);
                        break;
                    case R:
                        white_rook += (( (uint64_t) 1 ) << i);
                        break;
                    case P:
                        white_pawn += (( (uint64_t) 1 ) << i);
                        break;
                    default:
                        break;
                }
            }
        }

        void set_boards(int BK, int BQ, int BR, int BB, int BN, int BP, int WK, int WQ, int WR, int WB, int WN, int WP){
            black_king = BK;
            black_queen = BQ;
            black_rook = BR;
            black_bishop = BB;
            black_knight = BN;
            black_pawn = BP;
            white_king = WK;
            white_queen = WQ;
            white_rook = WR;
            white_bishop = WB;
            white_knight = WN;
            white_pawn = WP;
        }

        void print_bitb(int num){
            switch(num){
                case 0:
                    print_board(black_king);
                    break;
                case 1:
                    print_board(black_queen);
                    break;
                case 2:
                    print_board(black_rook);
                    break;
                case 3:
                    print_board(black_bishop);
                    break;
                case 4:
                    print_board(black_knight);
                    break;
                case 5:
                    print_board(black_pawn);
                    break;
                case 6:
                    print_board(white_king);
                    break;
                case 7:
                    print_board(white_queen);
                    break;
                case 8:
                    print_board(white_rook);
                    break;
                case 9:
                    print_board(white_bishop);
                    break;
                case 10:
                    print_board(white_knight);
                    break;
                case 11:
                    print_board(white_pawn);
                    break;
                default:
                    break;
            }
        }

        void print_board(uint64_t num){
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

        //need to be able to evaluate how favourable a current position is
        int eval_pos(){}

        //a move can be encoded in binary we just need to figure out the amount of bits
        int calc_move(){}

        int generate_moves(){}

        //need functions to make and undo moves, paramaeters subject to change
        void make_move(int pos, int target){}

        void undo_move(int pos, int target){}
};