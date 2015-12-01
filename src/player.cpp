#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include "common.h"
#include "player.h"
#include "gameEngine.h"

using namespace std;

string Player::genMove(GameEngine* engine, int c)
{
    featurize(engine);

    string candidate = engine->execute("all_legal " + COLOR[c]);
    if (candidate.length()<=4)
    {
        return "";
    }
    vector<pair<int,int>> v = parseFrom(candidate);
    return genMoveRandom(v);
}

string Player::genMoveRandom(vector<pair<int,int>>& v)
{
//    int ii = rand()%v.size();
//    auto p = v[ii];
//    string ret = POS2STR(p);
//    cout << "[" << candidate << "]" << endl << ii << endl << p.first << "," << p.second << endl << ret << endl;
//    return ret;
   return POS2STR(v[rand()%v.size()]);
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
    m_convLayer.resize(N_LAYERS);

    m_input = new Marray<float,1>(m_size*m_size*N_FEATURES);

    for(int i=0;i<N_LAYERS;++i)
    {
        m_convLayer[i] = new Marray<float,4>(m_size*m_size, m_size*m_size, KernelSize[i],KernelSize[i]);
        for(int i1=0;i1<m_size*m_size;++i1)
            for(int i2=0;i2<m_size*m_size;++i2)
                for(int i3=0;i3<KernelSize[i];++i3)
                    for(int i4=0;i4<KernelSize[i];++i4)
                        (*m_convLayer[i])(i1,i2,i3,i4) = rand()/(RAND_MAX/10.0) - 10.0;
    }
}
