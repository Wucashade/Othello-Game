typedef unsigned long long  U64;
#define getBit(bitboard, square) (bitboard & (1ULL << square))
#define setBit(bitboard, square) (bitboard |= (1ULL << square))


#include<iostream>

using namespace std;

enum {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8

};


void printBitboard(U64 bitboard)
{
    for (int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            int square = i * 8 + j;
            
            cout << ( getBit(bitboard, square) ? 1:0 )<< " ";
        }
        cout << "\n" << endl;
    }
}

int main()
{

    U64 bitboard = 0ULL;

    setBit(bitboard, e4);
    setBit(bitboard, e2);
    setBit(bitboard, e1);
    printBitboard(bitboard);

    return 0;


}