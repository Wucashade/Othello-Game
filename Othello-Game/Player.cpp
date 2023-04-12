#include "Player.hpp"

Player::Player(U64 bitboard, int diskNum, bool isHumanPlayer)
{
    pieceColour = bitboard;
    score = diskNum;
    isHuman = isHumanPlayer;

};

U64 Player::getBitboard()
{
    return pieceColour;
}

int Player::getScore()
{
    return score;
}

bool Player::isPlayerHuman()
{
    return isHuman;
}

