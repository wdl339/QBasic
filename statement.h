#pragma once
#ifndef STATEMENT_H
#define STATEMENT_H

#include <QMainWindow>
#include <vector>
#include <queue>
#include <stack>
#include <exp.h>
#include <calc.h>
#include <program.h>
#include <varstate.h>
#include <QRegularExpression>

using namespace std;

// 语句类型枚举
enum stmtType {REM, LET, PRINT, INPUT, GOTO, IF, END, ERROR};

//
// 语句类
//
class Statement
{
protected:

    int lineNum; // 语句的行号
    int runTime; // 语句运行次数
    vector<Exp*> child; // 语句内包含的一些表达式（树）
    virtual QString getRunTime(); // 获取运行次数

public:
    stmtType type; // 语句类型

    Statement(int num); // 构造
    ~Statement(); // 析构

    QString getWaitVarName(); // 用于INPUT语句获取INPUT后的变量名
    int getchildVal(int num); // 获取child[num]的值
    int getLineNum(); // 获取行号

    virtual void run(map<QString, VarState>& varTable); // 语句的运行

    QString syntaxTreeStr(Program*& program);  // 获取表达式树的字符串
    virtual QString getTreeNode(); // 获取表达式树的根节点的展示
};

//
// REM语句
//
class RemStmt: public Statement
{
public:
    RemStmt(int num, QString ss);
    QString getTreeNode() override;
    ~RemStmt();
};

//
// LET语句
//
class LetStmt: public Statement
{
public:
    LetStmt(int num, QString var, QString exp);
    QString getTreeNode() override;
    void run(map<QString, VarState>& varTable) override;
    ~LetStmt();
};

//
// PRINT语句
//
class PrintStmt: public Statement
{
public:
    PrintStmt(int num, QString ss);
    QString getTreeNode() override;
    void run(map<QString, VarState>& varTable) override;
    ~PrintStmt();
};

//
// INPUT语句
//
class InputStmt: public Statement
{
public:
    InputStmt(int num, QString ss);
    QString getTreeNode() override;
    ~InputStmt();
};

//
// GOTO语句
//
class GotoStmt: public Statement
{
public:
    GotoStmt(int num, int val);
    QString getTreeNode() override;
    ~GotoStmt();
};

//
// IF语句
//
class IfStmt: public Statement
{
    int runTimeFalse = 0; // IF语句特有的false和true情况分开计数
public:
    IfStmt(int num, QString ss);
    void run(map<QString, VarState>& varTable) override;
    QString getRunTime() override;
    QString getTreeNode() override;
    ~IfStmt();

};

//
// END语句
//
class EndStmt: public Statement
{
public:
    EndStmt(int num);
    QString getTreeNode() override;
    ~EndStmt();
};

//
// ERROR语句（用于展示表达式树时，不合法语句显示ERROR
//
class ErrorStmt: public Statement
{
public:
    ErrorStmt(int num);
    QString getTreeNode() override;
    ~ErrorStmt();
};

#endif // STATEMENT_H
