#include <iostream>
#include <string>
#include <ostream>
#include <sstream>
#include "conv_needed.hpp"
#include "magicbitb.cpp"
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
        //not rank, the first bit next to the semi colon is the LSB
        uint64_t not_A_file = 0b1111111011111110111111101111111011111110111111101111111011111110;
        uint64_t not_B_file = 0b1111110111111101111111011111110111111101111111011111110111111101;
        uint64_t not_G_file = 0b1011111110111111101111111011111110111111101111111011111110111111;
        uint64_t not_H_file = 0b0111111101111111011111110111111101111111011111110111111101111111;


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

        void set_boards(uint64_t BK, uint64_t BQ, uint64_t BR, uint64_t BB, uint64_t BN, uint64_t BP, uint64_t WK, uint64_t WQ, uint64_t WR, uint64_t WB, uint64_t WN, uint64_t WP){
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

        //piece attacks
        uint64_t pawn_attacks(int play){
            uint64_t attacks = 0;
            uint64_t pawns = play ? black_pawn : white_pawn;

            if(play){
                attacks |= ((not_A_file & pawns) << 7);
                attacks |= ((not_H_file & pawns) << 9);
            }
            else{
                attacks |= ((not_A_file & pawns) >> 9);
                attacks |= ((not_H_file & pawns) >> 7);
            }

            return attacks;
        }

        uint64_t knight_attacks(int play){
            uint64_t attacks = 0;
            uint64_t knights = play ? black_knight : white_knight;

            attacks |= (not_H_file & not_G_file & knights) << 10;
            attacks |= (not_H_file & knights) << 17;
            attacks |= (not_H_file & not_G_file & knights) >> 6;
            attacks |= (not_H_file & knights) >> 15;

            attacks |= (not_A_file & not_B_file & knights) >> 10;
            attacks |= (not_A_file & knights) >> 17;
            attacks |= (not_A_file & not_B_file & knights) << 6;
            attacks |= (not_A_file & knights) << 15;

            return attacks;
        }

        uint64_t king_attacks(int play){
            uint64_t attacks = 0;
            uint64_t king = play ? black_king : white_king;

            attacks |= ((not_A_file & king) >> 9);
            attacks |= ((not_A_file & king) >> 1);
            attacks |= ((not_A_file & king) << 7);

            attacks |= ((not_H_file & king) << 9);
            attacks |= ((not_H_file & king) << 1);
            attacks |= ((not_H_file & king) >> 7);

            attacks |= (king << 8);
            attacks |= (king >> 8);

            return attacks;
        }

        void print_mask_funcR(int square){ print_board(rmask(square)); }

        void print_mask_funcB(int square){ print_board(bmask(square)); }

        //need to be able to evaluate how favourable a current position is
        int eval_pos(){}

        //a move can be encoded in binary we just need to figure out the amount of bits
        int calc_move(){}

        int generate_moves(){}

        //need functions to make and undo moves, paramaeters subject to change
        void make_move(int pos, int target){}

        void undo_move(int pos, int target){}
};