#include <iostream>
#pragma once

typedef uint64_t U64;

class Player
{

    public:

        Player(U64 bitboard, int diskNum, bool isHumanPlayer);
        ~Player();
        U64 getBitboard();
        int getScore();
        bool isPlayerHuman();
        void setBitboard();

        U64 pieceColour;

    private:

        
        int score;
        bool isHuman;

        

};