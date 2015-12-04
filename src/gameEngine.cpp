#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include "common.h"
#include "gameEngine.h"
#include "player.h"

using namespace std;

GameEngine::GameEngine(int size):m_size(size)
{
    startGNUGo();
    string cmd("boardsize " + to_string(m_size));
    execute(cmd);
}

string GameEngine::execute(string command)
{
    //cout << "command: " << command << endl;
    //cout.flush();
    if (writeTo(command) == 0)
    {
        //string p = readFrom();
        //cout << "return: " << p;
        //cout.flush();
        //return p;
        return readFrom();
    }
}

void GameEngine::game_human(Player* p1, int computer_color)
{
    execute("clear_board");
    execute("komi 7.5");
    string cmdComputer;
    string cmdHuman;
    if (computer_color == BLACK)
    {
        cmdComputer = "play black ";
        cmdHuman = "play white ";
        execute(cmdComputer + p1->genMove(this,computer_color));
    }else
    {
        cmdComputer = "play white";
        cmdHuman = "play black ";
    }
    string vtx = "";
    cout << execute("showboard") << endl;
    cout.flush();
    cin >> vtx;
    while(vtx != "end")
    {
        execute(cmdHuman + vtx);
        execute(cmdComputer + p1->genMove(this,computer_color));
        cout << execute("showboard") << endl; cout.flush();
        cin >> vtx;
    }
}

int GameEngine::game(Player* p1, Player* p2)
{
    execute("clear_board");
    execute("komi 7.5");
    // first two moves random
    execute("play black " + p1->genMoveRnd(this,0));
    execute("play white " + p1->genMoveRnd(this,0));
    for (int i=0;i<m_depth;++i)
    {
        string move;
        move = p1->genMove(this,0);
        if (!move.empty())
            execute("play black " + move);
        else
        {
            break;
        }
        move = p2->genMove(this,1);
        if (!move.empty())
            execute("play white " + move);
        else
        {
            break;
        }
    }
    string result = execute("estimate_score");
    return (result[2] == 'B');
}

void GameEngine::error(string errmsg)
{
    cerr << errmsg << endl;
    abort();
}

void GameEngine::startGNUGo()
{
    if (pipe(pfd_a) == -1)
        error("can't open pipe a");
    if (pipe(pfd_b) == -1)
        error("can't open pipe b");
    switch (fork()) {
        case -1:
            error("fork failed (try chopsticks)");
        case 0:
            /* Attach pipe a to stdin */
            if (dup2(pfd_a[0], 0) == -1) 
                error("dup pfd_a[0] failed");
            /* attach pipe b to stdout" */
            if (dup2(pfd_b[1], 1) == -1)
                error("dup pfd_b[1] failed");
            execlp("gnugo", "gnugo", "--mode", "gtp", "--quiet", NULL);
            error("execlp failed");
    }
    /* We use stderr to communicate with the client since stdout is needed. */
    /* Attach pipe a to to_gnugo_stream  */
    to_gnugo_stream = fdopen(pfd_a[1], "w");
    /* Attach pipe b to from_gnugo_stream */
    from_gnugo_stream = fdopen(pfd_b[0], "r");
}

void GameEngine::closeGNUGo()
{
    execute("quit");
    if (to_gnugo_stream != NULL)
        fclose(to_gnugo_stream);
    if (from_gnugo_stream != NULL)
        fclose(from_gnugo_stream);
}

int GameEngine::writeTo(string command)
{
    if (fprintf(to_gnugo_stream, "%s\n", command.c_str()) < 0)
        error("writeTo fails: " + command);
    fflush(to_gnugo_stream);
    return 0;
}

string GameEngine::readFrom()
{
    string ret;
    int length = 0;
    while(length!=1)
    {
        if (!fgets(gnugo_line,ENGINE_BUF_SIZE,from_gnugo_stream))
            error("readFrom fails");
        length=strlen(gnugo_line);
        ret = ret + gnugo_line;
    }
    fflush(stderr);
    return ret;
}


