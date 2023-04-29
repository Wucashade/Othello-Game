
#include "AIOne.hpp"



#define GET_BIT(bb, square) (bb & (1ULL << square))
#define SET_BIT(bb, square) (bb |= (1ULL << square))
#define CORNER_MASK 0x8100000000000081ULL
#define C_SQUARES_MASK 0x4281000000008142ULL
#define X_SQUARES_MASK 0x42000000004200ULL
#define B_SQUARES_MASK 0x1800008181000018ULL
#define A_SQUARES_MASK 0x2400810000810024ULL
#define S_SQUARES_MASK 0x240000240000ULL
#define INNER_MIDDLE_MASK 0x183C3C180000ULL
#define OUTER_MIDDLE_MASK 0x3C424242423C00ULL

using namespace std;

enum squares
{
    a1,b1,c1,d1,e1,f1,g1,h1,
    a2,b2,c2,d2,e2,f2,g2,h2,
    a3,b3,c3,d3,e3,f3,g3,h3,
    a4,b4,c4,d4,e4,f4,g4,h4,
    a5,b5,c5,d5,e5,f5,g5,h5,
    a6,b6,c6,d6,e6,f6,g6,h6,
    a7,b7,c7,d7,e7,f7,g7,h7,
    a8,b8,c8,d8,e8,f8,g8,h8
};

enum state
{
    WHITE_MOVE,
    BLACK_MOVE,
    GAME_OVER
};

/*
    Masks to ignore moves that go out of bitboard bounds
    Source for masks: https://www.chessprogramming.org/General_Setwise_Operations#Shifting_Bitboards
*/
U64 avoidWrap[8] =
    {
        0xfefefefefefefe00, // UP-LEFT
        0xfefefefefefefefe, // LEFT
        0x00fefefefefefefe, // DOWN-LEFT
        0x00ffffffffffffff, // DOWN
        0x007f7f7f7f7f7f7f, // DOWN-RIGHT
        0x7f7f7f7f7f7f7f7f, // RIGHT
        0x7f7f7f7f7f7f7f00, // UP-RIGHT
        0xffffffffffffff00, // UP
};

int shift[8] = {9, 1, -7, -8, -9, -1, 7, 8};

/*
   9     8    7
     \   |   /
      \  | /
  1 --- Bit --- -1
       / | \
     /   |   \
   -7    -8   -9
*/

/*
The function shiftOne shifts the bit in the 8 different directions
that are shown above.
*/

AIOne::AIOne(){};

U64 AIOne::shiftOne(U64 bb, int dir8)
{
    if (dir8 < 0 || dir8 > 7) // Check to make sure that the direction exists in the array
    {
        cout << "Invalid direction" << endl;
    }
    int a = shift[dir8];
    return (a > 0) ? ((bb << a) & avoidWrap[dir8]) : ((bb >> -a) & avoidWrap[dir8]);
    // Shifts left if the value is positive and right if negative, also applies mask so that the bits don't wrap around to the other side of the bitboard if theyre at the end
}

int AIOne::popCount(U64 bb)
{
    int count = 0;
    if (bb == 0)
    {
        return count; // Won't waste time reading if bitboard is empty
    }
    else
    {
        while (bb) // Utilisiing Brian Khernighan's way
        {
            bb &= bb - 1;
            count++;
        }
        return count;
    }
}

U64 AIOne::generateMoves(U64 bbOwn, U64 bbOpponent)
{
    U64 emptyBB = ~(bbOwn | bbOpponent);
    U64 moves;
    U64 legalMoves = 0;

    for (int i = 0; i < 8; i++)
    {

        moves = shiftOne(bbOwn, i) & bbOpponent;

        moves |= shiftOne(moves, i) & bbOpponent;
        moves |= shiftOne(moves, i) & bbOpponent;
        moves |= shiftOne(moves, i) & bbOpponent;
        moves |= shiftOne(moves, i) & bbOpponent;
        moves |= shiftOne(moves, i) & bbOpponent;

        legalMoves |= shiftOne(moves, i) & emptyBB;
    }

    return legalMoves;
}

bool AIOne::isValid(U64 bbOwn, U64 bbOpponent, int index)
{
    U64 checkedBit = 1ULL << index;
    return (generateMoves(bbOwn, bbOpponent) & checkedBit) != 0;
}

