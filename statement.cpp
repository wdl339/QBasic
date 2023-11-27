#include "statement.h"

Statement::Statement(int num)
{
    lineNum = num;
}

Statement::~Statement()
{

}

void Statement::run()
{

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

PrintStmt::~PrintStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

PrintStmt::PrintStmt(int num, QString ss): Statement(num)
{
    Calc calc(ss);
    child.push_back(calc.makeSyntaxTree());
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

EndStmt::~EndStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}




