#include <iostream>
#include <bit>
#include <bitset>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <cstdlib>
#include <time.h>

typedef uint64_t U64;

class AITwo
{

    public:

    AITwo();


    static U64 shiftOne(U64 bb, int dir8);
    static int popCount(U64 bb);
    static U64 generateMoves(U64 bbOwn, U64 bbOpponent);
    bool isValid(U64 bbOwn, U64 bbOpponent, int index);
    static void commitMove(AITwo* aiTwo, U64 *bbOwn, U64 *bbOpponent, int index);
    static void frontierDisks(U64 bbOwn, U64 bbOpponent, U64 *myFront, U64 *oppFront);
    static int discSquareValue(U64 bbOwn, U64 bbOpponent);
    static int evaluateMove(U64 bbOwn, U64 bbOpponent, U64 ownMoves, U64 oppMoves);
    static int searchMove(AITwo* aiTwo, U64 bbOwn, U64 bbOpponent, int maxDepth, int alpha, int beta, int *bestMove, int *evalCount);
    static int iterativeSearchMove(AITwo* aiTwo, U64 &bbOwn, U64 &bbOpponent, int startDepth, int evalBudget);
    void computeMove(AITwo* aiTwo, U64 &bbOwn, U64 &bbOpponent, int *row, int *col);

};
