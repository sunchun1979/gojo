#ifndef _GAMEENGINE
#define _GAMEENGINE

#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include "common.h"

using namespace std;

class Player;

class GameEngine
{
    private:
        int pfd_a[2];
        int pfd_b[2];
        char gnugo_line[1024], client_line[1024];
        int length = 0;
        int verbose = 1;
        FILE *to_gnugo_stream, *from_gnugo_stream;

        int m_size;
        int m_depth = 50;

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
