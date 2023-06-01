
#include "AIOne.hpp"



#define GET_BIT(bb, square) (bb & (1ULL << square))
#define SET_BIT(bb, square) (bb |= (1ULL << square))
#define WIN_BONUS (1 << 20)


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
static U64 avoidWrap[8] =
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

static int shift[8] = {9, 1, -7, -8, -9, -1, 7, 8};

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

static U64 squareDefinitions[8] =
{
    0x8100000000000081ULL, // Corner mask
    0x4281000000008142ULL, //C squares mask
    0x42000000004200ULL, // X squares mask
    0x1800008181000018ULL,   // B squares mask
    0x2400810000810024ULL,    // A squares mask
    0x240000240000ULL,   // S squares mask
    0x183C3C180000ULL, // Inner middle mask
    0x3C424242423C00ULL   // Outer middle mask
};

static int squareEarlyValues[8] = {50, -10, -20, -1, 5, 5, -1, -3}; //{30, -12, -15, -1, 0, 0, -1, -3}
static int squareMidValues[8] = {50, -10, -20, 6, 10, 2, 1, -4};    //{50, -10, -20, 6, 10, 2, 1, -4}
static int squareEndValues[8] = {100, -15, -25, 5, 10, 5, 2, 1};    //{100, -25, -25, 5, 10, 5, 2, 1}

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

    assert((bbOwn & bbOpponent) == 0 && "Disk sets should be disjoint.");

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

