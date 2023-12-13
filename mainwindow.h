#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <statement.h>
#include <program.h>
#include <parser.h>
#include <vector>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QEventLoop>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
class Program;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Program* program; // 程序
    QEventLoop waitForInput; // 事件循环，运行的时候等待用户输入，在输入成功时结束
public:
    MainWindow(QWidget* parent = nullptr); // 构造函数
    ~MainWindow(); // 析构函数

    void readStrLine(QString input); // 读进输入的语句
    void dealWithCmd(QString s); // 处理没有行号的命令
    void help(); // 展示help界面
    void changeCodeDisplay(); // 在有新的输入后改变代码展示界面

    void startWait(Statement* s); // 开始等待用户输入
    void runInput(QString s); // 在WAIT模式下处理用户输入

    void runCodeLine(Statement* s, int& nextLineNum, map<int, Statement*>& code); // 一行一行地运行代码
    void strToStmt(int num, QString input, map<int, Statement*>& code); // 将input转化为Statement放入code中
    void showSyntaxTree(Statement* s); // 展示当前的表达式树

private slots:
    void clearAll(); // 清除当前所有状态
    void loadFile(); // 读入代码文件
    void codeLineEdit_return(); // 按下回车键后的操作
    void runCode(); // 运行代码

private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
