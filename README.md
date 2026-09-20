# Chess-Engine2
Making a second, more efficient, chess engine than the previous using a bitboard implementation 

The chess engine is still currently in progress, the remaining functionalities
to be finished building are the special move handling for the engine side of
the make_move, undo_move function and is_legal.

As well as all the functions to do with finding thes best move in a position,
these are:
    generate_moves - this function will generate a set amount of moves the
                     current player is able to do
    eval_pos - after the traversal of a set amount of moves being done, it
               will attempt to give the current position a value that states
               which side has the current advantage
    calc_move - this function will bring those two functions and the make_move
                , undo_move and is_legal functions in order to find the best
                move in the current situation, it will make use of recursions
                up until a certain depth which it will then evaluate the
                current board state and rurn back up to the base call undoing
                the moves made to get there.

In this project there are currently 2 classes known as Engine and Board, board
inherits functions from engine as they have quite a bit of overlap however
there are some changes in functions such as make move as they require a user to
input their desired move.

Furthermore there are 4 other files which we will go into depth on after
exlaining the functions of board and engine.

# Engine and Board
All the attributes with a colour of either black or white before the '_'
character are the bitboards  of the engine.
The attributes below those are values of specific ranks and files for helping
exculde certain pieces when calculating a set group of pieces moves / attacks.

The attributes above the bitboards are:
    ahead - how many piles / half moves the engine should go through
    curr_player - the colour of the current player
    en_pass_sq - the current square that can be used for an enpassent move
    c_rights - castleing rights of both sides of the board, stored as an int to
               easily remove castling rights using bit manipluation

Note that both engine and board class have the same attributes aside from ahead
which is unique to engine and the board class instantiates an engine if needed

There are two different constructor for the engine class, one allows you to
define the bitboards on construction whilst the other does not, this was done
for testing purposes.

For the board class there is only a single constructor which takes in two
inputs, the first input is a string which is of FEN standard chess notation
from which the board to be played will be built from and the second input is
for how many users are playing. Inside the constructor the FEN string is split
into its sepoerate components based on white spaces, using these the bitboards
and other variables are made / set with values.

# Engine functions
set_piles - allows you to set the value of ahead to a different value

set_boards - allows you to set the values of all bitboards to one of your choosing

print_bitb - a function that makes use of the print board function inside of
             conv-needed.hpp which prints out the bitboard of your choice as a
             set of 1s and 0s, the top left represents the a8 square whilst the
             bottom right represents the h1 square. This function was made for
             testing and debugging purposes for the next few functions.

pawn_atk_map - takes in which player is currently playing and a bitboard of
               pawns and uses bit maniplulation to output the expected attack
               bitboard of the bitboard put in.
               Note: it may be changed later to also only include attacks which
                     can take

pawn_move_map - takes in the same inputs as the function before but instead
                uses bit manipulations to creat the quite move map of the pawns
                Note: enpassent may need to be added to one of the two pawn map
                      functions

pawn_attacks - has 2 versions from function overloading, this makes use of the
               pawn_atk_map function, the first version does it for all pawns
               whilst the second does it for a single square, this was made for
               making the generate_moves function easier as the source square
               will be easilly identifiable.

kn_atk_map - does the same as pawn_atk_map but for the knight movements.

kn_attacks - is the same as the pawn_attacks function but it does it for the
             knight piece(s).

king_attacks - it does not have a corresponding king map function as there is
               usually only 1 king on the board when playing chess.

slider_atk - makes use of perfect hashing to get the movements of either the
             rook or bishop in O(1) time rather than using a for loop, the
             perfect hash will be further explained when we get on to the
             magicbitb.cpp file

get_all_atks - gets all the attack map for the all current sides rooks or
               bishops, makes use of slider_atk

rook_attacks - makes use of the get_all_atks function to get the atk maps for
               all the rooks of the current side playing

bishop_attacks - does the same as rook_attacks but for the bishops of the
                 current player

queen_attacks - calls get_all_atks twice, once as a bishop and the other as a
                rook as the way the queen moves is a merge of the rook and
                bishop pieces

figure_board - a function that takes in the target/source square and find the
               board the move is targeting and returns a pointer to it, this
               is made to work with the make_move function

