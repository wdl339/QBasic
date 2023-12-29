#include "statement.h"

extern bool stringIsPosNum(QString s);

//
// 基类构造函数，以num为参数
//
Statement::Statement(int num)
{
    lineNum = num;
    runTime = 0;
}

//
// 语句类的析构函数
//
Statement::~Statement()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

//
// getWaitVarName用于INPUT语句,返回INPUT后的变量名
//
QString Statement::getWaitVarName()
{
    return child[0]->name;
}

//
// getchildVal以num为参数，返回child[num]的值
//
int Statement::getchildVal(int num)
{
    return child[num]->val;
}

//
// getLineNum返回行号
//
int Statement::getLineNum()
{
    return lineNum;
}

//
// REM语句构造以num和字符串为参数，REM内含一个StringExp
//
RemStmt::RemStmt(int num, QString ss): Statement(num)
{
    StringExp* exp = new StringExp(ss);
    child.push_back(exp);
    type = REM;
}

//
// LET语句构造以num、等号前的字符串var和等号后的字符串exp为参数，LET内含一个VarExp和表达式树
//
LetStmt::LetStmt(int num, QString var, QString exp): Statement(num)
{
    VarExp* varExp = new VarExp(var);
    // 根据exp构造表达式树
    Calc* calc = new Calc(exp);
    child.push_back(varExp);
    child.push_back(calc->makeSyntaxTree());
    type = LET;
}

//
// PRINT语句构造以num和字符串为参数，PRINT内含一个表达式树
//
PrintStmt::PrintStmt(int num, QString exp): Statement(num)
{
    Calc* calc = new Calc(exp);
    child.push_back(calc->makeSyntaxTree());
    type = PRINT;
}

//
// INPUT语句构造以num和字符串为参数，PRINT内含一个VarExp
//
InputStmt::InputStmt(int num, QString ss): Statement(num)
{
    VarExp* exp = new VarExp(ss);
    child.push_back(exp);
    type = INPUT;
}

//
// GOTO语句构造以int数num和val为参数，GOTO内含一个ConstExp
//
GotoStmt::GotoStmt(int num, int val): Statement(num)
{
    ConstExp* exp = new ConstExp(val);
    child.push_back(exp);
    type = GOTO;
}

//
// IF语句构造以num和字符串为参数，IF内含一个两个表达式树，
// 一个StringExp（中间的比较符号）和一个ConstExp（去往的行号）
//
IfStmt::IfStmt(int num, QString ss): Statement(num)
{
    int thenIndex = ss.indexOf("THEN");
    if (thenIndex != -1) {
        // 分出THEN前后内容
        QString expBeforeThen = ss.left(thenIndex).trimmed();
        QString expAfterThen = ss.mid(thenIndex + 4).trimmed();

        QRegularExpression regex("(.*)\\s*([<>=])\\s*(.*)");
        QRegularExpressionMatch match = regex.match(expBeforeThen);

        if (match.hasMatch() && stringIsPosNum(expAfterThen)) {
            // match捕获的内容可以进一步细分
            QString exp1 = match.captured(1).trimmed();
            QString op = match.captured(2).trimmed();
            QString exp2 = match.captured(3).trimmed();

            if(op != "=" && op != "<" && op != ">") {
                throw QString("非法op");
            }

            Calc* calc = new Calc(exp1);
            child.push_back(calc->makeSyntaxTree());
            StringExp* com = new StringExp(op);
            child.push_back(com);
            Calc* calc2 = new Calc(exp2);
            child.push_back(calc2->makeSyntaxTree());
            ConstExp* n = new ConstExp(expAfterThen.toInt());
            child.push_back(n);
        } else {
            throw QString("非法语句");
        }
    } else {
        throw QString("非法语句");
    }
    type = IF;
}

//
// END语句构造以num为参数
//
EndStmt::EndStmt(int num): Statement(num)
{
    type = END;
}

//
// ERROR语句构造以num为参数
//
ErrorStmt::ErrorStmt(int num): Statement(num)
{
    type = ERROR;
}

//
// 基类的run以变量表为参数，默认只让runTime+1
//
void Statement::run(map<QString, VarState>& varTable)
{
    runTime += 1;
}

//
// LET语句的run以变量表为参数，对表达式树求值，赋值给VarExp的变量
//
void LetStmt::run(map<QString, VarState>& varTable)
{
    int res = child[1]->eval(varTable);
    QString varName = child[0]->name;
    // 赋新变量的值或重新赋值
    if (varTable.find(varName) == varTable.end()) {
        varTable[varName] = VarState(res);
    } else {
        varTable[varName].setValue(res);
    }
    child[0]->val = res;
    runTime += 1;
}

