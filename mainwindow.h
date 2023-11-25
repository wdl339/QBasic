#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <statement.h>
#include <vector>
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
    vector<Statement*> stmt;
    vector<VarExp*> var;
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void changeCode();

private slots:
    //    void on_cmdLineEdit_editingFinished();
    void clearAll();
    void loadFile();
    void codeLineEdit_return();
    void readCodeLine(QString input);
    bool stringIsNum(QString s);
    void deleteCode(int num);
    void dealWithCmd(QString s);
    void dealWithStmt(int num, QString s);
    void pushStmt(Statement* s);
    void checkVaildName(QString s);

private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
