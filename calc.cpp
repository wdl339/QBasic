#include "calc.h"

extern bool stringIsPosNum(QString s);

//
// Calc类的构造函数，以QString类字符串为参数
//
Calc::Calc(QString ss)
{
    // 将QString类传化为char*类
    string str = ss.toStdString();
    expression = new char[str.length() + 1];
    strcpy(expression, str.c_str());
    lastToken = OPAREN;
}

//
// enum token {OPAERN, ADD, SUB, MUL, DIV, MOD, POW, CPAREN, VALUE, EOL};
// makeSyntaxTree()根据expression构造表达式树，并返回树的根节点
//
Exp* Calc::makeSyntaxTree()
{
    token lastOp, topOp;
    Exp* currentValue;
    // 双栈法转换
    vector<token> op;
    vector<Exp*> node;
    char* str = expression;

    // 按照优先级顺序，对读入内容操作
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
                if(topOp != OPAREN)
                    throw QString("非法表达式（缺括号）");
                else
                    op.pop_back();
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
                while(!op.empty() && (topOp = op.back()) != OPAREN) {
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

//
// enum token {OPAERN, ADD, SUB, MUL, DIV, MOD, POW, CPAREN, VALUE, EOL};
// connectSyntaxNode()以token类t和Exp*的vector栈为参数
// 从栈中弹出两个EXp，根据token构造CompoundExp，并将其压入栈中
//
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

    QString name = "";
    switch(t) {
        case ADD:
            name = "+";
            break;
        case SUB:
            name = "-";
            break;
        case MUL:
            name = "*";
            break;
        case DIV:
            name = "/";
            break;
        case MOD:
            name = "MOD";
            break;
        case POW:
            name = "**";
            break;
        default:
            throw QString("非法表达式");
            break;
    }
    CompoundExp* res = new CompoundExp(name, 0, pLeft, pRight);
    node.push_back(res);
}

//
// enum token {OPAREN, ADD, SUB, MUL, DIV, MOD, POW, CPAREN, VALUE, EOL};
// getOp以一个表达式类指针value的引用为参数，对expression读入下一小段内容，返回这段内容是什么token
// 并且根据这段内容构造value
// 变量和数字都算是VALUE
//
Calc::token Calc::getOp(Exp*& value)
{
    while(*expression && *expression == ' ')
        ++expression;

    if(*expression == '\0') return EOL;

    token t = checkVarOrVal(value);
    if(t == VALUE || t == MOD)
        return t;

    // 对于符号的处理
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
            return dealWithSub(value);
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

    // 有输入非法内容
    throw QString("非法表达式");
}

//
// checkVarOrVal以一个表达式类指针value的引用为参数，对expression读入下一小段内容，检查是不是数字或变量
// 如果是，构造value并返回true
//
Calc::token Calc::checkVarOrVal(Exp*& value)
{
    // 读到变量或MOD
    if(isLetter(*expression) || *expression == '_') {
        QString s = "";
        while(isLetter(*expression) || isNum(*expression) || *expression == '_') {
            s += (*expression);
            ++expression;
        }
        if(s == "MOD") {
            return MOD;
        }
        value = new VarExp(s);
        return VALUE;
    }

    // 读到数字
    if(isNum(*expression)) {
        int val = 0;
        while(isNum(*expression)) {
            val = val * 10 + *expression - '0';
            ++expression;
        }
        value = new ConstExp(val);
        return VALUE;
    }

    return EOL;
}

//
// checkVarOrVal以一个表达式类指针value的引用为参数，判断‘-’是负号还是减号，构造value，并返回VALUE或SUB
//
Calc::token Calc::dealWithSub(Exp*& value)
{
    if(lastToken == OPAREN) {
        // 是负号
        QString exp;
        ++expression;
        vector<token> op;
        // 读入后面所有在这个负号影响范围内的表达式
        while(*expression && !((*expression == '+' || *expression == '-' || *expression == ')') && op.empty())) {
            if(*expression == '(')
                op.push_back(OPAREN);
            if(*expression == ')')
                op.pop_back();
            exp += (*expression);
            ++expression;
        }
        exp = exp.simplified();
        if(stringIsPosNum(exp)) {  // 负数情况
            value = new ConstExp(-exp.toInt());
        } else {  // 负表达式情况，递归使用Calc处理负号后的表达式
            Calc* calc = new Calc(exp);
            value = new CompoundExp("-", 0, calc->makeSyntaxTree());
        }
        return VALUE;
    } else {
        ++expression;
        return SUB;
    }
}

//
// isLetter以字符c为参数，返回它是否是一个字母
//
bool Calc::isLetter(char c)
{
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return true;
    }
    return false;
}

//
// isNum以字符c为参数，返回它是否是一个数字
//
bool Calc::isNum(char c)
{
    if(c >= '0' && c <= '9') {
        return true;
    }
    return false;
}
