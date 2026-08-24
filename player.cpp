#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include <sstream>
#include "conv_needed.hpp"

class Player
{
    public:
        int col;

        Player(string colour){
            col = side[colour];
        }
}