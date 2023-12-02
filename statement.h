#pragma once
#ifndef STATEMENT_H
#define STATEMENT_H

#include <QMainWindow>
#include <vector>
#include <exp.h>
#include <calc.h>
#include <varstate.h>
#include <QRegularExpression>

using namespace std;

enum stmtType {REM, LET, PRINT, INPUT, GOTO, IF, END, ERROR};

class Statement
{
public:
    Statement(int num);
    stmtType type;
    int lineNum;
    int runTime = 0;
    vector<Exp*> child;

    QString getChildName();
    int getchildVal(int num);
    bool stringIsPosNum(QString s);
    virtual QString getRunTime();
    virtual void run(map<QString, VarState>& varTable);
    ~Statement();
};

class RemStmt: public Statement
{
public:
    RemStmt(int num, QString ss);
    ~RemStmt();
};

class LetStmt: public Statement
{
public:
    LetStmt(int num, QString var, QString exp);
    void run(map<QString, VarState>& varTable) override;
    ~LetStmt();
};

class PrintStmt: public Statement
{
public:
    PrintStmt(int num, QString ss);
    void run(map<QString, VarState>& varTable) override;
    ~PrintStmt();
};

class InputStmt: public Statement
{
public:
    InputStmt(int num, QString ss);
    ~InputStmt();
};

class GotoStmt: public Statement
{
public:
    GotoStmt(int num, int val);
    ~GotoStmt();
};

class IfStmt: public Statement
{
public:
    int runTimeFalse = 0;

    IfStmt(int num, QString ss);
    void run(map<QString, VarState>& varTable) override;
    QString getRunTime() override;
    ~IfStmt();

};

class EndStmt: public Statement
{
public:
    EndStmt(int num);
    ~EndStmt();
};

class ErrorStmt: public Statement
{
public:
    ErrorStmt(int num);
    ~ErrorStmt();
};

#endif // STATEMENT_H
