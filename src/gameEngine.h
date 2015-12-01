#ifndef _GAMEENGINE
#define _GAMEENGINE

#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include "common.h"

#define ENGINE_BUF_SIZE 2048

using namespace std;

class Player;

class GameEngine
{
    private:
        int pfd_a[2];
        int pfd_b[2];
        char gnugo_line[ENGINE_BUF_SIZE], client_line[ENGINE_BUF_SIZE];
        int length = 0;
        int verbose = 1;
        FILE *to_gnugo_stream, *from_gnugo_stream;

        int m_size;
        int m_depth = 35;

    public:
        GameEngine(int size);
        ~GameEngine()
        {
            closeGNUGo();
        }

        string execute(string command);
        int game(Player* p1, Player* p2);

    private:
        void error(string errmsg);
        void startGNUGo();
        void closeGNUGo();
        int writeTo(string command);
        string readFrom();

};

#endif
