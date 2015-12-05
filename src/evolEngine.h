#ifndef _EVOLENGINE
#define _EVOLENGINE

#include <cstdio>
#include <iostream>

#include "common.h"
#include "player.h"
#include "gameEngine.h"

struct EvolConfiguration
{
    int PoolSize;
    int NumRestart;
    int CompareRounds;
};

class evolEngine
{
    private:
        GameEngine* m_gameEngine = NULL;
        string m_file_currentTop;
        vector<Player*> m_candidate;

    public:
        EvolConfiguration m_config;

    public:
        evolEngine();
        ~evolEngine();

        int compare(Player* p1, Player* p2);
        void finalize();
        void initialize();
        void elimination();
};

#endif
