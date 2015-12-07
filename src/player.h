#ifndef _PLAYER
#define _PLAYER

#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include "common.h"
#include "nn.h"

#include "../utils/ltensor/LTensor.h"

using namespace std;

class GameEngine;

class Player
{
    private:
        int m_size = 9;
        Marray<float,1>* m_input = NULL;
        Marray<float,1>* m_output = NULL;
        CNN* m_NN;

    public:
        Player();
        Player(vector<Player*> candidates, vector<float> weights);

        ~Player()
        {
            if (m_input!=NULL)
            {
                delete m_input;
                m_input = NULL;
            }
            if (m_output!=NULL)
            {
                delete m_output;
                m_output = NULL;
            }
            if (m_NN != NULL)
            {
                delete m_NN;
                m_NN = NULL;
            }
        }

        virtual string genMove(GameEngine* engine, int c);
        string genMoveRnd(GameEngine* engine, int c);
        void featurize(GameEngine* engine);
        float parseFeature(int x, int y, int F, GameEngine* engine);

        string predict(GameEngine* engine, vector<pair<int,int>>& v);
        void printOutput();
        void printInput();
        void saveTo(string fname);
        void loadFrom(string fname);
        void perturb();

    private:
        vector<pair<int,int>> parseFrom(string s);
        void initialize();
};

class PlayerRnd : public Player
{
    public:
        PlayerRnd() : Player() {}
        ~PlayerRnd() {}
        string genMove(GameEngine* engine, int c)
        {
            return genMoveRnd(engine, c);
        }
};

#endif
