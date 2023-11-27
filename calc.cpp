#include "calc.h"

Calc::Calc(QString ss)
{
    string str = ss.toStdString();
    char* ch = new char[str.length() + 1];
    strcpy(ch, str.c_str());
}

//enum token {OPAERN, ADD, SUB, MUL, DIV, POW, CPAREN, VALUE};
Exp* Calc::makeSyntaxTree()
{
    vector<token> op;
    vector<Exp*> node;
    token lastOp, topOp;
    Exp* currentValue;
    while((lastOp = getOp(currentValue)) != EOF) {
        switch(lastOp) {
            case VALUE:

            case OPAREN:
            case POW:
                op.push_back(lastOp);
                break;
            case CPAREN:
                while(!op.empty() && (topOp = op.back()) != OPAREN) {
                    connectSyntaxNode(topOp, node);
                    op.pop_back();
                }
                if(topOp != OPAREN) {
                    throw QString("表达式不合法（缺括号）");
                } else {
                    op.pop_back();
                }
                break;

        }


    }
    return node.back();
}

//enum token {OPAREN, ADD, SUB, MUL, DIV, POW, CPAREN, VALUE};
Calc::token Calc::getOp(Exp*& value)
{
    if(s == "(")return OPAREN;
    if(s == "+")return ADD;
    if(s == "-")return SUB;
    if(s == "*")return MUL;
    if(s == "/")return DIV;
    if(s == "**")return POW;
    if(s == ")")return CPAREN;

    return VALUE;
}

bool stringIsNum(QString s)
{
    QRegExp regExp("[0-9]+");
    if(regExp.exactMatch(s)) {
        return true;
    }
    return false;
}

void Calc::makeSyntaxNode(token s, vector<Exp*>& node)
{

}


void Calc::connectSyntaxNode(token s, vector<Exp*>& node)
{

}
