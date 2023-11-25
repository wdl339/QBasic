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

LetStmt::~LetStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

LetStmt::LetStmt(int num, QString ss): Statement(num)
{
    StringExp* exp = new StringExp(ss);
    child.push_back(exp);
}

QString LetStmt::showStr()
{
    return QString::number(lineNum) + ' ' + "REM" + ' ' + (child[0])->name;
}

InputStmt::~InputStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

InputStmt::InputStmt(int num, QString ss): Statement(num)
{
    VarExp* exp = new VarExp(ss);
    child.push_back(exp);
}

QString InputStmt::showStr()
{
    return QString::number(lineNum) + ' ' + "INPUT" + ' ' + (child[0])->name;
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

IfStmt::~IfStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

IfStmt::IfStmt(int num, int val): Statement(num)
{
    ConstExp* exp = new ConstExp(val);
    child.push_back(exp);
}

QString IfStmt::showStr()
{
    return QString::number(lineNum) + ' ' + "GOTO" + ' ' + QString::number((child[0])->val);
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



