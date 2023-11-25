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
    VarExp(QString n, int v): Exp(n, v) {}
};

class ConstExp : public Exp
{
public:
    ConstExp(int v, QString n = ""): Exp(n, v) {}
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
    IdentifierExp(QString n, int v, Exp* p1 = nullptr, Exp* p2 = nullptr):
        Exp(n, v)
    {
        child[0] = p1;
        child[1] = p2;
    }

};

#endif // EXP_H
