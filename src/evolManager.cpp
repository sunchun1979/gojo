#include <cstdio>
#include <iostream>
#include <ctime>

#include "common.h"
#include "gameEngine.h"
#include "player.h"

using namespace std;

int main(int argc, char* argv[])
{
    //srand(time(0));
    srand(0);
    GameEngine ge1(9);
    Player p1, p2;
    ge1.game(&p1,&p2);
    cout << ge1.execute("showboard") << endl;
    cout.flush();
}
