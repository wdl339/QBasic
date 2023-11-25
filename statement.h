#ifndef STATEMENT_H
#define STATEMENT_H

#include <QMainWindow>
#include <vector>
#include <exp.h>

using namespace std;

class Statement
{
public:
    Statement(int num);
    int lineNum;
    vector<Exp*> child;
    virtual QString showStr();
    virtual ~Statement();
};

class RemStmt: public Statement
{
public:
    RemStmt(int num, QString ss);
    QString showStr() override;
    virtual ~RemStmt();
};

class LetStmt: public Statement
{
public:
    LetStmt(int num, QString ss);
    QString showStr() override;
    virtual ~LetStmt();
};

//class PrintStmt: public Statement
//{

//};

class InputStmt: public Statement
{
public:
    InputStmt(int num, QString ss);
    QString showStr() override;
    virtual ~InputStmt();
};

class GotoStmt: public Statement
{
public:
    GotoStmt(int num, int val);
    QString showStr() override;
    virtual ~GotoStmt();
};

class IfStmt: public Statement
{
public:
    IfStmt(int num, int val);
    QString showStr() override;
    virtual ~IfStmt();

};

class EndStmt: public Statement
{
public:
    EndStmt(int num): Statement(num) {}
    QString showStr() override;
    virtual ~EndStmt();
};

#endif // STATEMENT_H
