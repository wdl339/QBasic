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
    enum runMode {RUN, WAIT};
    runMode mode;
    map<int, QString> stmt;
    map<QString, int> varTable;
    QString var;
    QEventLoop waitForInput;
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void readStrLine(QString input);
    bool stringIsPosNum(QString s);
    void dealWithCmd(QString s);
    void changeCodeDisplay();
    void startWait(Statement* s);
    void runInput(QString s);

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
