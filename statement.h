#ifndef STATEMENT_H
#define STATEMENT_H

#include <QMainWindow>
#include <vector>
#include <exp.h>

using namespace std;

class Statement
{
public:
    Statement(string ss);
    int lineNum;
    vector<Exp*> child;
    string s;
};

class RemStmt: public Statement
{

};

class LetStmt: public Statement
{

};

class PrintStmt: public Statement
{

};

class InputStmt: public Statement
{

};

class GotoStmt: public Statement
{

};

class IfStmt: public Statement
{

};

class EndStmt: public Statement
{

};

#endif // STATEMENT_H
