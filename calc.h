#pragma once
#ifndef CALC_H
#define CALC_H

#include <QMainWindow>
#include <exp.h>
#include <vector>
#include <QMessageBox>

class Calc
{
    char* expression;
    enum token {OPAREN, ADD, SUB, MUL, DIV, MOD, POW, CPAREN, VALUE, EOL};
    token lastToken;

    token getOp(Exp*& value);
    void connectSyntaxNode(token s, vector<Exp*>& node);
    bool stringIsNum(QString s);
    bool isLetter(char c);
    bool isNum(char c);
public:
    Calc(QString ss);
    ~Calc()
    {
        delete expression;
    }
    Exp* makeSyntaxTree();
};

#endif // CALC_H
