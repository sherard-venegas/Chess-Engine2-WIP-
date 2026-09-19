#include <iostream>
#include <string>
#include <ostream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include "magicbitb.cpp"
using namespace std;

class Engine
{
    private:
        int ahead;
        int curr_player = white;
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

        Engine (uint64_t BK, uint64_t BQ, uint64_t BR, uint64_t BB, uint64_t BN, uint64_t BP, uint64_t WK, uint64_t WQ, uint64_t WR, uint64_t WB, uint64_t WN, uint64_t WP){
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

        uint64_t slider_atk(int sq, int bishop){
            uint64_t all_b = black_king + black_queen + black_rook + black_bishop + black_knight + black_pawn;
            all_b += white_king + white_queen + white_rook + white_bishop + white_knight + white_pawn;

            uint64_t blockers = all_b & (bishop ? bishop_blk_m[sq] : rook_blk_m[sq]);
            print_board(blockers);
            cout << "\n";

            int index = bishop ? transform(blockers, b_magics[sq], BBits[sq]) : transform(blockers, r_magics[sq], RBits[sq]);
            
            return (bishop ? b_atk_tables[sq][index] : r_atk_tables[sq][index]);
        }

        uint64_t get_all_atks(uint64_t board, int bishop){
            uint64_t atks = 0ULL;

            while(board > 0){
                int ind = get_lsb_ind(board);
                board &= (board - 1);
                atks |= slider_atk(ind, bishop);
            }

            return atks;
        }

        uint64_t rook_attacks(int play){
            uint64_t curr_board = play ? black_rook : white_rook;
            return get_all_atks(curr_board, 0);
        }

        uint64_t bishop_attacks(int play){
            uint64_t curr_board = play ? black_bishop : white_bishop;
            return get_all_atks(curr_board, 1);
        }

        uint64_t queen_attacks(int play){
            uint64_t curr_board = play ? black_queen : white_queen;
            uint64_t atks = get_all_atks(curr_board, 0);
            atks |= get_all_atks(curr_board, 1);
            return atks;
        }

        void print_mask_funcR(int square){ print_board(rmask(square)); }

        void print_mask_funcB(int square){ print_board(bmask(square)); }
        
        //need to be able to evaluate how favourable a current position is
        int eval_pos(){}

        //a move can be encoded in binary we just need to figure out the amount of bits
        int calc_move(){}

        int generate_moves(){}

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

        bool is_legal(int from, int target, uint64_t* piece_board, uint64_t* taken_board){
            if(piece_board == nullptr){ return false; }

            else if(taken_board != nullptr){
                if(piece_board == taken_board){ return false; }

                else if( same_team(taken_board) ){ return false; }
            }

            else if( king_check(curr_player) ){ return false; }

            else{ return true; }
        }

        //need functions to make and undo moves, paramaeters subject to change
        void make_move(int pos, int target){
            uint64_t* from = figure_board(pos);
            uint64_t* to = figure_board(pos);

            if(to != nullptr){ *to -= ( (uint64_t) 1 << target ); }
        }

        void undo_move(int pos, int target, uint64_t* piece_b, uint64_t* taken_p){
            *piece_b -= ((uint64_t) 1 << target);
            *piece_b += ((uint64_t) 1 << pos);

            if(taken_p != nullptr){ *taken_p += ( (uint64_t) 1 << target ); }
        }

        bool king_check(int play){ return is_atkd( get_lsb_ind(play ? black_king : white_king), play); }

        bool is_atkd(int square, int play){
            uint64_t enemy_atks = king_attacks(play) | queen_attacks(play) | rook_attacks(play)
                                  | bishop_attacks(play) | knight_attacks(play) | pawn_attacks(play);
            return (1ULL << square) & enemy_atks;
        }

        bool is_stale(){
            bool check = king_check(curr_player);
            uint64_t attacks = king_attacks(curr_player) | queen_attacks(curr_player) | rook_attacks(curr_player)
                                  | bishop_attacks(curr_player) | knight_attacks(curr_player) | pawn_attacks(curr_player);
            
            return (attacks == 0) && !check;
        }

        bool end_game(){}
};