//
// PRINT语句的run以变量表为参数，对表达式树求值
//
void PrintStmt::run(map<QString, VarState>& varTable)
{
    child[0]->eval(varTable);
    runTime += 1;
}

//
// IF语句的run以变量表为参数，对两个表达式树求值，根据StringExp的符号进行比较
//
void IfStmt::run(map<QString, VarState>& varTable)
{
    child[0]->eval(varTable);
    child[2]->eval(varTable);
    bool flag = false;
    if(child[1]->name == "=") {
        if(child[0]->val == child[2]->val) {
            flag = true;
        }
    } else if (child[1]->name == ">") {
        if(child[0]->val > child[2]->val) {
            flag = true;
        }
    } else if (child[1]->name == "<") {
        if(child[0]->val < child[2]->val) {
            flag = true;
        }
    }
    // StringExp的val为0表示不跳转，不为0则表示接下来跳转的行号
    if(flag) {
        child[1]->val = child[3]->val;
        runTime += 1;
    } else {
        child[1]->val = 0;
        runTimeFalse += 1;
    }
}

//
// 基类getRunTime返回runTime的字符串格式
//
QString Statement::getRunTime()
{
    return QString::number(runTime);
}

//
// IF语句返回runTime合并上runTimeFalse的字符串格式
//
QString IfStmt::getRunTime()
{
    return QString::number(runTime) + " " + QString::number(runTimeFalse);
}

//
// syntaxTreeStr以程序类program为参数，返回语句对应的语法树的字符串
//
QString Statement::syntaxTreeStr(Program*& program)
{
    QString res = getTreeNode();
    // 利用队列，输出表达式（树）的内容
    queue<Exp*> que;
    Exp* tmp;
    // 用来保证缩进正确的变量
    int numOfT = 1;
    if(!child.empty()) {
        for (Exp* ch : child) {
            que.push(ch);
        }
    }
    bool flagForLet = false;
    if(type == LET) {
        flagForLet = true;
    }

    while(!que.empty()) {
        int size = que.size();
        for(int j = 0; j < size; j++) {
            res += '\n';
            tmp = que.front();
            que.pop();
            for(int i = 0; i < numOfT; i++) {
                res += "    ";
            }
            res += tmp->name;
            // 对LET语句，第一个变量要输出它的use count
            if (flagForLet) {
                res += (" " + QString::number(program->useTimeof(tmp->name)));
                flagForLet = false;
            }
            if(!(tmp->child).empty()) que.push(tmp->child[0]);
            if((tmp->child).size() == 2 && tmp->child[1]) que.push(tmp->child[1]);
        }
        numOfT += 1;
    }

    return res;
}

//
// getTreeNode返回表达式树根节点的展示字符串，基类默认返回“ ”
//
QString Statement::getTreeNode()
{
    return " ";
}

//
// getTreeNode返回表达式树根节点的展示字符串，有行号，语句类型和runTime信息
//
QString RemStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " REM " + getRunTime();
    return treeNode;
}

//
// getTreeNode返回表达式树根节点的展示字符串，有行号，语句类型和runTime信息
//
QString LetStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " LET = " + getRunTime();
    return treeNode;
}

//
// getTreeNode返回表达式树根节点的展示字符串，有行号，语句类型和runTime信息
//
QString PrintStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " PRINT " + getRunTime();
    return treeNode;
}

//
// getTreeNode返回表达式树根节点的展示字符串，有行号，语句类型和runTime信息
//
QString InputStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " INPUT " + getRunTime();
    return treeNode;
}

//
// getTreeNode返回表达式树根节点的展示字符串，有行号，语句类型和runTime信息
//
QString GotoStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " GOTO " + getRunTime();
    return treeNode;
}

//
// getTreeNode返回表达式树根节点的展示字符串，有行号，语句类型和runTime信息
//
QString IfStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " IF THEN " + getRunTime();
    return treeNode;
}

//
// getTreeNode返回表达式树根节点的展示字符串，有行号，语句类型和runTime信息
//
QString EndStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " END " + getRunTime();
    return treeNode;
}

//
// getTreeNode返回表达式树根节点的展示字符串，有行号，语句类型和runTime信息
//
QString ErrorStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " ERROR";
    return treeNode;
}
