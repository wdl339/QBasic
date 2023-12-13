#pragma once
#ifndef VARSTATE_H
#define VARSTATE_H

//
// 变量状态类
//
class VarState
{
    int val; // 变量的值
    int time = 0; // 变量使用次数(use count)
public:
    VarState(int v = 0): val(v), time(0) {} // 构造
    void setValue(int v); // 设置变量值
    int value(); // 返回变量值
    int useTime(); // 返回use count
    void clearTime(); // 清空use count
};

#endif // VARSTATE_H
