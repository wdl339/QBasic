#include "statement.h"

extern bool stringIsPosNum(QString& s);

Statement::Statement(int num)
{
    lineNum = num;
}

Statement::~Statement()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

void Statement::run(map<QString, VarState>& varTable)
{
    runTime += 1;
}

QString Statement::getRunTime()
{
    return QString::number(runTime);
}

QString Statement::getWaitVarName()
{
    return child[0]->name;
}

int Statement::getchildVal(int num)
{
    return child[num]->val;
}

int Statement::getLineNum()
{
    return lineNum;
}

RemStmt::RemStmt(int num, QString ss): Statement(num)
{
    StringExp* exp = new StringExp(ss);
    child.push_back(exp);
    type = REM;
}

LetStmt::LetStmt(int num, QString var, QString exp): Statement(num)
{
    VarExp* varExp = new VarExp(var);
    Calc* calc = new Calc(exp);
    child.push_back(varExp);
    child.push_back(calc->makeSyntaxTree());
    type = LET;
}

PrintStmt::PrintStmt(int num, QString ss): Statement(num)
{
    Calc* calc = new Calc(ss);
    child.push_back(calc->makeSyntaxTree());
    type = PRINT;
}

InputStmt::InputStmt(int num, QString ss): Statement(num)
{
    VarExp* exp = new VarExp(ss);
    child.push_back(exp);
    type = INPUT;
}

GotoStmt::GotoStmt(int num, int val): Statement(num)
{
    ConstExp* exp = new ConstExp(val);
    child.push_back(exp);
    type = GOTO;
}

IfStmt::IfStmt(int num, QString ss): Statement(num)
{
    int thenIndex = ss.indexOf("THEN");
    if (thenIndex != -1) {
        QString expBeforeThen = ss.left(thenIndex).trimmed();
        QString expAfterThen = ss.mid(thenIndex + 4).trimmed();

        QRegularExpression regex("(.*)\\s*([<>=])\\s*(.*)");
        QRegularExpressionMatch match = regex.match(expBeforeThen);

        if (match.hasMatch() && stringIsPosNum(expAfterThen)) {
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

EndStmt::EndStmt(int num): Statement(num)
{
    type = END;
}

ErrorStmt::ErrorStmt(int num): Statement(num)
{
    type = ERROR;
}

void LetStmt::run(map<QString, VarState>& varTable)
{
    int res = child[1]->eval(varTable);
    QString varName = child[0]->name;
    if (varTable.find(varName) == varTable.end()) {
        varTable[varName] = VarState(res);
    } else {
        varTable[varName].setValue(res);
    }
    child[0]->val = res;
    runTime += 1;
}

void PrintStmt::run(map<QString, VarState>& varTable)
{
    child[0]->eval(varTable);
    runTime += 1;
}

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
    if(flag) {
        child[1]->val = child[3]->val;
        runTime += 1;
    } else {
        child[1]->val = 0;
        runTimeFalse += 1;
    }
}

QString IfStmt::getRunTime()
{
    return QString::number(runTime) + " " + QString::number(runTimeFalse);
}

QString Statement::syntaxTreeStr(Program*& program)
{
    QString res = getTreeNode();
    queue<Exp*> que;
    Exp* tmp;
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

QString Statement::getTreeNode()
{
    return " ";
}

QString RemStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " REM " + getRunTime();
    return treeNode;
}

QString LetStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " LET = " + getRunTime();
    return treeNode;
}

QString PrintStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " PRINT " + getRunTime();
    return treeNode;
}

QString InputStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " INPUT " + getRunTime();
    return treeNode;
}

QString GotoStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " GOTO " + getRunTime();
    return treeNode;
}

QString IfStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " IF THEN " + getRunTime();
    return treeNode;
}

QString EndStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " END " + getRunTime();
    return treeNode;
}

QString ErrorStmt::getTreeNode()
{
    QString num = QString::number(lineNum);
    QString treeNode = num + " ERROR";
    return treeNode;
}
