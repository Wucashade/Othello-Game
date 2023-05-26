
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
#define C_SQUARES_MASK 0x4281000000008142ULL
#define X_SQUARES_MASK 0x42000000004200ULL
#define B_SQUARES_MASK 0x1800008181000018ULL
#define A_SQUARES_MASK 0x2400810000810024ULL
#define S_SQUARES_MASK 0x240000240000ULL
#define INNER_MIDDLE_MASK 0x183C3C180000ULL
#define OUTER_MIDDLE_MASK 0x3C424242423C00ULL
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

state s = BLACK_MOVE;

Bitboards::Bitboards(){};



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

void Bitboards::game()
{

    int row = 0, col = 0;
    int cnt = 0;

    bitboardWhite = 0ULL;
    bitboardBlack = 0ULL;

    SET_BIT(bitboardWhite, d4);
    SET_BIT(bitboardWhite, e5);
    SET_BIT(bitboardBlack, e4);
    SET_BIT(bitboardBlack, d5);
    
    s = BLACK_MOVE;
    

    clock_t tStart = clock();

    while (!(s == GAME_OVER))
    {

        

        if (s == BLACK_MOVE && aiOne->generateMoves(bitboardBlack, bitboardWhite) == 0)
        {
            s = WHITE_MOVE;
        }
        else if (s == WHITE_MOVE && aiTwo->generateMoves(bitboardWhite, bitboardBlack) == 0)
        {
            s = BLACK_MOVE;
        }

        else if (s == BLACK_MOVE)
        {

           
            /*
            while (!(isValid(bitboardBlack, (row * 8 + col))))
            {
                cout << "Enter row: ";
                cin >> row;
                cout << "Enter col: ";
                cin >> col;
            }
            */
        //    cout << "BLACK MOVE" << endl;
        //    cout << "First row: " << row << " First col: " << col << endl;
            aiOne->computeMove(aiOne, bitboardBlack, bitboardWhite, &row, &col);
            aiOne->commitMove(aiOne, &bitboardBlack, &bitboardWhite, (row * 8 + col));
            // cout << "Second row: " << row << " Second col: " << col << endl;
            // cout << "Time: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
            //printBitboard(bitboardWhite, bitboardBlack);

            // cnt = popCount(bitboardBlack);
            // cout << "BLACK: " << cnt << endl;
            // cnt = popCount(bitboardWhite);
            // cout << "WHITE: " << cnt << endl;
            s = WHITE_MOVE;

            
        }
        else if (s == WHITE_MOVE)
        {

            // cout << "WHITE MOVE" << endl;
            aiTwo->computeMove(aiTwo, bitboardWhite, bitboardBlack, &row, &col);
            aiTwo->commitMove(aiTwo, &bitboardWhite, &bitboardBlack, (row * 8 + col));

            // cout << "Time: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
            //printBitboard(bitboardWhite, bitboardBlack);

            // cnt = popCount(bitboardBlack);
            // cout << "BLACK: " << cnt << endl;
            // cnt = popCount(bitboardWhite);
            // cout << "WHITE: " << cnt << endl;
            s = BLACK_MOVE;

            
        }
        checkGameOver();
        
        
        
    }
    cout << "Time: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
}

