
#include<iostream>
#include <bit>
#include <bitset>
#include <cstdint>
#include <initializer_list>
#include <string>

typedef uint64_t U64;
#define getBit(bb, square) (bb & (1ULL << square))
#define setBit(bb, square) (bb |= (1ULL << square))

using namespace std;


U64 bitboardWhite = 0ULL;
U64 bitboardBlack = 0ULL;
U64 emptyBB;
U64 occupiedBB;

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

enum colours{DISKWHITE, DISKBLACK, DISKEMPTY};

/*
    Masks to ignore moves that go out of bitboard bounds
*/

const U64 notLeft = 0xFEFEFEFEFEFEFEFEULL,
          notRight = 0x7F7F7F7F7F7F7F7FULL,
          notDownRight = 0x007F7F7F7F7F7F7FULL,
          notDown = 0xFFFFFFFFFFFFFFFFULL,
          notDownLeft = 0x00FEFEFEFEFEFEFEULL,
          notUpLeft = 0xFEFEFEFEFEFEFE00ULL,
          notUp = 0xFFFFFFFFFFFFFFFFULL,
          notUpRight = 0x7F7F7F7F7F7F7F00ULL;

/*
    Different bitboard shifts to simulate movement
*/
U64 northShift (U64 bb) 
{
    return  bb << 8;
}
U64 northEastShift (U64 bb) 
{
    return  (bb & notUpRight) << 7;
}
U64 eastShift (U64 bb) 
{
    return  (bb & notRight) >> 1;
}
U64 southEastShift (U64 bb) 
{
    return  (bb & notDownRight) >> 9;
}
U64 southShift (U64 bb) 
{
    return  bb >> 8;
}
U64 southWestShift (U64 bb) 
{
    return  (bb & notDownLeft) >> 7;
}
U64 westShift (U64 bb) 
{
    return  (bb & notLeft) << 1;
}
U64 northWestShift (U64 bb) 
{
    return  (bb & notUpLeft) << 9;
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
            cout << (getBit(bbOne, square) ? "W":(getBit(bbTwo, square) ? "B":"0")) << " ";
            /*
            //cout << ( getBit(bb, square) ? 1:0 )<< " ";
            
            if((bbOne & (1ULL << square)) == 1)
            {
                cout << "W ";
                
            }
            
            else if (getBit(bbTwo, square) == 1)
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



}

int main()
{

    int cnt = 0;
    setBit(bitboardBlack, e5);
    setBit(bitboardBlack, d4);
    setBit(bitboardWhite, e4);
    setBit(bitboardWhite, d5);
    
    printBitboard(bitboardWhite, bitboardBlack);

    cnt = popCount(bitboardBlack);
    cout << "BLACK: "<<cnt << endl;
    cnt = popCount(bitboardWhite);
    cout << "WHITE: "<<cnt << endl;

    setBit(bitboardBlack, a5);
    setBit(bitboardBlack, h8);

    printBitboard(bitboardWhite, bitboardBlack);

    cnt = popCount(bitboardBlack);
    cout << "BLACK: "<<cnt << endl;
    cnt = popCount(bitboardWhite);
    cout << "WHITE: "<<cnt << endl;

    bitboardWhite=northShift(bitboardWhite);
    printBitboard(bitboardWhite, bitboardBlack);

    return 0;


}