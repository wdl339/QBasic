#pragma once
#ifndef CALC_H
#define CALC_H

#include <QMainWindow>
#include <exp.h>
#include <vector>
#include <QMessageBox>

//
// 负责构建表达式树的“计算机”类
//
class Calc
{
    char* expression; // 表达式
    enum token {OPAREN, ADD, SUB, MUL, DIV, MOD, POW, CPAREN, VALUE, EOL}; // 枚举符号类（按优先级排序）
    token lastToken; // 构建表达式树时，上一个读入的符号

    token getOp(Exp*& value); // 读入expression的下一段内容
    void connectSyntaxNode(token s, vector<Exp*>& node); // 构造CompoundExp压入栈中
    bool isLetter(char c); // 判断是否是一个字母
    bool isNum(char c); // 判断是否是一个数字
    bool checkVarOrVal(Exp*& value); // 检查expression的下一段内容是不是数字或变量
    token dealWithSub(Exp*& value); // 对于‘-’是负号还是减号的判断及处理

public:
    Calc(QString ss); // 构造
    ~Calc() // 析构
    {
        delete expression;
    }
    Exp* makeSyntaxTree(); // 构造表达式树并返回
};

#endif // CALC_H
