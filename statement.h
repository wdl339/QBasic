#pragma once
#ifndef STATEMENT_H
#define STATEMENT_H

#include <QMainWindow>
#include <vector>
#include <exp.h>
#include <calc.h>

using namespace std;

class Statement
{
public:
    Statement(int num);
    int lineNum;
    vector<Exp*> child;
    void run();
    virtual ~Statement();
};

class RemStmt: public Statement
{
public:
    RemStmt(int num, QString ss);
    virtual ~RemStmt();
};

class LetStmt: public Statement
{
public:
    LetStmt(int num, QString ss);
    virtual ~LetStmt();
};

class PrintStmt: public Statement
{
public:
    PrintStmt(int num, QString ss);
    virtual ~PrintStmt();
};

class InputStmt: public Statement
{
public:
    InputStmt(int num, QString ss);
    virtual ~InputStmt();
};

class GotoStmt: public Statement
{
public:
    GotoStmt(int num, int val);
    virtual ~GotoStmt();
};

class IfStmt: public Statement
{
public:
    IfStmt(int num, int val);
    virtual ~IfStmt();

};

class EndStmt: public Statement
{
public:
    EndStmt(int num);
    virtual ~EndStmt();
};

#endif // STATEMENT_H
