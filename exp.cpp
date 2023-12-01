#include "exp.h"


Exp::Exp(QString n, int v)
{
    name = n;
    val = v;
};

Exp::~Exp()
{

}

VarExp::VarExp(QString n, int v): Exp(n, v)
{
    checkVaildName(n);
}

ConstExp::ConstExp(int v): Exp(QString::number(v), v)
{

}

StringExp::StringExp(QString n, int v): Exp(n, v)
{

}

CompoundExp::CompoundExp(QString n, int v, Exp* p1, Exp* p2):
    Exp(n, v)
{
    child.push_back(p1);
    child.push_back(p2);
}

int Exp::eval(map<QString, VarState>& varTable)
{
    return val;
}

int VarExp::eval(map<QString, VarState>& varTable)
{
    val = varTable[name].value();
    return val;
}

int CompoundExp::eval(map<QString, VarState>& varTable)
{
    int num1 = child[0]->eval(varTable);
    int res = 0;
    if (name == "+") res = num1 + child[1]->eval(varTable);
    if (name == "-") {
        if(child[1]) {
            res = num1 - child[1]->eval(varTable);
        } else {
            res = -num1;
        }
    }
    if (name == "*") res = num1 * child[1]->eval(varTable);
    if (name == "/") {
        int dividend = child[1]->eval(varTable);
        if (dividend == 0) throw("除0");
        res = num1 / dividend;
    }
    if (name == "MOD") res = num1 % child[1]->eval(varTable);
    if (name == "**") {
        res = 1;
        int num2 = child[1]->eval(varTable);
        for(int i = 0; i < num2; i++) {
            res *= num1;
        }
    }
    val = res;
    return res;
}

void VarExp::checkVaildName(QString s)
{
    QRegExp regExp("^[a-zA-Z_][a-zA-Z0-9_]*$");
    if (regExp.exactMatch(s)) {
        QString sMaintain[13] = {"LET", "REM", "PRINT", "INPUT", "GOTO", "IF", "END",
                                 "RUN", "LOAD", "LIST", "CLEAR", "HELP", "QUIT"
                                };
        for(int i = 0; i < 13; i++) {
            if(s == sMaintain[i]) {
                throw QString("非法变量名！");
            }
        }
        return;
    }
    throw QString("非法变量名！");
}

VarExp::~VarExp()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

ConstExp::~ConstExp()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

StringExp::~StringExp()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

CompoundExp::~CompoundExp()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

