#ifndef EXP_H
#define EXP_H

#include <QMainWindow>
#include <vector>

class Exp
{
public:
    QString name;
    int val;
    Exp();
};

class VarExp : public Exp
{

};

class ConstExp : public Exp
{

};

class IdentifierExp : public Exp
{


};

#endif // EXP_H
