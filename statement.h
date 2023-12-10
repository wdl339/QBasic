#pragma once
#ifndef STATEMENT_H
#define STATEMENT_H

#include <QMainWindow>
#include <vector>
#include <queue>
#include <exp.h>
#include <calc.h>
#include <program.h>
#include <varstate.h>
#include <QRegularExpression>

using namespace std;

enum stmtType {REM, LET, PRINT, INPUT, GOTO, IF, END, ERROR};

class Statement
{
protected:

    int lineNum;
    int runTime = 0;
    vector<Exp*> child;
    virtual QString getRunTime();

public:
    stmtType type;

    Statement(int num);
    ~Statement();

    QString getWaitVarName();
    int getchildVal(int num);
    int getLineNum();

    virtual void run(map<QString, VarState>& varTable);

    QString syntaxTreeStr(Program*& program);  // 获取表达式树的字符串
    virtual QString getTreeNode(); // 获取表达式树的根节点的展示
};

class RemStmt: public Statement
{
public:
    RemStmt(int num, QString ss);
    QString getTreeNode() override;
    ~RemStmt();
};

class LetStmt: public Statement
{
public:
    LetStmt(int num, QString var, QString exp);
    QString getTreeNode() override;
    void run(map<QString, VarState>& varTable) override;
    ~LetStmt();
};

class PrintStmt: public Statement
{
public:
    PrintStmt(int num, QString ss);
    QString getTreeNode() override;
    void run(map<QString, VarState>& varTable) override;
    ~PrintStmt();
};

class InputStmt: public Statement
{
public:
    InputStmt(int num, QString ss);
    QString getTreeNode() override;
    ~InputStmt();
};

class GotoStmt: public Statement
{
public:
    GotoStmt(int num, int val);
    QString getTreeNode() override;
    ~GotoStmt();
};

class IfStmt: public Statement
{
    int runTimeFalse = 0;
public:
    IfStmt(int num, QString ss);
    void run(map<QString, VarState>& varTable) override;
    QString getRunTime() override;
    QString getTreeNode() override;
    ~IfStmt();

};

class EndStmt: public Statement
{
public:
    EndStmt(int num);
    QString getTreeNode() override;
    ~EndStmt();
};

class ErrorStmt: public Statement
{
public:
    ErrorStmt(int num);
    QString getTreeNode() override;
    ~ErrorStmt();
};

#endif // STATEMENT_H
