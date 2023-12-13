#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <QMainWindow>

bool stringIsPosNum(QString& s); // 判断输入的string是否表示一个正数
// 以下四个都是QString按空格分段，返回其中一部分
QString firstPart(QString& input);
QString secondPart(QString& input);
QString afterFirstPart(QString input);
QString afterThirdPart(QString& input);

// 以下函数都是用来判断是不是某类语句
bool isLet(QString& input);
bool isPrint(QString& input);
bool isInput(QString& input);
bool isGoto(QString& input);
bool isEnd(QString& input);


#endif // PARSER_H
