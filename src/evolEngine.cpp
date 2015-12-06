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
    m_config.PoolSize = 16; // should be 2^n
    m_config.NumRestart = 3000;
    m_config.CompareRounds = 11;
    m_roundTrack = 0;
}

evolEngine::~evolEngine()
{
    finalize();
    if (m_gameEngine != NULL) delete m_gameEngine;
}

void evolEngine::Run()
{
    for(m_roundTrack = 0; m_roundTrack < 1 /*MAX_ROUND;*/; ++m_roundTrack)
    {
        restart();
        for(int current_gen = 0; current_gen < m_config.NumRestart; ++ current_gen)
        {
            cout << "Round " << m_roundTrack << " Generation " << current_gen << "  ";
            cout.flush();
            elimination();
            addPlayer();
            saveTop();
        }
    }
}

void evolEngine::restart()
{
    finalize();
    initialize();
}

void evolEngine::saveTop()
{
    string fname_currentTop = m_fname_prefix + "_" + to_string(getpid()) + ".bin";
    m_candidate[0]->saveTo(fname_currentTop);
}

int evolEngine::compare(Player* p1, Player* p2)
{
    int p1wins = 0;
    for (int i=0;i<m_config.CompareRounds;++i)
    {
        if (m_gameEngine->game(p1,p2)) p1wins++;
    }
    return (2*p1wins > m_config.CompareRounds);
}

int evolEngine::compare(int p1, int p2)
{
    //cout << "[" << p1 << ", " << p2 << "]: ";
    int result = compare(m_candidate[p1], m_candidate[p2]);
    //if (result) cout << p1;
    //else cout << p2;
    //cout << endl;
    return result;
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
    m_candidate.resize(m_config.PoolSize);
    for (int i=0; i<m_config.PoolSize; ++i)
    {
        m_candidate[i] = new Player();
    }
}

void evolEngine::elimination()
{
    int top, bot;
    binaryElimination(0, m_config.PoolSize-1, top, bot);
    cout << "final top " << top << " bot " << bot << endl;
    std::swap(m_candidate[0], m_candidate[top]);
    std::swap(m_candidate[m_config.PoolSize-1], m_candidate[bot]);
}

void evolEngine::binaryElimination(int begin, int end, int& top, int& bot)
{
    if (begin + 1 == end)
    {
        if (compare(begin, end))
        {
            top = begin; bot = end;
        }else
        {
            bot = begin; top = end;
        }
        return;
    }
    int mid = (begin + end + 1) / 2 - 1;
    int lefttop, leftbot, righttop, rightbot;
    binaryElimination(begin, mid, lefttop, leftbot);
    binaryElimination(mid + 1, end, righttop, rightbot);
    if (compare(lefttop, righttop))
        top = lefttop;
    else
        top = righttop;
    if (compare(leftbot, rightbot))
        bot = rightbot;
    else
        bot = leftbot;
    return;
}

void evolEngine::addPlayer()
{
    delete m_candidate[m_config.PoolSize-1];

    vector<Player*> candidate(&m_candidate[0], &m_candidate[m_config.PoolSize-2]);
    vector<float> weights(m_config.PoolSize-1);
    weights[0] = 0.5;
    for(int i=1; i<m_config.PoolSize-1; ++i)
        weights[i] = 0.5/(m_config.PoolSize-1);

    m_candidate[m_config.PoolSize-1] = new Player(candidate, weights);
}
