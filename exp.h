#pragma once
#ifndef EXP_H
#define EXP_H

#include <QMainWindow>
#include <vector>

using namespace std;

class Exp
{

public:
    QString name;
    int val;
    vector<Exp*> child;
    Exp(QString n = "", int v = 0): name(n), val(v) {};
    virtual ~Exp();
};

class VarExp : public Exp
{
public:
    VarExp(QString n, int v = 0);
    void checkVaildName(QString s);
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
    StringExp(QString n, int v = 0): Exp(n, v) {}
    virtual ~StringExp();
};

class IdentifierExp : public Exp
{
public:
    IdentifierExp(QString n, int v = 0, Exp* p1 = nullptr, Exp* p2 = nullptr):
        Exp(n, v)
    {
        child.push_back(p1);
        child.push_back(p2);
    }
    ~IdentifierExp();

};

#endif // EXP_H
//13 PRINT -1 + 4 MOD 2 + 4 * 3 / 5 ** 8
