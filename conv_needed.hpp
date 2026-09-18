#include <map>
#include <string>
using namespace std;

//Making an ennumerated class for the pieces so that it can easily be converted into an integer and also be easily compared for the later use of FEN, captial = white, lower = black
enum pieces { k = -20, q = -9, r = -5, b = -4, n = -3, p = -1, X = 2, P = 1, N = 3, B = 4, R = 5, Q = 9, K = 20, e = 0 };

//which side is currently having a turn
enum side {white, black};

//map for converting from strings for easy fen translation
map<char, int> char_To_Piece = {
		{'k', k},{'q', q},{'r', r},{'b', b},{'n', n},{'p', p},{'K', K},{'Q', Q},{'R', R},{'B', B},{'N', N},{'P', P}
};

//map for easy numeric translation for easy board prints
map<int, string> piece_To_Char = {
	{k, "BKi"}, {q, "BQu"}, {r, "BRo"}, {b, "BBi"}, {n, "BKn"}, {p, "BPw"}, 
	{K, "WKi"}, {Q, "WQu"}, {R, "WRo"}, {B, "WBi"}, {N, "WKn"}, {P, "WPw"}
};

//map for coordinate conversions
map<string, int> coord = {
	{"a1", 0}, {"a2", 1}, {"a3", 2}, {"a4", 3}, {"a5", 4}, {"a6", 5}, {"a7", 6}, {"a8", 7},
	{"b1", 8}, {"b2", 9}, {"b3", 10}, {"b4", 11}, {"b5", 12}, {"b6", 13}, {"b7", 14}, {"b8", 15},
	{"c1", 16}, {"c2", 17}, {"c3", 18}, {"c4", 19}, {"c5", 20}, {"c6", 21}, {"c7", 22}, {"c8", 23},
	{"d1", 24}, {"d2", 25}, {"d3", 26}, {"d4", 27}, {"d5", 28}, {"d6", 29}, {"d7", 30}, {"d8", 31},
	{"e1", 32}, {"e2", 33}, {"e3", 34}, {"e4", 35}, {"e5", 36}, {"e6", 37}, {"e7", 38}, {"e8", 39},
	{"f1", 40}, {"f2", 41}, {"f3", 42}, {"f4", 43}, {"f5", 44}, {"f6", 45}, {"f7", 46}, {"f8", 47},
	{"g1", 48}, {"g2", 49}, {"g3", 50}, {"g4", 51}, {"g5", 52}, {"g6", 53}, {"g7", 54}, {"g8", 55},
	{"h1", 56}, {"h2", 57}, {"h3", 58}, {"h4", 59}, {"h5", 60}, {"h6", 61}, {"h7", 62}, {"h8", 63}
};

map<string, int> s_to_player = { {"w", white}, {"b", black} };


const int BitTable[64] = {
    64, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
    51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
    26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
    58, 20, 37, 17, 36, 8
};

//relevant occupancy bit count for each position a rook or bishop is in for each square
int RBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

int BBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

//magic bit boards and magic numbers
uint64_t r_magics[64];
uint64_t b_magics[64];
uint64_t r_atk_tables[64][4096];
uint64_t r_occ_tables[64][4096];
uint64_t b_atk_tables[64][4096];
uint64_t b_occ_tables[64][4096];