void AIOne::commitMove(AIOne* aiOne, U64 *bbOwn, U64 *bbOpponent, int index)
{

    U64 moves, disk;
    U64 newDisk = 1ULL << index;
    U64 capturedDisks = 0;

    assert(index < 64 && "Move must be within the board.");
    assert((*bbOwn & *bbOpponent) == 0 && "Disk sets must be disjoint.");
    assert(!((*bbOwn | *bbOpponent) & newDisk) && "Target not empty!");
   
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

int AIOne::frontierDiscs(U64 bbOwn, U64 bbOpponent)
{
        U64 bbEmpty = ~(bbOwn | bbOpponent);
        U64 testFront;
        int value;

        U64 myFront = 0;
        U64 oppFront = 0;

        for (int i = 0; i < 8; i++) {
                testFront = shiftOne(bbEmpty, i);
                myFront |= testFront & bbOwn;
                oppFront |= testFront & bbOpponent;
        }

        value = popCount(oppFront) - popCount(myFront);
        return value;
}

int AIOne::discSquareValue(U64 bbOwn, U64 bbOpponent)
{

    int value;
    int discsPlaced = popCount(bbOwn)+popCount(bbOpponent);

    for(int i = 0; i<(sizeof(squareDefinitions)/sizeof(squareDefinitions[0])); i++)
    {
        U64 ownCheckedSquares = bbOwn &  squareDefinitions[i];
        U64 oppCheckedSquares = bbOpponent &  squareDefinitions[i];

        if(discsPlaced < 25)
        {
            value += popCount(ownCheckedSquares) * squareEarlyValues[i];
            value -= popCount(oppCheckedSquares) * squareEarlyValues[i];
        }
        else if(discsPlaced < 45 && discsPlaced > 24)
        {
            value += popCount(ownCheckedSquares) * squareMidValues[i];
            value -= popCount(oppCheckedSquares) * squareMidValues[i];
        }
        else
        {
            value += popCount(ownCheckedSquares) * squareEndValues[i];
            value -= popCount(oppCheckedSquares) * squareEndValues[i];
        }
    }

    return value;
}

int AIOne::evaluateMove(U64 bbOwn, U64 bbOpponent, U64 ownMoves, U64 oppMoves)
{
    int discsPlaced;
    int value = 0;
    U64 ownCheckedSquares, oppCheckedSquares;
    
    if (!ownMoves && !oppMoves)
    {
        return (popCount(bbOwn) - popCount(bbOpponent));
    }
    
    discsPlaced = popCount(bbOwn)+popCount(bbOpponent);
    //EVALUATION FOR EARLY GAME
    if(discsPlaced < 25)
    {
        value += discSquareValue(bbOwn, bbOpponent) * 20;  //POSITION
        value += (popCount(ownMoves) - popCount(oppMoves)) * 80; //MOBILITY
        value += frontierDiscs(bbOwn, bbOpponent) * 40;  //FRONTIER
    }
    //EVALUATION FOR MID GAME
    else if(discsPlaced < 45 && discsPlaced > 24)
    {
        value += discSquareValue(bbOwn, bbOpponent) * 40;  //POSITION
        value += (popCount(ownMoves) - popCount(oppMoves)) * 60; //MOBILITY
        value += frontierDiscs(bbOwn, bbOpponent) * 70;  //FRONTIER
    }
    //EVALUATION FOR END GAME
    else
    {
        value += discSquareValue(bbOwn, bbOpponent) * 50;  //POSITION
        value += (popCount(ownMoves) - popCount(oppMoves)) * 20; //MOBILITY
        value += frontierDiscs(bbOwn, bbOpponent) * 20;  //FRONTIER
    }

    return value;
}

int AIOne::searchMove(AIOne* aiOne, U64 bbOwn, U64 bbOpponent, int maxDepth, int alpha, int beta,
               int *bestMove, int *evalCount)
{

    U64 ownNewDisks, oppNewDisks;
    U64 ownMoves, oppMoves;
    int a, best;

    ownMoves = generateMoves(bbOwn, bbOpponent);
    oppMoves = generateMoves(bbOpponent, bbOwn);

    if (!ownMoves && oppMoves)
    {
        return -searchMove(aiOne, bbOpponent, bbOwn, maxDepth, -beta, -alpha,
                           bestMove, evalCount);
    }

    if (maxDepth == 0 || (!ownMoves && !oppMoves))
    {
        ++*evalCount;
        return evaluateMove(bbOwn, bbOpponent, ownMoves, oppMoves);
    }
    best = -INT_MAX;
    for (int i = 0; i < 64; i++)
    {
        if (!(ownMoves & (1ULL << i)))
        {
            continue;
        }

        ownNewDisks = bbOwn;
        oppNewDisks = bbOpponent;
        commitMove(aiOne, &ownNewDisks, &oppNewDisks, i);

        a = -searchMove(aiOne, oppNewDisks, ownNewDisks, maxDepth - 1, -beta, -alpha,
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

int AIOne::iterativeSearchMove(AIOne* aiOne, U64 &bbOwn, U64 &bbOpponent,
                               int startDepth, int evalBudget)
{
    int depth, bestMove, evalCount, s;

    assert(startDepth > 0 && "At least one move must be explored.");

    evalCount = 0;
    bestMove = 0;
    for (depth = startDepth; evalCount < evalBudget; depth++)
    {
        s = searchMove(aiOne, bbOwn, bbOpponent, depth, -INT_MAX, INT_MAX,
                       &bestMove, &evalCount);
        if (s >= WIN_BONUS || -s >= WIN_BONUS)
        {
            break;
            
        }
        
    }
    assert(bestMove != -1 && "No move found?");
    return bestMove;
}

void AIOne::computeMove(AIOne* aiOne, U64 &bbOwn, U64 &bbOpponent, int *row, int *col)
{
    int move_idx;

    int startDepth;

    if((popCount(bbOwn) + popCount(bbOpponent)) > 44)
    {
        startDepth = 16;
    }
    else
    {
        startDepth = 8;
    }
    static const int evaluationBudget = 250000;

    move_idx = iterativeSearchMove(aiOne, bbOwn, bbOpponent,
                                   startDepth, evaluationBudget);


    *row = move_idx / 8;
    *col = move_idx % 8;
}


