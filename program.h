#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

#include <QMainWindow>
#include <varstate.h>
using namespace std;

class MainWindow;

class Program
{
    friend MainWindow;

    enum runMode {RUN, WAIT}; // 程序的运行模式，RUN正常运行，WAIT等待输入
    runMode mode; // 程序的运行模式

    map<int, QString> stmt; // 输入的每一行的语句
    map<QString, VarState> varTable; // 储存变量
    QString varName; // WAIT模式下等待输入的变量的变量名
public:
    Program();
    ~Program() {}

    void clearAll();
    bool isRun();
    void erase(int num);
    void add(int num, QString input);
    bool hasVar(QString input);
    int useTimeof(QString var);
    void startWaitVar(QString var);
    void setWaitVar(int val);

};

#endif // PROGRAM_H
