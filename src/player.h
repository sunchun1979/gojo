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
        Marray<float,1>* m_input = NULL;

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
            if (m_input!=NULL)
            {
                delete m_input;
                m_input = NULL;
            }
        }

        string genMove(GameEngine* engine, int c);
        void featurize(GameEngine* engine);
        float parseFeature(int x, int y, int F, GameEngine* engine);

    private:
        vector<pair<int,int>> parseFrom(string s);
        void initialize();
        string genMoveRandom(vector<pair<int,int>>& v);
};
#endif
