#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <QMainWindow>

bool stringIsPosNum(QString& s); // 判断输入的string是否表示一个正数
QString firstPart(QString& input);
QString secondPart(QString& input);
QString afterFirstPart(QString input);
QString afterThirdPart(QString& input);

bool isLet(QString& input);
bool isPrint(QString& input);
bool isInput(QString& input);
bool isGoto(QString& input);
bool isEnd(QString& input);


#endif // PARSER_H
