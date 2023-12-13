#include "exp.h"

//
// 基类构造，以QString字符串和int数为参数
//
Exp::Exp(QString n, int v)
{
    name = n;
    val = v;
};

//
// 析构
//
Exp::~Exp()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

//
// 构造，以变量名和int数（为0）为参数
//
VarExp::VarExp(QString n, int v): Exp(n, v)
{
    checkVaildName(n);
}

//
// 构造，以int数为参数，name也是这个数
//
ConstExp::ConstExp(int v): Exp(QString::number(v), v)
{

}

//
// 构造，以字符串和int数（为0）为参数
//
StringExp::StringExp(QString n, int v): Exp(n, v)
{

}

//
// 构造，以符号名、int数（为0）和左右Exp指针为参数
//
CompoundExp::CompoundExp(QString n, int v, Exp* p1, Exp* p2):
    Exp(n, v)
{
    //左右子节点
    child.push_back(p1);
    child.push_back(p2);
}

//
// 基类Exp的eval()以变量表为参数，默认是返回val
//
int Exp::eval(map<QString, VarState>& varTable)
{
    return val;
}

//
// 变量Exp的eval()以变量表为参数，在变量表中找到变量并返回其值
//
int VarExp::eval(map<QString, VarState>& varTable)
{
    if (varTable.find(name) == varTable.end()) {
        throw QString("不存在的变量");
    }
    val = varTable[name].value();
    return val;
}

//
// 组合Exp的eval()以变量表为参数，根据name（符号名）进行相应计算，返回计算值
//
int CompoundExp::eval(map<QString, VarState>& varTable)
{
    int num1 = child[0]->eval(varTable);
    int res = 0;
    int num2;
    if (name == "+") res = num1 + child[1]->eval(varTable);
    if (name == "-") {
        // 如果是负表达式而不是减号，child里只有一个子节点
        if(child[1]) {
            res = num1 - child[1]->eval(varTable);
        } else {
            res = -num1;
        }
    }
    if (name == "*") res = num1 * child[1]->eval(varTable);
    if (name == "/") {
        int dividend = child[1]->eval(varTable);
        if (dividend == 0) throw QString("除0错误");
        res = num1 / dividend;
    }
    if (name == "MOD") {
        num2 = child[1]->eval(varTable);
        res = num1 % num2;
        if(res * num2 < 0)
            res += num2;
    }
    if (name == "**") {
        res = 1;
        num2 = child[1]->eval(varTable);
        if (num2 < 0) {
            throw QString("不支持指数为负数");
        }
        for(int i = 0; i < num2; i++) {
            res *= num1;
        }
    }
    val = res;
    return res;
}

//
// checkVaildName以字符串为参数，检查变量名是否合法
//
void VarExp::checkVaildName(QString s)
{
    // 合法变量名的格式
    QRegExp regExp("^[a-zA-Z_][a-zA-Z0-9_]*$");
    if (regExp.exactMatch(s)) {
        // 不能是保留名
        QString sMaintain[13] = {"LET", "REM", "PRINT", "INPUT", "GOTO", "IF", "END",
                                 "RUN", "LOAD", "LIST", "CLEAR", "HELP", "QUIT"
                                };
        for(int i = 0; i < 13; i++) {
            if(s == sMaintain[i]) {
                throw QString("非法变量名！");
            }
        }
        return;
    }
    throw QString("非法变量名！");
}

// 析构
VarExp::~VarExp()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

// 析构
ConstExp::~ConstExp()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

// 析构
StringExp::~StringExp()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

// 析构
CompoundExp::~CompoundExp()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

