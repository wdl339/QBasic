#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <statement.h>
#include <vector>
#include <queue>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
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
    map<int, QString> stmt;
    map<QString, int> varTable;
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void readStrLine(QString input);
    bool stringIsPosNum(QString s);
    void dealWithCmd(QString s);
    void changeCodeDisplay();

    void runCodeLine(Statement* s, int& nextLineNum);
    void buildSyntaxTree(int num, QString ss, map<int, Statement*>& code);
    void showSyntaxTree(Statement* s);
    QString getTreeNode(Statement* s);

private slots:
    //    void on_cmdLineEdit_editingFinished();
    void clearAll();
    void loadFile();
    void codeLineEdit_return();
    void runCode();



private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
