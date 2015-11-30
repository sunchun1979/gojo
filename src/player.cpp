#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "common.h"
#include "player.h"
#include "gameEngine.h"

using namespace std;

string Player::genMove(GameEngine* engine, int c)
{
    string candidate = engine->execute("all_legal " + COLOR[c]);
    cout << "[" << candidate << "]" << endl;
    return "";
}


