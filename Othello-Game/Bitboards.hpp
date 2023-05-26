
#include "AIOne.hpp"
#include "AITwo.hpp"

class Bitboards
{


    public:

    Bitboards();

    int init();
    void game();
    void game2();
    void checkGameOver();
    void checkGameOver2();

    AIOne* aiOne;
    AITwo* aiTwo;

    // AIOne* aiTwo;
    // AITwo* aiOne;

};