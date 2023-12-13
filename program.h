#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

#include <QMainWindow>
#include <varstate.h>
using namespace std;

class MainWindow;

//
// 程序类
//
class Program
{
    friend MainWindow;

    enum runMode {RUN, WAIT}; // 程序的运行模式，RUN正常运行，WAIT等待输入
    runMode mode; // 程序的运行模式

    map<int, QString> stmt; // 输入的每一行的语句
    map<QString, VarState> varTable; // 储存变量的变量表
    QString varName; // WAIT模式下等待输入的变量的变量名
public:
    Program(); // 构造
    ~Program(); // 析构

    void clearUseCount(); // 清空所有变量的use count
    bool isRun(); // 程序模式是否为RUN
    void erase(int num); // 抹除特定行语句
    void add(int num, QString input); // 加入语句
    bool hasVar(QString input); // 是否有名字为input的变量
    int useTimeof(QString var); // 返回变量的use count
    void startWaitVar(QString var); // 开始等待INPUT
    void setWaitVar(int val); // 设置等待的变量

};

#endif // PROGRAM_H
