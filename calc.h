#pragma once
#ifndef CALC_H
#define CALC_H

#include <QMainWindow>
#include <exp.h>
#include <vector>

class Calc
{
    char* expression;
    enum token {OPAREN, ADD, SUB, MUL, DIV, POW, CPAREN, VALUE};

    token getOp(Exp*& value);
    void makeSyntaxNode(token s, vector<Exp*>& node);
    void connectSyntaxNode(token s, vector<Exp*>& node);
public:
    Calc(QString ss);
    Exp* makeSyntaxTree();
};

#endif // CALC_H
