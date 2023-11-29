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

int Exp::eval(map<QString, int>& varTable)
{
    return val;
}

int VarExp::eval(map<QString, int>& varTable)
{
    val = varTable[name];
    return varTable[name];
}

int ConstExp::eval(map<QString, int>& varTable)
{
    return val;
}

int StringExp::eval(map<QString, int>& varTable)
{
    return val;
}

int CompoundExp::eval(map<QString, int>& varTable)
{
    int num1 = child[0]->eval(varTable);
    int num2 = child[1]->eval(varTable);
    int res = 0;
    if (name == "+") res = num1 + num2;
    if (name == "-") res = num1 - num2;
    if (name == "*") res = num1 * num2;
    if (name == "/") {
        if (num2 == 0) throw("除0");
        res = num1 / num2;
    }
    if (name == "MOD") res = num1 % num2;
    if (name == "**") {
        res = 1;
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

