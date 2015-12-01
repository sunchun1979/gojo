#ifndef _COMMON
#define _COMMON

#include <string>

static const std::string COLOR[] = {"black", "white"};

#define N_LAYERS 10
static const int KernelSize[] = {5,3,3,3,3,3,3,3,3,3,3};

#define STR2POS(a,b) make_pair(a>'I'?a-'A'-1:a-'A',b-'1')
#define POS2STR(a) std::string({char(a.first>7?a.first+'B':a.first+'A'),char(a.second+'1'),'\0'})

#endif


