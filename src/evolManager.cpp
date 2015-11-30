#include <cstdio>
#include <iostream>

#include "common.h"
#include "gameEngine.h"
#include "player.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout << "evolManger" << endl;
    GameEngine ge1(9);
    Player p1, p2;
    ge1.game(&p1,&p2);
    cout << ge1.execute("showboard") << endl;
}
