
#include<iostream>
#include <bit>
#include <bitset>
#include <cstdint>
#include <initializer_list>
#include <string>

typedef uint64_t U64;
#define GET_BIT(bb, square) (bb & (1ULL << square))
#define SET_BIT(bb, square) (bb |= (1ULL << square))
#define CORNER_MASK 0x8100000000000081ULL
//Credits to @chessprogramming591 teaching these macros. 
//Link : https://www.youtube.com/watch?v=o-ySJ2EBarY&list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs&index=2

using namespace std;


U64 bitboardWhite = 0ULL;
U64 bitboardBlack = 0ULL;

enum squares {
  h8, g8, f8, e8, d8, c8, b8, a8,
  h7, g7, f7, e7, d7, c7, b7, a7,
  h6, g6, f6, e6, d6, c6, b6, a6,
  h5, g5, f5, e5, d5, c5, b5, a5,
  h4, g4, f4, e4, d4, c4, b4, a4,
  h3, g3, f3, e3, d3, c3, b3, a3,
  h2, g2, f2, e2, d2, c2, b2, a2,
  h1, g1, f1, e1, d1, c1, b1, a1
};

/*
    Masks to ignore moves that go out of bitboard bounds
    Source for masks: https://www.chessprogramming.org/General_Setwise_Operations#Shifting_Bitboards
*/
U64 avoidWrap[8] =
{
   0xfefefefefefefe00, //UP-LEFT
   0xfefefefefefefefe, //LEFT
   0x00fefefefefefefe, //DOWN-LEFT
   0x00ffffffffffffff, //DOWN
   0x007f7f7f7f7f7f7f, //DOWN-RIGHT
   0x7f7f7f7f7f7f7f7f, //RIGHT
   0x7f7f7f7f7f7f7f00, //UP-RIGHT
   0xffffffffffffff00, //UP
};

int shift[8] = {9, 1,-7,-8,-9,-1, 7, 8}; 

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

U64 shiftOne (U64 bb, int dir8) {
    if(dir8 < 0 || dir8 > 7)        // Check to make sure that the direction exists in the array
    {
        cout << "Invalid direction" << endl;
    }
    int a = shift[dir8]; 
    return (a > 0) ? ((bb << a)&avoidWrap[dir8]):((bb >> -a)&avoidWrap[dir8]); 
    //Shifts left if the value is positive and right if negative, also applies mask so that the bits don't wrap around to the other side of the bitboard if theyre at the end

}

//Function to print out the current bitboard


void printBitboard(U64 bbOne, U64 bbTwo)
{
    if(bbOne == bitboardWhite)
    {
        for (int i = 7; i > -1; i--)
        {
            cout << 8-i << "  ";    // Prints the rank
            for(int j = 7; j > -1; j--)
            {
                int square = i * 8 + j;
                cout << (GET_BIT(bbOne, square) ? "W":(GET_BIT(bbTwo, square) ? "B":"0")) << " ";
                // Checks the first bitboard if there is a bit, then the second bitboard and prints appropriate letters
                //If neither bitboards have a bit in that space then it prints a "0"

            }
        cout << "" << endl;
        }
        cout << endl << "   a b c d e f g h" << endl;   // Prints the file
    }
    else{
        for (int i = 7; i > -1; i--)
        {
            cout << 8-i << "  ";    // Prints the rank
            for(int j = 7; j > -1; j--)
            {
                int square = i * 8 + j;
                cout << (GET_BIT(bbOne, square) ? "B":(GET_BIT(bbTwo, square) ? "W":"0")) << " ";
                // Checks the first bitboard if there is a bit, then the second bitboard and prints appropriate letters
                //If neither bitboards have a bit in that space then it prints a "0"

            }
            cout << "" << endl;
        }
        cout << endl << "   a b c d e f g h" << endl;   // Prints the file
    }
}

void printSingleBitboard(U64 bb)
{
    for (int i = 7; i > -1; i--)
    {
        cout << 8-i << "  ";    // Prints the rank
        for(int j = 7; j > -1; j--)
        {
            int square = i * 8 + j;
            cout << ( GET_BIT(bb, square) ? 1:0 )<< " ";    //Prints either a 1 or 0 for square

        }
        cout << "" << endl;
    }
    cout << endl << "   a b c d e f g h" << endl;   // Prints the file
}

int popCount(U64 bb)
{
    int count = 0;
    if (bb == 0)  
    {
        return count;   //Won't waste time reading if bitboard is empty
    }
    else
    {
        while(bb) // Utilisiing Brian Khernighan's way
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

    for(int i = 0; i < 8; i++)
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

bool isValid(U64 bb, int col, int row)
{
    U64 checkedBit = 1ULL << (row * 8 + col );
    if(bb == bitboardWhite)
    {
        return (generateMoves(bitboardWhite, bitboardBlack) &  checkedBit) != 0;
    }
    else
    {
        return (generateMoves(bitboardBlack, bitboardWhite) &  checkedBit) != 0;
    }


}

void commitMove(U64 *bbOwn, U64 *bbOpponent, int col, int row)
{

    U64 moves, disk;
    U64 newDisk = 1ULL << (row * 8 + col);
    U64 capturedDisks = 0;

    
    if (isValid(*bbOwn, col, row) == 0)
    {
        cout << "Invalid Move" << endl;
    }
    else
    {
        *bbOwn |= newDisk;

        for(int i = 0; i < 8; i++)
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

    value = value + ((popCount(ownCorners) - popCount(oppCorners)) * 20);
    value = value + ((popCount(ownMoves) - popCount(oppMoves)) * 5);

    return value;

}

int main()
{


    int cnt = 0;

    SET_BIT(bitboardWhite, d4);
    SET_BIT(bitboardWhite, e5);
    SET_BIT(bitboardBlack, e4);
    SET_BIT(bitboardBlack, d5);
    SET_BIT(bitboardWhite, f5);
    /*
    cout << bitboardWhite<<endl;
    
    printBitboard(bitboardWhite, bitboardBlack);
    printSingleBitboard(bitboardBlack);
    printSingleBitboard(bitboardWhite);

    cnt = popCount(bitboardBlack);
    cout << "BLACK: "<<cnt << endl;
    cnt = popCount(bitboardWhite);
    cout << "WHITE: "<<cnt << endl;

*/

    printBitboard(bitboardWhite, bitboardBlack);
    
    printSingleBitboard(generateMoves(bitboardWhite, bitboardBlack));

    commitMove(&bitboardWhite, &bitboardBlack, 4, 2);

    printSingleBitboard(generateMoves(bitboardBlack, bitboardWhite));

    commitMove(&bitboardBlack, &bitboardWhite, 5, 2);

    printBitboard(bitboardWhite, bitboardBlack);

    cnt = popCount(bitboardBlack);
    cout << "BLACK: "<<cnt << endl;
    cnt = popCount(bitboardWhite);
    cout << "WHITE: "<<cnt << endl;

    cout << evaluateMove(bitboardWhite, bitboardBlack, generateMoves(bitboardWhite, bitboardBlack), generateMoves(bitboardBlack, bitboardWhite)) << endl;
    cout << evaluateMove(bitboardBlack, bitboardWhite, generateMoves(bitboardBlack, bitboardWhite), generateMoves(bitboardWhite, bitboardBlack)) << endl;


    return 0;


}