same_team - a function that returns a boolean, it makes sure that the piece you
            are moving has not taken a piece from the same team by checking if
            the target bitboard pointer is a pointer to a bit board of the 
            current side, it is a function that is for the is_legal function

right_move - a function currently in progress but aims to check if the piece
             moved can actually move there by getting the pointer of the baord
             and derefferencing the point to make an attack map and seeing if
             the target square lines up with any of the set bits in the map

is_legal - makes use of the previous two functions described to check if the
           move being made is legal, it is currently a work in progress

promotion_board - is a function to return the address of the board the pawn
                  , that has reached the opposite side, wants to promote to.
                  This function is used by make_move and undo_move

make_move - gets the source and target square as well as if its a special move
            or not and the promotion board and changes all the boards according
            to that information and then checks if it is_legal, this function
            is currently in progress as all the special moves are not done.

undo_move - takes in the same info with the addition of the taken piece board
            and the piece moved board to reverse what has been done by the
            make_move function in the case that the move turns out to be
            illegal, it is also a work in progress for the same reasons as
            make_move

is_atkd - checks if a square is currently attacked by the opposing side

king_check - makes use of the is_atkd function to see if your king is currently
             in check

is_stale - a function that checks if the current position is a stalemate, makes
           use of the king_check function and all the attack functions

is_mate - is currently a skeleton function but will check if the current game
          position is a checkmate

is_end - a function that brings together the last two functions to see if the
         game is over(in an end state)

# Board functions
print_board - prints the board from the bitboards all coming together

id_pieces - a function used by print_board to figure out what piece to print
            next

make_move - different from the make_move function it inherits form the engine
            class, it gets two user inputs, validates them and makes the move
            , makaes use of the is_legal function it inherited from engine,
            gets the user to remake a move until it is legal.
            Note: this function is in progress as special moves are not done
                  yet

choose_sq - it is how the user input is taken and sanitised to a standard
            format so that the user input is not too picky with how things are
            entered in

valid_sq - user try catch method to handle the error of the user input not
           being a valid chess coordinate

unless stated here the remaining function that board has inherited from the
engine class are the same

functions with get - gets the corresponding private attribute and returns it

# Conv_needed.hpp
conv_needed.hpp is a header file that contains all the enums, maps and
helper functions that are needed but do not really belong in any other file or
class.

the functions are:

get_lsb_ind - gets the position of the lsb and returns it through bit
              manipulation and a Debruijn squence

print_board - a function that ouputs a series of 1s and 0s to show a bitboard
              it is a function made for testing purposes

north_fill    A function that pads 1s behind a bit that is already set
and         - until reaches the front(south fill) or back (a north fill)
south_fill    this is used by the pawn_moves function to get the appropriate
              blockers of the pawn.

# magicbitb.cpp
magicbitb.cpp is a series of functions that help make the perfect hash table of
the slider piece movements, the way this has been done is through creating a
series of psuedo-random low bit non 0 number and testing to see if it creates
zero clashes.
Most functions do not do anything complex and do waht their names suggest but
the more complex ones will get a further explanation.

The functions are:

rmask     they create a bitmask of where the relevant blockers could be
and     - positioned if the rook or bishop were to be placed on the given
bmask     square

rAtk      They take in a set of relevant blockers and create the corresponding
and     - attack bit map for the rook(rAtk) and the bishop(bAtk)
bAtk      

transform - Essentially a hash function that uses the corresponding magic
            number for the given square and slider piece(rook or bishop)
            to find the corresponding index of the correct attack bit map

find_magic - uses all the functions before to find the magic number for the
             given square and slider piece for every combonation of blocker
             and store the corresponding attack map

find_all_sq_magics - makes use of find_magics to find all the magic numbers for
                     every square for both slider pieces as well as making the
                     attack map hastable for the corresponding square

Note: eventually the magics and attack maps shall be made to store in a file so
      that you do not need to calculate them every time at the beginning of the
      game saving some time

# testing_functions.cpp
A function used to test all the new functions that have been made in both
classes and any other files so that we can ensure they are all working as
intended, will be removed once development has finished.

# game.cpp
Once all the functions and classes have been finished, the main game will
come together here and be playable

# player.cpp
Thoughts of a player class crossed my mind and unsure if its needed but it
will remain here until my mind has been made up on wether to keep it or not