void AIOne::commitMove(U64 *bbOwn, U64 *bbOpponent, int index)
{

    U64 moves, disk;
    U64 newDisk = 1ULL << index;
    U64 capturedDisks = 0;

    if (isValid(*bbOwn, *bbOpponent, index) == 0)
    {
        cout << "Invalid Move" << endl;
    }
    else
    {
        *bbOwn |= newDisk;

        for (int i = 0; i < 8; i++)
        {

            moves = shiftOne(newDisk, i) & *bbOpponent;

            moves |= shiftOne(moves, i) & *bbOpponent;
            moves |= shiftOne(moves, i) & *bbOpponent;
            moves |= shiftOne(moves, i) & *bbOpponent;
            moves |= shiftOne(moves, i) & *bbOpponent;
            moves |= shiftOne(moves, i) & *bbOpponent;

            disk = shiftOne(moves, i) & *bbOwn;
            capturedDisks |= (disk ? moves : 0);
        }

        *bbOwn ^= capturedDisks;
        *bbOpponent ^= capturedDisks;
    }
}

int AIOne::evaluateMove(U64 bbOwn, U64 bbOpponent, U64 ownMoves, U64 oppMoves)
{

    int ownCount, oppCount;
    U64 ownCorners = bbOwn & CORNER_MASK;
    U64 oppCorners = bbOpponent & CORNER_MASK;
    int value = 0;

    if (!ownMoves && !oppMoves)
    {
        /* Terminal state. */
        ownCount = popCount(bbOwn);
        oppCount = popCount(bbOpponent);
        return (ownCount - oppCount) * (1 << 20);
    }

    value = value + ((popCount(ownCorners) - popCount(oppCorners)) * 20);
    value = value + ((popCount(ownMoves) - popCount(oppMoves)) * 5);
    value = value + ((rand() % 20) * 1);

    return value;
}

int AIOne::searchMove(U64 bbOwn, U64 bbOpponent, int maxDepth, int alpha, int beta,
               int *bestMove, int *evalCount)
{

    U64 ownNewDisks, oppNewDisks;
    U64 ownMoves = generateMoves(bbOwn, bbOpponent);
    U64 oppMoves = generateMoves(bbOpponent, bbOwn);

    if (!ownMoves && oppMoves)
    {
        return -searchMove(bbOpponent, bbOwn, maxDepth, -beta, -alpha,
                           bestMove, evalCount);
    }

    if (maxDepth == 0 || (!ownMoves && !oppMoves))
    {
        ++*evalCount;
        return evaluateMove(bbOwn, bbOpponent, ownMoves, oppMoves);
    }

    int best = -INT_MAX;
    for (int i = 0; i < 64; i++)
    {
        if (!(ownMoves & (1ULL << i)))
        {
            continue;
        }

        ownNewDisks = bbOwn;
        oppNewDisks = bbOpponent;
        commitMove(&ownNewDisks, &oppNewDisks, i);

        int a = -searchMove(bbOpponent, bbOwn, maxDepth - 1, -beta, -alpha,
                            NULL, evalCount);

        if (a > best)
        {
            best = a;
            if (bestMove)
            {
                *bestMove = i;
            }
            alpha = a > alpha ? a : alpha;

            if (alpha >= beta)
            {
                break;
            }
        }
    }
    return best;
}

int AIOne::iterativeSearchMove(U64 &bbOwn, U64 &bbOpponent,
                               int startDepth, int evalBudget)
{
    int depth, bestMove, evalCount, s;

    evalCount = 0;
    bestMove = -1;
    for (depth = startDepth; evalCount < evalBudget; depth++)
    {
        s = searchMove(bbOwn, bbOpponent, depth, -INT_MAX, INT_MAX,
                       &bestMove, &evalCount);
        if (s >= (1 << 20) || -s >= (1 << 20))
        {
            break;
        }
    }

    return bestMove;
}

void AIOne::computeMove(U64 &bbOwn, U64 &bbOpponent, int *row, int *col)
{
    int move_idx;

    static const int START_DEPTH = 8;
    static const int EVAL_BUDGET = 10000;

    move_idx = iterativeSearchMove(bbOwn, bbOpponent,
                                   START_DEPTH, EVAL_BUDGET);

    cout << move_idx << endl;

    *row = move_idx / 8;
    *col = move_idx % 8;
}

