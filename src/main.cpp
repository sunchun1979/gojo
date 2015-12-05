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
    srand(time(0));
    Test t;
    t.TestEvol();
}