void Bitboards::game2()
{

    int row = 0, col = 0;
    int cnt = 0;

    bitboardWhite = 0ULL;
    bitboardBlack = 0ULL;
    
    s = BLACK_MOVE;

    SET_BIT(bitboardWhite, d4);
    SET_BIT(bitboardWhite, e5);
    SET_BIT(bitboardBlack, e4);
    SET_BIT(bitboardBlack, d5);
    

    

    clock_t tStart = clock();

    while (!(s == GAME_OVER))
    {

        if (s == BLACK_MOVE && aiTwo->generateMoves(bitboardBlack, bitboardWhite) == 0)
        {
            s = WHITE_MOVE;
        }
        else if (s == WHITE_MOVE && aiOne->generateMoves(bitboardWhite, bitboardBlack) == 0)
        {
            s = BLACK_MOVE;
        }

        else if (s == BLACK_MOVE)
        {

           
            /*
            while (!(isValid(bitboardBlack, (row * 8 + col))))
            {
                cout << "Enter row: ";
                cin >> row;
                cout << "Enter col: ";
                cin >> col;
            }
            */
        //    cout << "BLACK MOVE" << endl;
        //    cout << "First row: " << row << " First col: " << col << endl;
            aiTwo->computeMove(aiTwo, bitboardBlack, bitboardWhite, &row, &col);
            aiTwo->commitMove(aiTwo, &bitboardBlack, &bitboardWhite, (row * 8 + col));
            // cout << "Second row: " << row << " Second col: " << col << endl;
            // cout << "Time: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
            //printBitboard(bitboardWhite, bitboardBlack);

            // cnt = popCount(bitboardBlack);
            // cout << "BLACK: " << cnt << endl;
            // cnt = popCount(bitboardWhite);
            // cout << "WHITE: " << cnt << endl;
            s = WHITE_MOVE;

            
        }
        else if (s == WHITE_MOVE)
        {

            // cout << "WHITE MOVE" << endl;
            aiOne->computeMove(aiOne, bitboardWhite, bitboardBlack, &row, &col);
            aiOne->commitMove(aiOne, &bitboardWhite, &bitboardBlack, (row * 8 + col));

            // cout << "Time: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
            //printBitboard(bitboardWhite, bitboardBlack);

            // cnt = popCount(bitboardBlack);
            // cout << "BLACK: " << cnt << endl;
            // cnt = popCount(bitboardWhite);
            // cout << "WHITE: " << cnt << endl;
            s = BLACK_MOVE;
        }
        checkGameOver2();
        
        
        
    }
    cout << "Time: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
}

void Bitboards::checkGameOver()
{
    if (aiOne->generateMoves(bitboardBlack, bitboardWhite) == 0 && aiTwo->generateMoves(bitboardWhite, bitboardBlack) == 0)
    {
        s = GAME_OVER;
        cout << "GAME OVER" << endl;
        if (popCount(bitboardWhite) > popCount(bitboardBlack))
        {

            int cnt = 0;
            cnt = popCount(bitboardBlack);
            cout << "BLACK: " << cnt << endl;
            cnt = popCount(bitboardWhite);
            cout << "WHITE: " << cnt << endl;

            cout << "WHITE WINS!" << endl;
        }
        else if (popCount(bitboardBlack) > popCount(bitboardWhite))
        {

            int cnt = 0;
            cnt = popCount(bitboardBlack);
            cout << "BLACK: " << cnt << endl;
            cnt = popCount(bitboardWhite);
            cout << "WHITE: " << cnt << endl;

            cout << "BLACK WINS!" << endl;
        }
        else
        {
            int cnt = 0;
            cnt = popCount(bitboardBlack);
            cout << "BLACK: " << cnt << endl;
            cnt = popCount(bitboardWhite);
            cout << "WHITE: " << cnt << endl;

            cout << "DRAW!" << endl;
        }
    }
    
}

void Bitboards::checkGameOver2()
{
    if (aiTwo->generateMoves(bitboardBlack, bitboardWhite) == 0 && aiOne->generateMoves(bitboardWhite, bitboardBlack) == 0)
    {
        s = GAME_OVER;
        cout << "GAME OVER" << endl;
        if (popCount(bitboardWhite) > popCount(bitboardBlack))
        {
            int cnt = 0;
            cnt = popCount(bitboardBlack);
            cout << "BLACK: " << cnt << endl;
            cnt = popCount(bitboardWhite);
            cout << "WHITE: " << cnt << endl;

            cout << "WHITE WINS!" << endl;
        }
        else if (popCount(bitboardBlack) > popCount(bitboardWhite))
        {

            int cnt = 0;
            cnt = popCount(bitboardBlack);
            cout << "BLACK: " << cnt << endl;
            cnt = popCount(bitboardWhite);
            cout << "WHITE: " << cnt << endl;

            cout << "BLACK WINS!" << endl;
        }
        else
        {
            int cnt = 0;
            cnt = popCount(bitboardBlack);
            cout << "BLACK: " << cnt << endl;
            cnt = popCount(bitboardWhite);
            cout << "WHITE: " << cnt << endl;

            cout << "DRAW!" << endl;
        }
    }
    
    
}

int Bitboards::init()
{
    aiOne = new AIOne();
    aiTwo = new AITwo();

    // aiTwo = new AIOne();
    // aiOne = new AITwo();
    
    game();
    game2();

    return 0;
}