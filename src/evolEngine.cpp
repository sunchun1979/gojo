#include <cstdio>
#include <iostream>
#include <ctime>

#include "common.h"
#include "gameEngine.h"
#include "player.h"
#include "evolEngine.h"

using namespace std;

evolEngine::evolEngine()
{
    m_gameEngine = new GameEngine(9);
    m_config.PoolSize = 16;
    m_config.NumRestart = 3000;
    m_config.CompareRounds = 11;
    m_file_currentTop = "nn_" + to_string(getpid()) + ".bin";
}

evolEngine::~evolEngine()
{
    finalize();
    if (m_gameEngine == NULL) delete m_gameEngine;
}

int evolEngine::compare(Player* p1, Player* p2)
{
    int p1wins = 0;
    for (int i=0;i<m_config.CompareRounds;++i)
    {
        if (m_gameEngine->game(p1,p2)) p1wins++;
        cout << i << " " << p1wins << endl;
    }
    return (2*p1wins > m_config.CompareRounds);
}

void evolEngine::finalize()
{
    for (int i=0;i<m_candidate.size();++i)
        if (m_candidate[i]!=NULL)
        {
            delete m_candidate[i];
            m_candidate[i] = NULL;
        }
}

void evolEngine::initialize()
{
    for (int i=0; i<m_config.PoolSize; ++i)
    {
        m_candidate[i] = new Player();
    }
}

void elimination()
{
}
