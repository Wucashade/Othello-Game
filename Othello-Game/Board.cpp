#include "Board.hpp"
#include "Game.hpp"
#include "Window.hpp"



#define GET_BIT(bb, square) (bb & (1ULL << square))
#define SET_BIT(bb, square) (bb |= (1ULL << square))

U64 bitboardWhite = 0ULL;
U64 bitboardBlack = 0ULL;

enum squares
{
    a1, a2, a3, a4, a5, a6, a7, a8,
  	b1, b2, b3, b4, b5, b6, b7, b8,
  	c1, c2, c3, c4, c5, c6, c7, c8,
  	d1, d2, d3, d4, d5, d6, d7, d8,
  	e1, e2, e3, e4, e5, e6, e7, e8,
  	f1, f2, f3, f4, f5, f6, f7, f8,
  	g1, g2, g3, g4, g5, g6, g7, g8,
  	h1, h2, h3, h4, h5, h6, h7, h8
};

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



Board::Board()
{

};

Board::~Board()
{

};

void Board::init()
{
	SET_BIT(bitboardWhite, d4);
	SET_BIT(bitboardWhite, e5);
	SET_BIT(bitboardBlack, d5);
	SET_BIT(bitboardBlack, e4);

	outlineColour = BLACK;
	boxColour = BOARD_COLOUR;

	boxWidth = Window::windowWidth / BOARD_BOXES_X;
	boxHeight = Window::windowHeight / BOARD_BOXES_Y;
}

void Board::renderBoard() 
{

	SDL_Rect boardBuild;
	boardBuild.w = boxWidth;
	boardBuild.h = boxHeight;

	for (int i = 0; i < BOARD_BOXES_X; i++) 
	{
		for (int j = 0; j < BOARD_BOXES_Y; j++) 
		{
			boardBuild.x = Game::boardTopLeftX + i * boxWidth;
			boardBuild.y = Game::boardTopLeftY + j * boxHeight;
			SDL_SetRenderDrawColor(Window::renderer, boxColour.r, boxColour.g, boxColour.b, boxColour.a);
			SDL_RenderFillRect(Window::renderer, &boardBuild);

			
			SDL_SetRenderDrawColor(Window::renderer, outlineColour.r, outlineColour.g, outlineColour.b, outlineColour.a);
			SDL_RenderDrawRect(Window::renderer, &boardBuild);
		
		}
	}
}

void Board::render() 
{
	renderBoard();
	renderDisks(bitboardWhite, bitboardBlack);

}

void Board::update() 
{

}

void Board::resize() 
{
	boxWidth = Window::windowWidth / BOARD_BOXES_X;
	boxHeight = Window::windowHeight / BOARD_BOXES_Y;

	if (boxWidth > boxHeight) {

		boxWidth = boxHeight;
	}
	else 
	{
		boxHeight = boxWidth;
	}

}

int Board::getWidth() 
{
	return BOARD_BOXES_X * boxWidth;
}

int Board::getHeight() 
{
	return BOARD_BOXES_Y * boxHeight;
}


void Board::renderDisks(U64 bbOne, U64 bbTwo) 
{

	for (int i = 0; i < BOARD_BOXES_X; ++i) 
		{
			for (int j = 0; j < BOARD_BOXES_Y; ++j) 
			{
				int square = i * 8 + j;
				if(GET_BIT(bbOne, square))
				{
					drawImage("disctextures/whitedisc.png", i, j);
				}
				else if (GET_BIT(bbTwo, square))
				{
					drawImage("disctextures/blackdisc.png", i, j);
				}
				
			}
		}
}

void Board::drawImage(const string& imagePath, int x, int y)
{
	int w,h;
	SDL_Surface* surface;
	SDL_Rect rectOne, rectTwo;

	surface = IMG_Load(imagePath.c_str());
	if(!surface)
	{
		cout << SDL_GetError() << endl;
	}
	SDL_Texture* disk = SDL_CreateTextureFromSurface(Window::renderer, surface);
	if(!disk)
	{
		cout << SDL_GetError() << endl;
	}
	SDL_QueryTexture(disk, NULL, NULL, &w, &h);
	rectOne.w = w;
	rectOne.h = h;
	rectOne.x = rectOne.y = 0;

	rectTwo.x = Game::boardTopLeftX + x * boxWidth;
	rectTwo.y = Game::boardTopLeftY + y * boxHeight;
	rectTwo.w = boxWidth;
	rectTwo.h = boxHeight;


	SDL_RenderCopy(Window::renderer, disk, &rectOne, &rectTwo);
	SDL_FreeSurface(surface);
}

U64 Board::shiftOne(U64 bb, int dir8)
{
    if (dir8 < 0 || dir8 > 7) // Check to make sure that the direction exists in the array
    {
        cout << "Invalid direction" << endl;
    }
    int a = shift[dir8];
    return (a > 0) ? ((bb << a) & avoidWrap[dir8]) : ((bb >> -a) & avoidWrap[dir8]);
    // Shifts left if the value is positive and right if negative, also applies mask so that the bits don't wrap around to the other side of the bitboard if theyre at the end
}

int Board::popCount(U64 bb)
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

U64 Board::generateMoves(U64 bbOwn, U64 bbOpponent)
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

bool Board::isValid(U64 bb, int index)
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

void  Board::commitMove(U64 *bbOwn, U64 *bbOpponent, int index)
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

void Board::handleMouseButtonDown(SDL_MouseButtonEvent& b, U64 bbOwn, U64 bbOpponent)
{
	int x, y, boardX, boardY;
	SDL_GetMouseState(&x, &y);
	cout << "hi" << endl;
	printSingleBitboard(bbOwn);
	printSingleBitboard(bbOpponent);


	boardX = (x - Game::boardTopLeftX) / boxWidth;
	boardY = (y - Game::boardTopLeftY) / boxHeight;
	int index = boardX * 8 + boardY;

	if(isValid(bbOwn, index))
	{
		commitMove(&bbOwn, &bbOpponent, index);
	}

}

void Board::printBitboard(U64 bbOne, U64 bbTwo)
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

void Board::printSingleBitboard(U64 bb)
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
