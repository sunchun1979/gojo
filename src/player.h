#ifndef _PLAYER
#define _PLAYER

#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include "common.h"

#include "../utils/ltensor/LTensor.h"

using namespace std;

class GameEngine;

class Player
{
    private:
        int m_size = 9;
        vector< Marray<float,4>* > m_convLayer;

    public:
        Player()
        {
            initialize();
        }

        ~Player()
        {
            for(int i=0;i<N_LAYERS;++i)
            {
                if (m_convLayer[i]!=NULL)
                {
                    delete m_convLayer[i];
                    m_convLayer[i] = NULL;
                }
            }
        }

        string genMove(GameEngine* engine, int c);

    private:
        vector<pair<int,int>> parseFrom(string s);
        void initialize();
};
#endif
