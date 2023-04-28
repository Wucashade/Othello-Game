#include "Board.hpp"
#include "Window.hpp"

#define GET_BIT(bb, square) (bb & (1ULL << square))
#define SET_BIT(bb, square) (bb |= (1ULL << square))


#define CORNER_MASK 0x8100000000000081ULL



Board::Board()
{
    bool moveMade;
    U64 bitboardWhite = 0ULL;
    U64 bitboardBlack = 0ULL;

};

Board::~Board()
{

};

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

enum state
{
    WHITE_MOVE,
    BLACK_MOVE,
    GAME_OVER
};

state s;


void Board::init()
{
	
	SET_BIT(bitboardBlack, d5);
	SET_BIT(bitboardBlack, e4);
	SET_BIT(bitboardWhite, d4);
	SET_BIT(bitboardWhite, e5);
	outlineColour = BLACK;
	boxColour = BOARD_COLOUR;

    s = BLACK_MOVE;

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

void Board::render(U64 bbOne, U64 bbTwo) 
{
	renderBoard();
	renderDisks(bbOne, bbTwo);

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

	for (int i = 0; i < BOARD_BOXES_X; i++) 
		{
			for (int j = 0; j < BOARD_BOXES_Y; j++) 
			{
				int square = i * 8 + j;
				if(GET_BIT(bbOne, square))
				{
					drawImage("disctextures/blackdisc.png", i, j);
				}
				else if (GET_BIT(bbTwo, square))
				{
					drawImage("disctextures/whitedisc.png", i, j);
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
    if (dir8 < 0 || dir8 > 7) // Check to make sure that the direction exists in the array§
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

bool Board::isValid(U64 bbOwn, U64 bbOpponent, int index)
{
    U64 checkedBit = 1ULL << index;
    return (generateMoves(bbOwn, bbOpponent) & checkedBit) != 0;
}

void  Board::commitMove(U64 *bbOwn, U64 *bbOpponent, int index)
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

bool Board::handleMouseButtonDown(SDL_MouseButtonEvent& b, Player* playerOne, Player* playerTwo)
{
    if(b.button == SDL_BUTTON_LEFT)
    {
        int x, y, boardX, boardY;
	    SDL_GetMouseState(&x, &y);

        boardX = (x - Game::boardTopLeftX) / boxWidth;
        boardY = (y - Game::boardTopLeftY) / boxHeight;
        int index = boardX * 8 + boardY;
        cout << "board clicked at X: " << boardX << " Y: " << boardY<<endl;
        if(isValid(playerOne->pieceColour, playerTwo->pieceColour, index) == 1)
        {
            commitMove(&playerOne->pieceColour, &playerTwo->pieceColour, index);
            return 1;
        }
        else
        {
            return 0;
        }  
    }
    else
    {
        return 0;
    }
}
void Board::placeDisk(Player* playerOne, Player* playerTwo, int index)
{
    if(s == BLACK_MOVE && isValid(playerOne->pieceColour, 
                        playerTwo->pieceColour, index) != 0)
    {

        commitMove(&playerOne->pieceColour, &playerTwo->pieceColour, index);
        s = WHITE_MOVE;

    }
    if (generateMoves(playerOne->pieceColour, playerTwo->pieceColour) == 0 && 
        generateMoves(playerTwo->pieceColour, playerOne->pieceColour) == 0)
    {
        s = GAME_OVER;
        cout << "GAME OVER" << endl;
        if (popCount(playerOne->pieceColour) > popCount(playerTwo->pieceColour))
        {
            cout << "BLACK WINS!" << endl;
        }
        else if (popCount(playerTwo->pieceColour) > popCount(playerOne->pieceColour))
        {
            cout << "WHITE WINS!" << endl;
        }
    }
    else if (s == BLACK_MOVE && generateMoves(playerOne->pieceColour, playerTwo->pieceColour) == 0)
    {
        s = WHITE_MOVE;
    }
    else if (s == WHITE_MOVE && generateMoves(playerTwo->pieceColour, playerOne->pieceColour) == 0)
    {
        s = BLACK_MOVE;
    }
    if(s == WHITE_MOVE && isValid(playerTwo->pieceColour, 
                        playerOne->pieceColour, index) != 0)
    {

        commitMove(&playerTwo->pieceColour, &playerOne->pieceColour, index);
        s = BLACK_MOVE;
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
    for (int i = 0; i <8 ; i++)
    {
        cout << i+1 << "  "; // Prints the rank
        for (int j = 0; j < 8; j++)
        {
            int square = i * 8 + j;
            cout << (GET_BIT(bb, square) ? 1 : 0) << " "; // Prints either a 1 or 0 for square
        }
        cout << "" << endl;
    }
    cout << endl
         << "   a b c d e f g h" << endl; // Prints the file
}

int Board::evaluateMove(U64 bbOwn, U64 bbOpponent, U64 ownMoves, U64 oppMoves)
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

int Board::searchMove(U64 bbOwn, U64 bbOpponent, int maxDepth, int alpha, int beta,
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

int Board::iterativeSearchMove(U64 &bbOwn, U64 &bbOpponent,
                               int startDepth, int evalBudget)
{
    int depth, bestMove, evalCount, s;

    evalCount = 0;
    bestMove = -1;
    for (depth = startDepth; evalCount < evalBudget; depth++)
    {
        s = searchMove(bbOwn, bbOpponent, depth, -INT_MAX, INT_MAX, &bestMove, &evalCount);
        if (s >= (1 << 20) || -s >= (1 << 20))
        {
            break;
        }
    }

    return bestMove;
}

void Board::computeMove(U64 &bbOwn, U64 &bbOpponent, int *row, int *col)
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
