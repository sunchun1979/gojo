#ifndef _PLAYER
#define _PLAYER

#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include "common.h"

using namespace std;

class GameEngine;

class Player
{
    public:
        Player()
        {
        }

        ~Player()
        {
        }

        string genMove(GameEngine* engine, int c);

    private:
        vector<pair<int,int>> parseFrom(string s);
};
#endif
