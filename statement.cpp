#include "statement.h"

Statement::Statement(int num)
{
    lineNum = num;
}

Statement::~Statement()
{

}

QString Statement::showStr()
{
    return " ";
}

RemStmt::~RemStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

RemStmt::RemStmt(int num, QString ss): Statement(num)
{
    StringExp* exp = new StringExp(ss);
    child.push_back(exp);
}

QString RemStmt::showStr()
{
    return QString::number(lineNum) + ' ' + "REM" + ' ' + (child[0])->name;
}

EndStmt::~EndStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

QString EndStmt::showStr()
{
    return QString::number(lineNum) + ' ' + "END";
}

GotoStmt::~GotoStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

GotoStmt::GotoStmt(int num, int val): Statement(num)
{
    ConstExp* exp = new ConstExp(val);
    child.push_back(exp);
}

QString GotoStmt::showStr()
{
    return QString::number(lineNum) + ' ' + "GOTO" + ' ' + QString::number((child[0])->val);
}

