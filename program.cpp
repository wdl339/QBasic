#include "program.h"

//
// 构造
//
Program::Program()
{
    varName = "";
    mode = RUN;
}

//
// 析构
//
Program::~Program()
{
    stmt.clear();
    varTable.clear();
}

//
// clearUseCount清空所有变量的use count
//
void Program::clearUseCount()
{
    for (auto& pair : varTable) {
        (pair.second).clearTime();
    }
}

//
// isRun返回程序模式是否为RUN
//
bool Program::isRun()
{
    return mode == RUN;
}

//
// erase以num为参数，抹除第num行语句
//
void Program::erase(int num)
{
    stmt.erase(num);
}

//
// add以num和字符串input为参数，加入第num行语句
//
void Program::add(int num, QString input)
{
    stmt[num] = input;
}

//
// hasVar返回是否有名字为input的变量
//
bool Program::hasVar(QString input)
{
    return varTable.find(input) != varTable.end();
}

//
// useTimeof以变量名var为参数，返回变量的use count
//
int Program::useTimeof(QString var)
{
    if (hasVar(var)) {
        return varTable[var].useTime();
    } else {
        return 0;
    }
}

//
// startWaitVar以变量名var为参数，做INPUT的等待准备
//
void Program::startWaitVar(QString var)
{
    mode = WAIT;
    varName = var;
}

//
// setWaitVar以val为参数，将等待中的变量的值设为val，并解除等待
//
void Program::setWaitVar(int val)
{
    if (varTable.find(varName) == varTable.end()) {
        varTable[varName] = VarState(val);
    } else {
        varTable[varName].setValue(val);
    }
    mode = RUN;
}
