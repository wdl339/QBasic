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
    treeNode = QString::number(num) + " " + "REM\n";
}

LetStmt::~LetStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

LetStmt::LetStmt(int num, QString var, QString exp): Statement(num)
{
    VarExp* varExp = new VarExp(var);
    child.push_back(varExp);
    Calc calc(exp);
    child.push_back(calc.makeSyntaxTree());
    treeNode = QString::number(num) + " " + "LET =\n";
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
    treeNode = QString::number(num) + " " + "PRINT\n";
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
    treeNode = QString::number(num) + " " + "INPUT\n";
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
    treeNode = QString::number(num) + " " + "GOTO\n";
}

IfStmt::~IfStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

IfStmt::IfStmt(int num, QString ss): Statement(num)
{
    int thenIndex = ss.indexOf("THEN");
    if (thenIndex != -1) {
        QString expBeforeThen = ss.left(thenIndex).trimmed();
        QString expAfterThen = ss.mid(thenIndex + 4).trimmed();
        QString statement = "IF (a + b) > (c - d) THEN 10";

        QRegularExpression regex("(.*)\\s*([<>=])\\s*(.*)");
        QRegularExpressionMatch match = regex.match(expBeforeThen);

        if (match.hasMatch()) {
            QString exp1 = match.captured(1).trimmed();
            QString op = match.captured(2).trimmed();
            QString exp2 = match.captured(3).trimmed();

            Calc calc(exp1);
            child.push_back(calc.makeSyntaxTree());
            StringExp* iden = new StringExp(op);
            child.push_back(iden);
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
    treeNode = QString::number(num) + " " + "IF THEN\n";
}

EndStmt::~EndStmt()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

EndStmt::EndStmt(int num): Statement(num)
{
    treeNode = QString::number(num) + " " + "END\n";
}



