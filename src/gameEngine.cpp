#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include "common.h"
#include "gameEngine.h"
#include "player.h"

using namespace std;

string GameEngine::execute(string command)
{
    if (writeTo(command) == 0)
        return readFrom();
}

int GameEngine::game(Player* p1, Player* p2)
{
    p1->genMove(this, 0);
    p2->genMove(this, 1);
}

