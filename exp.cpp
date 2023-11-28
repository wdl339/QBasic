#include "exp.h"


Exp::~Exp()
{

}

VarExp::VarExp(QString n, int v)
{
    checkVaildName(n);
    name = n;
    val = v;
}

ConstExp::ConstExp(int v)
{
    val = v;
    name = QString::number(v);
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

IdentifierExp::~IdentifierExp()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

