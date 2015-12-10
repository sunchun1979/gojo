#include <cstdio>
#include <iostream>
#include <ctime>

#include "common.h"
#include "player.h"
#include "gameEngine.h"
#include "evolEngine.h"

#include "test.h"

using namespace std;

int main(int argc, char* argv[])
{
    Test t;
    //t.TestInteractive(argv[1]);
    //t.TestAgainstRandom(argv[1]);
    //t.TestAgainstGNUGo(argv[1]);
    t.TestAgainstNNRandom(argv[1]);
}

