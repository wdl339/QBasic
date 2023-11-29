#include "statement.h"

Statement::Statement(int num)
{
    lineNum = num;
}

Statement::~Statement()
{

}

RemStmt::RemStmt(int num, QString ss): Statement(num)
{
    StringExp* exp = new StringExp(ss);
    child.push_back(exp);
    type = REM;
}

LetStmt::LetStmt(int num, QString var, QString exp): Statement(num)
{
    VarExp* varExp = new VarExp(var);
    child.push_back(varExp);
    Calc calc(exp);
    child.push_back(calc.makeSyntaxTree());
    type = LET;
}

PrintStmt::PrintStmt(int num, QString ss): Statement(num)
{
    Calc calc(ss);
    child.push_back(calc.makeSyntaxTree());
    type = PRINT;
}

InputStmt::InputStmt(int num, QString ss): Statement(num)
{
    VarExp* exp = new VarExp(ss);
    child.push_back(exp);
    type = INPUT;
}

GotoStmt::GotoStmt(int num, int val): Statement(num)
{
    ConstExp* exp = new ConstExp(val);
    child.push_back(exp);
    type = GOTO;
}

IfStmt::IfStmt(int num, QString ss): Statement(num)
{
    int thenIndex = ss.indexOf("THEN");
    if (thenIndex != -1) {
        QString expBeforeThen = ss.left(thenIndex).trimmed();
        QString expAfterThen = ss.mid(thenIndex + 4).trimmed();

        QRegularExpression regex("(.*)\\s*([<>=])\\s*(.*)");
        QRegularExpressionMatch match = regex.match(expBeforeThen);

        if (match.hasMatch()) {
            QString exp1 = match.captured(1).trimmed();
            QString op = match.captured(2).trimmed();
            QString exp2 = match.captured(3).trimmed();

            Calc calc(exp1);
            child.push_back(calc.makeSyntaxTree());
            StringExp* com = new StringExp(op);
            child.push_back(com);
            Calc calc2(exp2);
            child.push_back(calc2.makeSyntaxTree());

        } else {
            throw QString("非法输入");
        }
        ConstExp* n = new ConstExp(expAfterThen.toInt());
        child.push_back(n);
    } else {
        throw QString("非法输入");
    }
    type = IF;
}

EndStmt::EndStmt(int num): Statement(num)
{
    type = END;
}

void LetStmt::run(map<QString, int>& varTable)
{
    int res = child[1]->eval(varTable);
    varTable[child[0]->name] = res;
    child[0]->val = res;
}

void PrintStmt::run(map<QString, int>& varTable)
{
    child[0]->eval(varTable);
}

void IfStmt::run(map<QString, int>& varTable)
{
    child[0]->eval(varTable);
    child[2]->eval(varTable);
    bool flag = false;
    if(child[1]->name == "=") {
        if(child[0]->val == child[2]->val) {
            flag = true;
        }
    } else if (child[1]->name == ">") {
        if(child[0]->val > child[2]->val) {
            flag = true;
        }
    } else if (child[1]->name == "<") {
        if(child[0]->val < child[2]->val) {
            flag = true;
        }
    }
    if(flag) {
        child[1]->val = child[3]->val;
    }
}

RemStmt::~RemStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

LetStmt::~LetStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

PrintStmt::~PrintStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}


InputStmt::~InputStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

GotoStmt::~GotoStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

IfStmt::~IfStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

EndStmt::~EndStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}
