#include <iostream>
#include "Board.hpp"

typedef uint64_t U64;

class Player
{

    public:

        Player(U64 bitboard, int diskNum, bool isHumanPlayer);
        ~Player();
        U64 getBitboard();
        int getScore();
        bool isPlayerHuman();
        


    private:

        U64 pieceColour;
        int score;
        bool isHuman;

};