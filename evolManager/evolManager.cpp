#include <cstdio>
#include <iostream>

#include "gameEngine.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout << "evolManger" << endl;
    GameEngine ge1;
    cout << ge1.execute("showboard") << endl;
}
