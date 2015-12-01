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
    if (candidate.length()<=4)
    {
        return "";
    }
    vector<pair<int,int>> v = parseFrom(candidate);
//    int ii = rand()%v.size();
//    auto p = v[ii];
//    string ret = POS2STR(p);
//    cout << "[" << candidate << "]" << endl << ii << endl << p.first << "," << p.second << endl << ret << endl;
//    return ret;
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

void Player::initialize()
{
    m_convLayer.resize(N_LAYERS);
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
