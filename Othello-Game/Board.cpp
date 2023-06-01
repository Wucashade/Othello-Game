#include "Board.hpp"
#include "Window.hpp"
#include "Game.hpp"



#define GET_BIT(bb, square) (bb & (1ULL << square))
#define SET_BIT(bb, square) (bb |= (1ULL << square))


Board::Board()
{
    bool moveMade;
    U64 bitboardWhite;
    U64 bitboardBlack;
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

const U64 avoidWrap[8] =
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

const int shift[8] = {9, 1, -7, -8, -9, -1, 7, 8};

const U64 squareDefinitions[8] =
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

static int squareEarlyValues[8] = {30, -6, -15, -1, 5, 5, -1, -3};
static int squareMidValues[8] = {50, -5, -20, 6, 10, 2, 1, -4};
static int squareEndValues[8] = {100, -12, -25, 5, 10, 5, 2, 1};


void Board::init()
{

	outlineColour = BLACK;
	boxColour = BOARD_COLOUR;

	boxWidth = (Window::windowWidth) / BOARD_BOXES_X+2;
	boxHeight = (Window::windowHeight) / BOARD_BOXES_Y+2;

}

void Board::renderBoard() 
{


	SDL_Rect boardBuild;
	boardBuild.w = boxWidth;
	boardBuild.h = boxHeight;

	for (int i = 3; i < BOARD_BOXES_X; i++) 
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

void Board::renderMenu(Player* playerOne, Player* playerTwo, string currentTurn)
{

    SDL_Rect statsBuild;
    SDL_Surface* surface;
    SDL_Rect blackScore;
    SDL_Rect whiteScore;
    SDL_Rect homeButton;
    SDL_Rect whosTurn;
    SDL_Texture* words;
    SDL_Color black = {0,0,0};
    SDL_Color white = {255,255,255};

    TTF_Font* globalFont = TTF_OpenFont("fonts/oswald/Oswald-Bold.ttf", 24);

    //CREATING TEXT FOR SCORES

    string blackScoreText = to_string(popCount(playerOne->getBitboard()));
    string blackText = "BLACK: " + blackScoreText;
    string whiteScoreText = to_string(popCount(playerTwo->getBitboard()));
    string whiteText = "WHITE: " + whiteScoreText;

     //DEFINE AREA FOR SIDE MENU

    statsBuild.w = boxWidth*3;
    statsBuild.h = boxHeight*8;
    statsBuild.x = Game::boardTopLeftX;
    statsBuild.y = Game::boardTopLeftY;

    //DRAW SIDE MENU

    SDL_SetRenderDrawColor(Window::renderer, 169,169,169, 0);
	SDL_RenderFillRect(Window::renderer, &statsBuild);

    //DEFINE AREA FOR HOME BUTTON

    homeButton.w = boxWidth*3;
    homeButton.h = boxHeight;
    homeButton.x = Game::boardTopLeftX;
    homeButton.y = Game::boardTopLeftY;

    //DEFINE AREA FOR BLACK SCORE

    blackScore.w = boxWidth*2.5;
    blackScore.h = boxHeight;
    blackScore.x = Game::boardTopLeftX + 0.2 * boxWidth;
    blackScore.y = Game::boardTopLeftY + 2 * boxHeight;

    //DEFINE AREA FOR WHITE SCORE

    whiteScore.w = boxWidth*2.5;
    whiteScore.h = boxHeight;
    whiteScore.x = Game::boardTopLeftX + 0.2 * boxWidth;
    whiteScore.y = Game::boardTopLeftY + 4 * boxWidth;

    //DEFINE AREA FOR WHO'S TURN

    whosTurn.w = boxWidth*2.5;
    whosTurn.h = boxHeight;
    whosTurn.x = Game::boardTopLeftX + 0.2 * boxWidth;
    whosTurn.y = Game::boardTopLeftY + 6 * boxWidth;

    //DISPLAY HOME BUTTON

    surface = TTF_RenderText_Solid(globalFont, "HOME", black);
    words = SDL_CreateTextureFromSurface(Window::renderer, surface);
    SDL_SetRenderDrawColor(Window::renderer, outlineColour.r, outlineColour.g, outlineColour.b, outlineColour.a);
    SDL_RenderDrawRect(Window::renderer, &homeButton);
    SDL_RenderCopy(Window::renderer, words, NULL, &homeButton);
    SDL_FreeSurface(surface);

    //DISPLAY BLACK SCORE

    surface = TTF_RenderText_Solid(globalFont, blackText.c_str(), black);
    words = SDL_CreateTextureFromSurface(Window::renderer, surface);
    SDL_RenderCopy(Window::renderer, words, NULL, &blackScore);
    SDL_FreeSurface(surface);
    

    //DISPLAY WHITE SCORE

    surface = TTF_RenderText_Solid(globalFont, whiteText.c_str(), white);
    words = SDL_CreateTextureFromSurface(Window::renderer, surface);
    SDL_RenderCopy(Window::renderer, words, NULL, &whiteScore);
    SDL_FreeSurface(surface);

    //DISPLAY WHO'S TURN
    if(currentTurn == "BLACK" && playerOne->isPlayerHuman() == 1)
    {
        surface = TTF_RenderText_Solid_Wrapped(globalFont, "BLACK PLAYER MOVE", black, boxWidth*2.5);
    }
    else if(currentTurn == "WHITE"  && playerTwo->isPlayerHuman() == 1)
    {
        surface = TTF_RenderText_Solid_Wrapped(globalFont, "WHITE PLAYER MOVE", white, boxWidth*2.5);
    }
    else
    {
        surface = TTF_RenderText_Solid_Wrapped(globalFont, "COMPUTER MAKING MOVE ...", white, boxWidth*2.5);
    }
    
    words = SDL_CreateTextureFromSurface(Window::renderer, surface);
    SDL_RenderCopy(Window::renderer, words, NULL, &whosTurn);
    SDL_FreeSurface(surface);

    //FREEING THE MEMORY

    TTF_CloseFont( globalFont );
    globalFont = NULL;
    SDL_DestroyTexture(words);

}

void Board::render(Player* playerOne, Player* playerTwo, string currentTurn) 
{
    
    // Render the board, menu and discs
	renderBoard();
    renderMenu(playerOne, playerTwo, currentTurn);
	renderDisks(playerOne->getBitboard(), playerTwo->getBitboard());

}

void Board::update() 
{

}

void Board::resize() 
{

    // resizes the squares based on window width and height
	boxWidth = Window::windowWidth / BOARD_BOXES_X;
	boxHeight = Window::windowHeight / BOARD_BOXES_Y;


    // Makes sure that the squares will always stay as squares
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

    // Iterates across the board to draw the discs
	for (int i = 0; i < BOARD_BOXES_X-3; i++) 
		{
			for (int j = 0; j < BOARD_BOXES_Y; j++) 
			{
				int square = i * 8 + j;
                //Checks if bit in square is black or white
				if(GET_BIT(bbOne, square))
				{
					drawImage("disctextures/blackdisc.png", i+3, j);
				}
				else if (GET_BIT(bbTwo, square))
				{
					drawImage("disctextures/whitedisc.png", i+3, j);
				}
			}
		}
       
}

void Board::drawImage(const string& imagePath, int x, int y)
{
	int w,h;
	SDL_Surface* surface;
	SDL_Rect rectOne, rectTwo;

    //Gets the image connected to the colour
	surface = IMG_Load(imagePath.c_str());
	if(!surface)
	{
		cout << SDL_GetError() << endl;
	}
	SDL_Texture* disc = SDL_CreateTextureFromSurface(Window::renderer, surface);
	if(!disc)
	{
		cout << SDL_GetError() << endl;
	}
	SDL_QueryTexture(disc, NULL, NULL, &w, &h);
	rectOne.w = w;
	rectOne.h = h;
	rectOne.x = rectOne.y = 0;

	rectTwo.x = Game::boardTopLeftX + x * boxWidth;
	rectTwo.y = Game::boardTopLeftY + y * boxHeight;
	rectTwo.w = boxWidth;
	rectTwo.h = boxHeight;

    //free up resources
	SDL_RenderCopy(Window::renderer, disc, &rectOne, &rectTwo);
	SDL_FreeSurface(surface);
    SDL_DestroyTexture(disc);
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
    U64 flood;
    U64 legalMoves = 0;

    //Iterates for each direction
    for (int i = 0; i < 8; i++)
    {
        //Checks squares across the entire length of board
        flood = shiftOne(bbOwn, i) & bbOpponent;

        flood |= shiftOne(flood, i) & bbOpponent;
        flood |= shiftOne(flood, i) & bbOpponent;
        flood |= shiftOne(flood, i) & bbOpponent;
        flood |= shiftOne(flood, i) & bbOpponent;
        flood |= shiftOne(flood, i) & bbOpponent;

        legalMoves |= shiftOne(flood, i) & emptyBB;
    }

    return legalMoves;
}

bool Board::isValid(U64 bbOwn, U64 bbOpponent, int index)
{
    // Checks bit if its a legal move
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
            //Utilising Dumb7Fill, the same as generating moves

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
        // Get the mouse coordinates from events handled

        boardX = ((x - Game::boardTopLeftX) / boxWidth)-3;
        boardY = (y - Game::boardTopLeftY) / boxHeight;
        int index = boardX * 8 + boardY;
        cout << "board clicked at X: " << boardX << " Y: " << boardY<<endl;
        if(isValid(playerOne->pieceColour, playerTwo->pieceColour, index) == 1)
        {
            commitMove(&playerOne->pieceColour, &playerTwo->pieceColour, index);
            return 1;   //Returns to verify that player made move
        }
        else
        {
            return 0;   // Returns that player hasn't made a move yet
        }  
    }
    else
    {
        return 0;
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

int Board::frontierDiscs(U64 bbOwn, U64 bbOpponent)
{
        U64 bbEmpty = ~(bbOwn | bbOpponent);
        U64 testFront;
        int value;

        U64 ownFront = 0;
        U64 oppFront = 0;

        //Checks whether there are empty discs in all 8 directions
        for (int i = 0; i < 8; i++) {
                testFront = shiftOne(bbEmpty, i);
                ownFront |= testFront & bbOwn;
                oppFront |= testFront & bbOpponent;
        }

        value = popCount(ownFront) - popCount(oppFront);
        return value;
}

int Board::discSquareValue(U64 bbOwn, U64 bbOpponent)
{

    int value;
    // Check turn number
    int discsPlaced = popCount(bbOwn)+popCount(bbOpponent);

    for(int i = 0; i<(sizeof(squareDefinitions)/sizeof(squareDefinitions[0])); i++)
    {
        U64 ownCheckedSquares = bbOwn &  squareDefinitions[i];
        U64 oppCheckedSquares = bbOpponent &  squareDefinitions[i];
        if(discsPlaced < 25)    //GET VALUES FOR EARLY GAME
        {
            value += popCount(ownCheckedSquares) * squareEarlyValues[i];
            value -= popCount(oppCheckedSquares) * squareEarlyValues[i];
        }
        else if(discsPlaced < 45 && discsPlaced > 24)   //GET VALUES FOR MID GAME
        {
            value += popCount(ownCheckedSquares) * squareMidValues[i];
            value -= popCount(oppCheckedSquares) * squareMidValues[i];
        }
        else    //GET VALUES FOR END GAME
        {
            value += popCount(ownCheckedSquares) * squareEndValues[i];
            value -= popCount(oppCheckedSquares) * squareEndValues[i];
        }
    }

    return value;
}

int Board::evaluateMove(U64 bbOwn, U64 bbOpponent, U64 ownMoves, U64 oppMoves)
{

    int discsPlaced;
    int value = 0;
    U64 ownCheckedSquares, oppCheckedSquares;

    if (!ownMoves && !oppMoves)
    {
        //Chooses based on disc difference if no moves found
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

int Board::searchMove(U64 bbOwn, U64 bbOpponent, int maxDepth, int alpha, int beta,
               int *bestMove, int *evaluationCount)
{

    U64 ownNewDisks, oppNewDisks;
    U64 ownMoves = generateMoves(bbOwn, bbOpponent);
    U64 oppMoves = generateMoves(bbOpponent, bbOwn);

    if (!ownMoves && oppMoves)
    {
        //Change sides if main player doesn't have moves
        return -searchMove(bbOpponent, bbOwn, maxDepth, -beta, -alpha,
                           bestMove, evaluationCount);
    }

    if (maxDepth == 0 || (!ownMoves && !oppMoves))  //Evaluate position if depth reached or no moves left
    {
        ++*evaluationCount;
        return evaluateMove(bbOwn, bbOpponent, ownMoves, oppMoves);
    }

    int best = -INT_MAX;
    //Checks the board
    for (int i = 0; i < 64; i++)
    {
        if (!(ownMoves & (1ULL << i)))
        {
            continue;
        }

        ownNewDisks = bbOwn;
        oppNewDisks = bbOpponent;
        commitMove(&ownNewDisks, &oppNewDisks, i);  

        int a = -searchMove(oppNewDisks, ownNewDisks, maxDepth - 1, -beta, -alpha,
                            NULL, evaluationCount);

        if (a > best)   //Alpha beta pruning
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

int Board::iterativeSearchMove(U64 &bbOwn, U64 &bbOpponent, int startDepth, int evaluationBudget)
{
    int depth, bestMove, evaluationCount, s;

    evaluationCount = 0;
    bestMove = -1;
    for (depth = startDepth; evaluationCount < evaluationBudget; depth++)
    {
        s = searchMove(bbOwn, bbOpponent, depth, -INT_MAX, INT_MAX, &bestMove, &evaluationCount);
        if (s >= (1 << 20) || -s >= (1 << 20))
        {
            break;
        }
    }

    return bestMove;
}

void Board::computeMove(U64 &bbOwn, U64 &bbOpponent, int *row, int *col)
{
    int moveIndex;
    int startDepth;

    if((popCount(bbOwn) + popCount(bbOpponent)) > 45)
    {
        startDepth = 12;    //Search depth for end game
    }
    else
    {
        startDepth = 8; // Search depth before end game
    }
    static const int evaluationBudget = 500000; //Number of evaluations allowed

    moveIndex = iterativeSearchMove(bbOwn, bbOpponent, startDepth, evaluationBudget);


    *row = moveIndex / 8;
    *col = moveIndex % 8;
}
