#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include "common.h"
#include "player.h"
#include "gameEngine.h"

using namespace std;

string Player::genMove(GameEngine* engine, int c)
{
    string candidate = engine->execute("all_legal " + COLOR[c]);
    vector<pair<int,int>> v = parseFrom(candidate);
    return POS2STR(v[rand()%v.size()]);
}

vector<pair<int,int>> Player::parseFrom(string s)
{
    int N = (s.length()-3)/3;
    vector<pair<int,int>> v(N);
    for (int i = 0; i < N; ++i)
    {
        v[i] = STR2POS(s[2+3*i],s[3+3*i]);
    }
    return v;
}
