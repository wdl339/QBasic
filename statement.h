#pragma once
#ifndef STATEMENT_H
#define STATEMENT_H

#include <QMainWindow>
#include <vector>
#include <exp.h>
#include <calc.h>
#include <QRegularExpression>

using namespace std;

enum stmtType {REM, LET, PRINT, INPUT, GOTO, IF, END};

class Statement
{
public:
    Statement(int num);
    stmtType type;
    int lineNum;
    vector<Exp*> child;
    virtual void run(map<QString, int>& varTable) {}
    virtual ~Statement();
};

class RemStmt: public Statement
{
public:
    RemStmt(int num, QString ss);
    void run(map<QString, int>& varTable) {}
    ~RemStmt();
};

class LetStmt: public Statement
{
public:
    LetStmt(int num, QString var, QString exp);
    void run(map<QString, int>& varTable) override;
    ~LetStmt();
};

class PrintStmt: public Statement
{
public:
    PrintStmt(int num, QString ss);
    void run(map<QString, int>& varTable) override;
    ~PrintStmt();
};

class InputStmt: public Statement
{
public:
    InputStmt(int num, QString ss);
    void run(map<QString, int>& varTable) {}
    ~InputStmt();
};

class GotoStmt: public Statement
{
public:
    GotoStmt(int num, int val);
    void run(map<QString, int>& varTable) {}
    ~GotoStmt();
};

class IfStmt: public Statement
{
public:
    IfStmt(int num, QString ss);
    void run(map<QString, int>& varTable) override;
    ~IfStmt();

};

class EndStmt: public Statement
{
public:
    EndStmt(int num);
    void run(map<QString, int>& varTable) {}
    ~EndStmt();
};

#endif // STATEMENT_H
