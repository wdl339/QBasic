#pragma once
#ifndef VARSTATE_H
#define VARSTATE_H


class VarState
{
    int val;
    int time = 0;
public:
    VarState(int v = 0): val(v), time(0) {}
    void setValue(int v);
    int value();
    int useTime();
};

#endif // VARSTATE_H
