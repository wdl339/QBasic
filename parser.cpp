#include "parser.h"

//
// stringIsPosNum以字符串为参数，返回它是否表示一个正数
//
bool stringIsPosNum(QString s)
{
    QRegExp regExp("[0-9]+");
    if(regExp.exactMatch(s)) {
        return true;
    }
    return false;
}

//
// firstPart把QString按空格分段，返回第一段
//
QString firstPart(QString& input)
{
    return input.section(' ', 0, 0);
}

//
// secondPart把QString按空格分段，返回第二段
//
QString secondPart(QString& input)
{
    return input.section(' ', 1, 1);
}

//
// afterFirstPart把QString按空格分段，返回第一段以后的内容
//
QString afterFirstPart(QString input)
{
    return input.section(' ', 1);
}

//
// afterThirdPart把QString按空格分段，返回第三段以后的内容
//
QString afterThirdPart(QString& input)
{
    return input.section(' ', 3);
}

//
// isLet以字符串为参数，初步判断它是否符合LET语句的语法规范，并返回
//
bool isLet(QString& input)
{
    return (firstPart(input) == "LET" && input.section(' ', 2, 2) == "=");
}

//
// isPrint以字符串为参数，初步判断它是否符合PRINT语句的语法规范，并返回
//
bool isPrint(QString& input)
{
    return (firstPart(input) == "PRINT" && secondPart(input) != "");
}

//
// isInput以字符串为参数，初步判断它是否符合INPUT语句的语法规范，并返回
//
bool isInput(QString& input)
{
    return (firstPart(input) == "INPUT" && secondPart(input) != "" && input.section(' ', 2) == "");
}

//
// isGoto以字符串为参数，初步判断它是否符合GOTO语句的语法规范，并返回
//
bool isGoto(QString& input)
{
    QString second = secondPart(input);
    return (firstPart(input) == "GOTO" && stringIsPosNum(second) && input.section(' ', 2) == "");
}

//
// isEnd以字符串为参数，初步判断它是否符合END语句的语法规范，并返回
//
bool isEnd(QString& input)
{
    return (firstPart(input) == "END" && afterFirstPart(input) == "");
}
