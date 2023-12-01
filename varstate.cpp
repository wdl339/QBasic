#include "varstate.h"

void VarState::setValue(int v)
{
    val = v;
}

int VarState::value()
{
    time += 1;
    return val;
}

int VarState::useTime()
{
    return time;
}
