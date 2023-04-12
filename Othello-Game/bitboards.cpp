
#include <iostream>
#include <bit>
#include <bitset>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <cstdlib>
#include <time.h>
#include "Bitboards.hpp"

typedef uint64_t U64;
#define GET_BIT(bb, square) (bb & (1ULL << square))
#define SET_BIT(bb, square) (bb |= (1ULL << square))
#define CORNER_MASK 0x8100000000000081ULL
// Credits to @chessprogramming591 teaching these macros.
// Link : https://www.youtube.com/watch?v=o-ySJ2EBarY&list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs&index=2

using namespace std;

U64 bitboardWhite = 0ULL;
U64 bitboardBlack = 0ULL;

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

U64 shiftOne(U64 bb, int dir8)
{
    if (dir8 < 0 || dir8 > 7) // Check to make sure that the direction exists in the array
    {
        cout << "Invalid direction" << endl;
    }
    int a = shift[dir8];
    return (a > 0) ? ((bb << a) & avoidWrap[dir8]) : ((bb >> -a) & avoidWrap[dir8]);
    // Shifts left if the value is positive and right if negative, also applies mask so that the bits don't wrap around to the other side of the bitboard if theyre at the end
}

// Function to print out the current bitboard

void printBitboard(U64 bbOne, U64 bbTwo)
{
    if (bbOne == bitboardWhite)
    {
        for (int i = 0; i < 8; i++)
        {
            cout << i+1 << "  "; // Prints the rank
            for (int j = 0; j < 8; j++)
            {
                int square = i * 8 + j;
                cout << (GET_BIT(bbOne, square) ? "W" : (GET_BIT(bbTwo, square) ? "B" : "0")) << " ";
                // Checks the first bitboard if there is a bit, then the second bitboard and prints appropriate letters
                // If neither bitboards have a bit in that space then it prints a "0"
            }
            cout << "" << endl;
        }
        cout << endl
             << "   a b c d e f g h" << endl; // Prints the file
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            cout << i+1 << "  "; // Prints the rank
            for (int j = 0; j < 8; j++)
            {
                int square = i * 8 + j;
                cout << (GET_BIT(bbOne, square) ? "B" : (GET_BIT(bbTwo, square) ? "W" : "0")) << " ";
                // Checks the first bitboard if there is a bit, then the second bitboard and prints appropriate letters
                // If neither bitboards have a bit in that space then it prints a "0"
            }
            cout << "" << endl;
        }
        cout << endl
             << "   a b c d e f g h" << endl; // Prints the file
    }
}

void printSingleBitboard(U64 bb)
{
    for (int i = 7; i > -1; i--)
    {
        cout << 8 - i << "  "; // Prints the rank
        for (int j = 7; j > -1; j--)
        {
            int square = i * 8 + j;
            cout << (GET_BIT(bb, square) ? 1 : 0) << " "; // Prints either a 1 or 0 for square
        }
        cout << "" << endl;
    }
    cout << endl
         << "   a b c d e f g h" << endl; // Prints the file
}

int popCount(U64 bb)
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

U64 generateMoves(U64 bbOwn, U64 bbOpponent)
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

bool isValid(U64 bb, int index)
{
    U64 checkedBit = 1ULL << index;
    if (bb == bitboardWhite)
    {
        return (generateMoves(bitboardWhite, bitboardBlack) & checkedBit) != 0;
    }
    else
    {
        return (generateMoves(bitboardBlack, bitboardWhite) & checkedBit) != 0;
    }
}

void commitMove(U64 *bbOwn, U64 *bbOpponent, int index)
{

    U64 moves, disk;
    U64 newDisk = 1ULL << index;
    U64 capturedDisks = 0;

    if (isValid(*bbOwn, index) == 0)
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

int evaluateMove(U64 bbOwn, U64 bbOpponent, U64 ownMoves, U64 oppMoves)
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

int searchMove(U64 bbOwn, U64 bbOpponent, int maxDepth, int alpha, int beta,
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

static int iterativeSearchMove(U64 &bbOwn, U64 &bbOpponent,
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

void computeMove(U64 &bbOwn, U64 &bbOpponent, int *row, int *col)
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

void game()
{

    int row = 0, col = 0;
    int cnt = 0;

    SET_BIT(bitboardWhite, d4);
    SET_BIT(bitboardWhite, e5);
    SET_BIT(bitboardBlack, e4);
    SET_BIT(bitboardBlack, d5);
    

    state s = BLACK_MOVE;

    clock_t tStart = clock();

    while (!(s == GAME_OVER))
    {

        if (s == BLACK_MOVE)
        {

            printBitboard(bitboardWhite, bitboardBlack);

            cnt = popCount(bitboardBlack);
            cout << "BLACK: " << cnt << endl;
            cnt = popCount(bitboardWhite);
            cout << "WHITE: " << cnt << endl;
            s = WHITE_MOVE;

            cout << "BLACK MOVE" << endl;
            /*
            while (!(isValid(bitboardBlack, (row * 8 + col))))
            {
                cout << "Enter row: ";
                cin >> row;
                cout << "Enter col: ";
                cin >> col;
            }
            */
            computeMove(bitboardBlack, bitboardWhite, &row, &col);
            commitMove(&bitboardBlack, &bitboardWhite, (row * 8 + col));
            cout << "Time: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
        }
        if (generateMoves(bitboardBlack, bitboardWhite) == 0 && generateMoves(bitboardWhite, bitboardBlack) == 0)
        {
            s = GAME_OVER;
            cout << "GAME OVER" << endl;
            if (popCount(bitboardWhite) > popCount(bitboardBlack))
            {
                cout << "WHITE WINS!" << endl;
            }
            else if (popCount(bitboardBlack) > popCount(bitboardWhite))
            {
                cout << "BLACK WINS!" << endl;
            }
        }
        else if (s == BLACK_MOVE && generateMoves(bitboardBlack, bitboardWhite) == 0)
        {
            s = WHITE_MOVE;
            cout << "hi";
        }
        else if (s == WHITE_MOVE && generateMoves(bitboardWhite, bitboardBlack) == 0)
        {
            s = BLACK_MOVE;
        }
        if (s == WHITE_MOVE)
        {

            printBitboard(bitboardWhite, bitboardBlack);

            cnt = popCount(bitboardBlack);
            cout << "BLACK: " << cnt << endl;
            cnt = popCount(bitboardWhite);
            cout << "WHITE: " << cnt << endl;
            s = BLACK_MOVE;

            cout << "WHITE MOVE" << endl;
            computeMove(bitboardWhite, bitboardBlack, &row, &col);
            commitMove(&bitboardWhite, &bitboardBlack, (row * 8 + col));

            cout << "Time: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
        }
    }
}

int main()
{
    game();

    return 0;
}