#include "program.h"

Program::Program()
{
    varName = "";
    mode = RUN;
}

void Program::clearAll()
{
    stmt.clear();
    varTable.clear();
}

bool Program::isRun()
{
    return mode == RUN;
}

void Program::erase(int num)
{
    stmt.erase(num);
}

void Program::add(int num, QString input)
{
    stmt[num] = input;
}

bool Program::hasVar(QString input)
{
    return varTable.find(input) != varTable.end();
}

int Program::useTimeof(QString var)
{
    if (hasVar(var)) {
        return varTable[var].useTime();
    } else {
        return 0;
    }

}

void Program::startWaitVar(QString var)
{
    mode = WAIT;
    varName = var;
}

void Program::setWaitVar(int val)
{
    if (varTable.find(varName) == varTable.end()) {
        varTable[varName] = VarState(val);
    } else {
        varTable[varName].setValue(val);
    }
    mode = RUN;
}
