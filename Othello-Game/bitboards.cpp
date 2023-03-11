typedef unsigned long long  U64;
#include<iostream>

using namespace std;


void printBitboard(U64 bitboard)
{
    for (int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            int square = i * 8 + j;
            
            cout << ((bitboard & (1ULL << square)) ? 1:0 )<< " ";
        }
        cout << "\n" << endl;
    }
}

int main()
{

    U64 bitboard = 0ULL;
    printBitboard(bitboard);

    return 0;


}