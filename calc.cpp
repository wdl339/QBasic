#include "calc.h"

Calc::Calc(QString ss)
{
    string str = ss.toStdString();
    expression = new char[str.length() + 1];
    strcpy(expression, str.c_str());
    lastToken = OPAREN;
}

//enum token {OPAERN, ADD, SUB, MUL, DIV, MOD, POW, CPAREN, VALUE, EOL};
Exp* Calc::makeSyntaxTree()
{
    token lastOp, topOp;
    Exp* currentValue;
    vector<token> op;
    vector<Exp*> node;
    char* str = expression;

    while((lastOp = getOp(currentValue)) != EOL) {
        lastToken = lastOp;
        switch(lastOp) {
            case VALUE:
                node.push_back(currentValue);
                break;
            case OPAREN:
            case POW:
                op.push_back(lastOp);
                break;
            case CPAREN:
                while(!op.empty() && (topOp = op.back()) != OPAREN) {
                    op.pop_back();
                    connectSyntaxNode(topOp, node);
                }
                if(topOp != OPAREN) {
                    throw QString("非法表达式（缺括号）");
                } else {
                    op.pop_back();
                }
                break;
            case MUL:
            case DIV:
            case MOD:
                while(!op.empty() && (topOp = op.back()) >= MUL) {
                    op.pop_back();
                    connectSyntaxNode(topOp, node);
                }
                op.push_back(lastOp);
                break;
            case ADD:
            case SUB:
                while(!op.empty() && (topOp = op.back()) >= MUL) {
                    op.pop_back();
                    connectSyntaxNode(topOp, node);
                }
                op.push_back(lastOp);
                break;
            default:
                break;
        }
    }

    while(!op.empty()) {
        connectSyntaxNode(op.back(), node);
        op.pop_back();
    }
    expression = str;
    if(node.empty() || node.size() > 1) {
        throw QString("非法表达式");
        return nullptr;
    }
    return node.back();

}


//enum token {OPAREN, ADD, SUB, MUL, DIV, MOD, POW, CPAREN, VALUE, EOL};
Calc::token Calc::getOp(Exp*& value)
{
    while(*expression && *expression == ' ')
        ++expression;

    if(*expression == '\0') return EOL;

    if(isLetter(*expression)) {
        QString s = "";
        while(isLetter(*expression) || isNum(*expression)) {
            s += (*expression);
            ++expression;
        }
        if(s == "MOD") {
            return MOD;
        }
        value = new VarExp(s);
        return VALUE;
    }

    if(isNum(*expression)) {
        int val = 0;
        while(isNum(*expression)) {
            val = val * 10 + *expression - '0';
            ++expression;
        }
        value = new ConstExp(val);
        return VALUE;
    }

    switch (*expression) {
        case '(':
            ++expression;
            return OPAREN;
        case ')':
            ++expression;
            return CPAREN;
        case '+':
            ++expression;
            return ADD;
        case '-':
            if(lastToken != VALUE || lastToken != CPAREN) {
                ++expression;
                while(*expression && *expression == ' ')
                    ++expression;
                if(isNum(*expression)) {
                    int val = 0;
                    while(isNum(*expression)) {
                        val = val * 10 + *expression - '0';
                        ++expression;
                    }
                    value = new ConstExp(-val);
                    return VALUE;
                }
            } else {
                ++expression;
                return SUB;
            }
            break;
        case '/':
            ++expression;
            return DIV;
        case '*':
            ++expression;
            if(*expression && *expression == '*') {
                ++expression;
                return POW;
            } else {
                return MUL;
            }
    }

    throw QString("非法表达式");
}

bool Calc::stringIsNum(QString s)
{
    QRegExp regExp("[0-9]+");
    if(regExp.exactMatch(s)) {
        return true;
    }
    return false;
}

bool Calc::isLetter(char c)
{
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return true;
    }
    return false;
}

bool Calc::isNum(char c)
{
    if(c >= '0' && c <= '9') {
        return true;
    }
    return false;
}

//enum token {OPAERN, ADD, SUB, MUL, DIV, MOD, POW, CPAREN, VALUE, EOL};
void Calc::connectSyntaxNode(token t, vector<Exp*>& node)
{
    QString s = "";
    Exp* pLeft, * pRight;
    if(node.empty()) {
        throw QString("非法表达式");
    } else pRight = node.back();
    node.pop_back();
    if(node.empty()) {
        throw QString("非法表达式");
    } else pLeft = node.back();
    node.pop_back();

    IdentifierExp* res = new IdentifierExp("", 0, pLeft, pRight);
    switch(t) {
        case ADD:
            res->name = "+";
            break;
        case SUB:
            res->name = "-";
            break;
        case MUL:
            res->name = "*";
            break;
        case DIV:
            res->name = "/";
            break;
        case MOD:
            res->name = "MOD";
            break;
        case POW:
            res->name = "**";
            break;
        default:
            break;
    }
    node.push_back(res);
}
