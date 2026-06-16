#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include <sstream>
#include "conv_needed.hpp"
using namespace std;

class Board
{
    public:
        int board[64];
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


        }
};
