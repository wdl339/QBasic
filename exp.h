#pragma once
#ifndef EXP_H
#define EXP_H

#include <QMainWindow>
#include <vector>
#include <varstate.h>

using namespace std;

class Exp
{
public:
    int val;
    QString name;
    vector<Exp*> child;
    Exp(QString n = "", int v = 0);
    virtual int eval(map<QString, VarState>& varTable);
    virtual ~Exp();
};

class VarExp : public Exp
{
public:
    VarExp(QString n, int v = 0);
    void checkVaildName(QString s);
    int eval(map<QString, VarState>& varTable) override;
    ~VarExp();
};

class ConstExp : public Exp
{
public:
    ConstExp(int v);
    ~ConstExp();
};

class StringExp : public Exp
{
public:
    StringExp(QString n, int v = 0);
    virtual ~StringExp();
};

class CompoundExp : public Exp
{
public:
    CompoundExp(QString n, int v = 0, Exp* p1 = nullptr, Exp* p2 = nullptr);
    int eval(map<QString, VarState>& varTable) override;
    ~CompoundExp();

};

#endif // EXP_H
