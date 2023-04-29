
#include "AIOne.hpp"
#include "AITwo.hpp"

class Bitboards
{


    public:

    Bitboards();

    int init();
    void game();

    AIOne* aiOne;
    AITwo* aiTwo;

};