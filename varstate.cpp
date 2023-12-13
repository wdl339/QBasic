#include "varstate.h"

//
// setValue以int数为参数，设置变量的值
//
void VarState::setValue(int v)
{
    val = v;
}

//
// value返回变量的值，并且增加一次use count
//
int VarState::value()
{
    time += 1;
    return val;
}

//
// useTime返回变量的use count
//
int VarState::useTime()
{
    return time;
}

//
// clearTime将变量use count置0
//
void VarState::clearTime()
{
    time = 0;
}
