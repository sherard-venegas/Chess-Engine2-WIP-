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
	{K, "WKi"}, {Q, "BQu"}, {R, "WRo"}, {N, "WBi"}, {N, "WKn"}, {P, "WPw"}
};

map<string, int> side = { {"w", 0}, {"b", 1} };
