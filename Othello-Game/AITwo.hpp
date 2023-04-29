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


    U64 shiftOne(U64 bb, int dir8);
    int popCount(U64 bb);
    U64 generateMoves(U64 bbOwn, U64 bbOpponent);
    bool isValid(U64 bbOwn, U64 bbOpponent, int index);
    void commitMove(U64 *bbOwn, U64 *bbOpponent, int index);
    int evaluateMove(U64 bbOwn, U64 bbOpponent, U64 ownMoves, U64 oppMoves);
    int searchMove(U64 bbOwn, U64 bbOpponent, int maxDepth, int alpha, int beta, int *bestMove, int *evalCount);
    int iterativeSearchMove(U64 &bbOwn, U64 &bbOpponent, int startDepth, int evalBudget);
    void computeMove(U64 &bbOwn, U64 &bbOpponent, int *row, int *col);

};
