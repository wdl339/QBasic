#include "parser.h"

bool stringIsPosNum(QString& s)
{
    QRegExp regExp("[0-9]+");
    if(regExp.exactMatch(s)) {
        return true;
    }
    return false;
}

QString firstPart(QString& input)
{
    return input.section(' ', 0, 0);
}

QString secondPart(QString& input)
{
    return input.section(' ', 1, 1);
}

QString afterFirstPart(QString input)
{
    return input.section(' ', 1);
}

QString afterThirdPart(QString& input)
{
    return input.section(' ', 3);
}

bool isLet(QString& input)
{
    return (firstPart(input) == "LET" && input.section(' ', 2, 2) == "=");
}

bool isPrint(QString& input)
{
    return (firstPart(input) == "PRINT" && secondPart(input) != "");
}

bool isInput(QString& input)
{
    return (firstPart(input) == "INPUT" && secondPart(input) != "" && input.section(' ', 2) == "");
}

bool isGoto(QString& input)
{
    QString second = secondPart(input);
    return (firstPart(input) == "GOTO" && stringIsPosNum(second) && input.section(' ', 2) == "");
}

bool isEnd(QString& input)
{
    return (firstPart(input) == "END" && afterFirstPart(input) == "");
}
