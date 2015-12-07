#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <cfloat>
#include <algorithm>
#include <iomanip>
#include "common.h"
#include "player.h"
#include "gameEngine.h"

using namespace std;

Player::Player()
{
    initialize();
    int F = m_size*m_size*N_FEATURES;
    vector<vector<int>> shape(4);
    shape[0] = {F,F,9};
    shape[1] = {F,F,9};
    shape[2] = {F,F,9};
    shape[3] = {F,m_size*m_size,9};
    m_NN = new CNN(shape, m_size, N_FEATURES);
}

Player::Player(vector<Player*> candidates, vector<float> weights)
{
    initialize();
    int cSize = candidates.size();
    vector<CNN*> nnc(cSize);
    for(int i=0;i<cSize;++i)
        nnc[i] = candidates[i]->m_NN;
    m_NN = new CNN(nnc, weights);
}

string Player::genMove(GameEngine* engine, int c)
{
    //return genMoveRnd(engine, c);
    string candidate = engine->execute("all_legal " + COLOR[c]);
    if (candidate.length()<=4)
    {
        return "";
    }
    vector<pair<int,int>> v = parseFrom(candidate);
    return predict(engine, v);
}

string Player::genMoveRnd(GameEngine* engine, int c)
{
    string candidate = engine->execute("all_legal " + COLOR[c]);
    if (candidate.length()<=4)
    {
        return "";
    }
    vector<pair<int,int>> v = parseFrom(candidate);
    return POS2STR(v[rand()%v.size()]);
}

string Player::predict(GameEngine* engine, vector<pair<int,int>>& v)
{
    featurize(engine);
    //printInput();
    m_NN->forward(m_input,m_output);
    //printOutput();
    int x,y;
    float max = -FLT_MAX;
    for (int i;i<v.size();++i)
    {
        if ((*m_output)(v[i].first*m_size + v[i].second) > max)
        {
            x = v[i].first;
            y = v[i].second;
            max = (*m_output)(v[i].first*m_size+ v[i].second);
        }
    }
    return POS2STR(make_pair(x,y));
}

void Player::perturb()
{
    m_NN->perturb();
}

void Player::featurize(GameEngine* engine)
{
    vector<pair<int,int>> tmp;
    int fshift, shift;
    for(int i=0;i<m_size*m_size*N_FEATURES;++i) (*m_input)(i) = 0;

    fshift = BLACK; shift = m_size*m_size*fshift;
    tmp = parseFrom(engine->execute("list_stones black"));
    for(int i=0;i<tmp.size();++i)
    {
        (*m_input)(shift + tmp[i].first*m_size + tmp[i].second) = parseFeature(tmp[i].first, tmp[i].second, fshift, engine);
    }

    fshift = GROUPSIZE; shift = m_size*m_size*fshift;
    for(int i=0;i<tmp.size();++i)
    {
        (*m_input)(shift + tmp[i].first*m_size + tmp[i].second) = parseFeature(tmp[i].first, tmp[i].second, fshift, engine);
    }
    fshift = LIBERTY; shift = m_size*m_size*fshift;
    for(int i=0;i<tmp.size();++i)
    {
        (*m_input)(shift + tmp[i].first*m_size + tmp[i].second) = parseFeature(tmp[i].first, tmp[i].second, fshift, engine);
    }

    fshift = WHITE; shift = m_size*m_size*fshift;
    tmp = parseFrom(engine->execute("list_stones white"));
    for(int i=0;i<tmp.size();++i)
    {
        (*m_input)(shift + tmp[i].first*m_size + tmp[i].second) = parseFeature(tmp[i].first, tmp[i].second, fshift, engine);
    }
    fshift = GROUPSIZE; shift = m_size*m_size*fshift;
    for(int i=0;i<tmp.size();++i)
    {
        (*m_input)(shift + tmp[i].first*m_size + tmp[i].second) = parseFeature(tmp[i].first, tmp[i].second, fshift, engine);
    }
    fshift = LIBERTY; shift = m_size*m_size*fshift;
    for(int i=0;i<tmp.size();++i)
    {
        (*m_input)(shift + tmp[i].first*m_size + tmp[i].second) = parseFeature(tmp[i].first, tmp[i].second, fshift, engine);
    }

    fshift = LEGAL_BLACK; shift = m_size*m_size*fshift;
    tmp = parseFrom(engine->execute("all_legal black"));
    for(int i=0;i<tmp.size();++i)
    {
        (*m_input)(shift + tmp[i].first*m_size + tmp[i].second) = parseFeature(tmp[i].first, tmp[i].second, fshift, engine);
    }

    fshift = LEGAL_WHITE; shift = m_size*m_size*fshift;
    tmp = parseFrom(engine->execute("all_legal white"));
    for(int i=0;i<tmp.size();++i)
    {
        (*m_input)(shift + tmp[i].first*m_size + tmp[i].second) = parseFeature(tmp[i].first, tmp[i].second, fshift, engine);
    }

    fshift = D_EDGE; shift = m_size*m_size*fshift;
    for (int i=0;i<m_size;++i)
        for (int j=0;j<m_size;++j)
            (*m_input)(shift + i*m_size + j) = parseFeature(i,j,fshift,engine);

    fshift = D_CORNER; shift = m_size*m_size*fshift;
    for (int i=0;i<m_size;++i)
        for (int j=0;j<m_size;++j)
            (*m_input)(shift + i*m_size + j) = parseFeature(i,j,fshift,engine);

    //cout << *m_input << endl;
}

float Player::parseFeature(int x, int y, int F, GameEngine* engine)
{
    string tmp;
    switch(F)
    {
        case BLACK:
            return 1;
        case WHITE:
            return 1;
        case LEGAL_BLACK:
            return 1;
        case LEGAL_WHITE:
            return 1;
        case GROUPSIZE:
            tmp = engine->execute("worm_stones " + POS2STR(make_pair(x,y)));
            return tmp.length()/3-1;
        case LIBERTY:
            tmp = engine->execute("countlib " + POS2STR(make_pair(x,y)));
            return stoi(tmp.substr(2,tmp.length()-3));
        case D_EDGE:
            return min(min(x,y),min(m_size-1-x,m_size-1-y));
        case D_CORNER:
            return min(min(x+y,m_size-1-x+y), min(m_size-1-y+x, 2*m_size-2-x-y));
        default:
            return 0;
    }
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

void Player::initialize()
{
    int F = m_size*m_size*N_FEATURES;
    m_input = new Marray<float,1>(F);
    m_output = new Marray<float,1>(m_size*m_size);
}

void Player::printInput()
{
    printVec(m_input, m_size, N_FEATURES, "input");
}

void Player::printOutput()
{
    printVec(m_output, m_size, 1, "output");
}

void Player::saveTo(string fname)
{
    m_NN->saveTo(fname);
}

void Player::loadFrom(string fname)
{
    m_NN->loadFrom(fname);
}
