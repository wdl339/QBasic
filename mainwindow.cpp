#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnClearCode, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ui->btnLoadCode, SIGNAL(clicked()), this, SLOT(loadFile()));
    connect(ui->cmdLineEdit, SIGNAL(returnPressed()), this, SLOT(codeLineEdit_return()));
}

MainWindow::~MainWindow()
{
    delete ui;
    vector<Statement*>::iterator p;
    for(p = stmt.begin(); p != stmt.end(); p++) {
        delete (*p);
    }
    vector<VarExp*>::iterator p2;
    for(p2 = var.begin(); p2 != var.end(); p2++) {
        delete (*p2);
    }
}

void MainWindow::clearAll()
{
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
    stmt.clear();
}

bool cmp(Statement* a, Statement* b)
{
    return a->lineNum < b->lineNum;
}

void MainWindow::codeLineEdit_return()
{
    try {
        QString input = ui->cmdLineEdit->text();
        ui->cmdLineEdit->clear();
        readCodeLine(input);
        changeCode();
    } catch (QString error) {
        QMessageBox::critical(this, "Error", error);
    }
}

void MainWindow::readCodeLine(QString input)
{
    input = input.simplified();
    if(stringIsNum(input)) {
        deleteCode(input.toInt());
        return;
    }
    QString first = input.section(' ', 0, 0);
    if(!stringIsNum(first)) {
        dealWithCmd(input);
        return;
    }
    dealWithStmt(first.toInt(), input.section(' ', 1));
}

bool MainWindow::stringIsNum(QString s)
{
    QRegExp regExp("[0-9]+");
    if(regExp.exactMatch(s)) {
        return true;
    }
    return false;
}

void MainWindow::deleteCode(int num)
{
    vector<Statement*>::iterator p;
    for(p = stmt.begin(); p != stmt.end(); p++) {
        if((*p)->lineNum == num) {
            delete (*p);
            stmt.erase(p);
            break;
        }
    }
}

void MainWindow::dealWithCmd(QString s)
{
    if (s == "RUN") {

    } else if (s == "LOAD") {
        loadFile();
    } else if (s == "LIST") {
        // 啥也不干
    } else if (s == "CLEAR") {
        clearAll();
    } else if (s == "HELP") {

    } else if (s == "QUIT") {
        exit(0);
    } else if (s == "LET") {

    } else if (s == "PRINT") {

    } else if (s == "INPUT") {

    } else {
        throw QString("非法输入！");
    }
}

void MainWindow::dealWithStmt(int num, QString ss)
{
    QString first = ss.section(' ', 0, 0);
    QString second = ss.section(' ', 1, 1);
    if (first == "REM") {
        RemStmt* s = new RemStmt(num, ss.section(' ', 1));
        pushStmt(s);
    } else if (first == "LET") {

    } else if (first == "PRINT") {

    } else if (first == "INPUT" && second != "" && ss.section(' ', 2) == "") {
        checkVaildName(second);
        InputStmt* s = new InputStmt(num, second);
        pushStmt(s);
    } else if (first == "GOTO" && stringIsNum(second) && ss.section(' ', 2) == "") {
        GotoStmt* s = new GotoStmt(num, second.toInt());
        pushStmt(s);
    } else if (first == "IF") {

    } else if (first == "END" && ss.section(' ', 1) == "") {
        EndStmt* s = new EndStmt(num);
        pushStmt(s);
    } else {
        throw QString("非法输入！");
    }
}

void MainWindow::checkVaildName(QString s)
{
    QRegExp regExp("^[a-zA-Z_][a-zA-Z0-9_]*$");
    if (regExp.exactMatch(s)) {
        QString sMaintain[13] = {"LET", "REM", "PRINT", "INPUT", "GOTO", "IF", "END",
                                 "RUN", "LOAD", "LIST", "CLEAR", "HELP", "QUIT"
                                };
        for(int i = 0; i < 13; i++) {
            if(s == sMaintain[i]) {
                throw QString("非法变量名！");
            }
        }
        return;
    }
    throw QString("非法变量名！");
}

void MainWindow::pushStmt(Statement* s)
{
    vector<Statement*>::iterator p;
    for(p = stmt.begin(); p != stmt.end(); p++) {
        if(s->lineNum == (*p)->lineNum) {
            delete (*p);
            *p = s;
            break;
        }
    }
    if (p == stmt.end()) {
        stmt.push_back(s);
        sort(stmt.begin(), stmt.end(), cmp);
    }
}

void MainWindow::changeCode()
{
    vector<Statement*>::iterator p;
    ui->CodeDisplay->clear();
    for(p = stmt.begin(); p != stmt.end(); p++) {
        ui->CodeDisplay->append(((*p)->showStr()));
    }
}

void MainWindow::loadFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open a file."), "../");
    if (!path.isEmpty()) {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            clearAll();
            while (!in.atEnd()) {
                QString line = in.readLine();
                ui->cmdLineEdit->setText(line);
                codeLineEdit_return();
            }
            file.close();
        }
    }
}

//void MainWindow::on_cmdLineEdit_editingFinished()
//{
//    QString cmd = ui->cmdLineEdit->text();
//    ui->cmdLineEdit->setText("");

//    ui->CodeDisplay->append(cmd);
//}

