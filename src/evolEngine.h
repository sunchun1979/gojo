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
        static const int MAX_ROUND = 100;
        GameEngine* m_gameEngine = NULL;
        vector<Player*> m_candidate;
        int m_roundTrack;

    public:
        EvolConfiguration m_config;
        string m_fname_prefix = "nn";

    public:
        evolEngine();
        ~evolEngine();

        int compare(Player* p1, Player* p2);
        int compare(int p1, int p2);
        void finalize();
        void initialize();
        void restart();
        void elimination();
        void addPlayer();
        void saveTop();
        void Run();

    private:
        void binaryElimination(int begin, int end, int& top, int& bot);
};

#endif
