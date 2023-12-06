#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <statement.h>
#include <varstate.h>
#include <parser.h>
#include <vector>
#include <queue>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QEventLoop>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum runMode {RUN, WAIT}; // 程序的运行模式，RUN正常运行，WAIT等待输入
    runMode mode; // 程序的运行模式
    map<int, QString> stmt; // 输入的每一行的语句
    map<QString, VarState> varTable; // 储存变量
    QString varName; // WAIT模式下等待输入的变量的变量名
    QEventLoop waitForInput; // 事件循环，运行的时候等待用户输入，在输入成功时结束
    MainWindow(QWidget* parent = nullptr); // 构造函数
    ~MainWindow(); // 析构函数

    void readStrLine(QString input); // 读进输入的语句
    void dealWithCmd(QString s); // 处理没有行号的命令
    void help(); // 展示help界面
    void changeCodeDisplay(); // 在有新的输入后改变代码展示界面

    void startWait(Statement* s); // 开始等待用户输入
    void runInput(QString s); // 在WAIT模式下处理用户输入

    void runCodeLine(Statement* s, int& nextLineNum, map<int, Statement*>& code); // 一行一行地运行代码
    void buildSyntaxTree(int num, QString input, map<int, Statement*>& code); // 对每个语句构建表达式树
    void showSyntaxTree(Statement* s); // 展示当前的表达式树
    QString getTreeNode(Statement* s); // 获取每个语句表达式树的根节点的展示

private slots:
    //    void on_cmdLineEdit_editingFinished();
    void clearAll(); // 清除当前所有状态
    void loadFile(); // 读入代码文件
    void codeLineEdit_return(); // 按下回车键后的操作
    void runCode(); // 运行代码



private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
