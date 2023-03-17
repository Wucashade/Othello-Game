
#include<iostream>
#include <bit>
#include <bitset>
#include <cstdint>
#include <initializer_list>
#include <string>

typedef uint64_t U64;
#define GET_BIT(bb, square) (bb & (1ULL << square))
#define SET_BIT(bb, square) (bb |= (1ULL << square))

using namespace std;


U64 bitboardWhite = 0ULL;
U64 bitboardBlack = 0ULL;
U64 emptyBB;
U64 occupiedBB;
/*
//Board representation as an enum
enum squares {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8
};
*/
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

enum colours{DISKWHITE, DISKBLACK, DISKEMPTY};

/*
    Masks to ignore moves that go out of bitboard bounds
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

U64 shiftOne (U64 bb, int dir8) {
    if(dir8 < 0 || dir8 > 7)
    {
        cout << "Invalid direction" << endl;
    }
    int r = shift[dir8]; // {+-1,7,8,9}
    return (r > 0) ? ((bb << r)&avoidWrap[dir8]):((bb >> -r)&avoidWrap[dir8]);

}

//Function to print out the current bitboard


void printBitboard(U64 bbOne, U64 bbTwo)
{
    char whiteSquare[] = "W";
    for (int i = 7; i > -1; i--)
    {
        cout << 8-i << "  ";
        for(int j = 7; j > -1; j--)
        {
            int square = i * 8 + j;
            cout << (GET_BIT(bbOne, square) ? "W":(GET_BIT(bbTwo, square) ? "B":"0")) << " ";
            /*
            //cout << ( GET_BIT(bb, square) ? 1:0 )<< " ";
            
            if((bbOne & (1ULL << square)) == 1)
            {
                cout << "W ";
                
            }
            
            else if (GET_BIT(bbTwo, square) == 1)
            {
                cout << "B ";
            }
            else
            {
                cout << "0 ";
            }

            */
        }
        cout << "" << endl;
    }
    cout << endl << "   a b c d e f g h" << endl;
}

void printSingleBitboard(U64 bb)
{
    for (int i = 7; i > -1; i--)
    {
        cout << 8-i << "  ";
        for(int j = 7; j > -1; j--)
        {
            int square = i * 8 + j;
            cout << ( GET_BIT(bb, square) ? 1:0 )<< " ";

        }
        cout << "" << endl;
    }
    cout << endl << "   a b c d e f g h" << endl;
}

int popCount(U64 bb)
{
    int count = 0;
    if (bb == 0)  
    {
        return count;
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
    emptyBB = ~(bbOwn | bbOpponent);
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

int main()
{

    int cnt = 0;
    SET_BIT(bitboardWhite, d4);
    SET_BIT(bitboardWhite, e5);
    SET_BIT(bitboardBlack, e4);
    SET_BIT(bitboardBlack, d5);
    SET_BIT(bitboardWhite, c4);
    
    printBitboard(bitboardWhite, bitboardBlack);

    cnt = popCount(bitboardBlack);
    cout << "BLACK: "<<cnt << endl;
    cnt = popCount(bitboardWhite);
    cout << "WHITE: "<<cnt << endl;


    printSingleBitboard(generateMoves(bitboardBlack, bitboardWhite));

    
    return 0;


}