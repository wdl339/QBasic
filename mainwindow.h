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
    struct StmtStr {
        QString s;
        int lineNum;
    };
    vector<StmtStr> stmt;
    vector<VarExp*> var;
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void readStrLine(QString input);
    bool stringIsPosNum(QString s);
    void deleteStrLine(int num);
    void dealWithCmd(QString s);
    void pushStmt(StmtStr s);
    static bool cmp(StmtStr a, StmtStr b);
    void changeCodeDisplay();

    void runCodeLine(int num, QString s);
    void showSyntaxTree(Statement* s);

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
