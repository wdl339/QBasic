#pragma once
#ifndef EXP_H
#define EXP_H

#include <QMainWindow>
#include <vector>
#include <varstate.h>

using namespace std;

class Statement;
class LetStmt;
class IfStmt;

//
// 表达式类
//
class Exp
{
    friend Statement;
    friend LetStmt;
    friend IfStmt;
protected:
    int val; // Exp的值
    QString name; // Exp的名字
    vector<Exp*> child; // Exp的孩子Exp（仅对CompoundExp有用）
public:
    Exp(QString n = "", int v = 0); // 构造
    virtual int eval(map<QString, VarState>& varTable); // 求Exp的值
    virtual ~Exp(); // 析构
};

//
// 变量表达式
//
class VarExp : public Exp
{
public:
    VarExp(QString n, int v = 0);
    void checkVaildName(QString s); // 检查是否为合法变量名
    int eval(map<QString, VarState>& varTable) override;
    ~VarExp();
};

//
// 常量表达式
//
class ConstExp : public Exp
{
public:
    ConstExp(int v);
    ~ConstExp();
};

//
// 字符串表达式（主要是REM用）
//
class StringExp : public Exp
{
public:
    StringExp(QString n, int v = 0);
    virtual ~StringExp();
};

//
// 组合表达式（符号）
//
class CompoundExp : public Exp
{
public:
    CompoundExp(QString n, int v = 0, Exp* p1 = nullptr, Exp* p2 = nullptr);
    int eval(map<QString, VarState>& varTable) override;
    ~CompoundExp();

};

#endif // EXP_H
