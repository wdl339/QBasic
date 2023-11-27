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
    connect(ui->btnRunCode, SIGNAL(returnPressed()), this, SLOT(runCode()));
}

MainWindow::~MainWindow()
{
    delete ui;
    vector<VarExp*>::iterator p;
    for(p = var.begin(); p != var.end(); p++) {
        delete (*p);
    }
}

void MainWindow::clearAll()
{
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
    stmt.clear();
}

void MainWindow::codeLineEdit_return()
{
    try {
        QString input = ui->cmdLineEdit->text();
        ui->cmdLineEdit->clear();
        readStrLine(input);
        changeCodeDisplay();
    } catch (QString error) {
        QMessageBox::critical(this, "Error", error);
    }
}

void MainWindow::readStrLine(QString input)
{
    if(stringIsPosNum(input)) {
        deleteStrLine(input.toInt());
        return;
    }
    QString first = input.section(' ', 0, 0);
    if(!stringIsPosNum(first)) {
        dealWithCmd(input);
        return;
    }
    int num = first.toInt();
    if(num >= 1000000) {
        throw QString("行号错误");
    }
    StmtStr s;
    s.s = input;
    s.lineNum = num;
    pushStmt(s);
}

bool MainWindow::stringIsPosNum(QString s)
{
    QRegExp regExp("[0-9]+");
    if(regExp.exactMatch(s)) {
        return true;
    }
    return false;
}

void MainWindow::deleteStrLine(int num)
{
    vector<StmtStr>::iterator p;
    for(p = stmt.begin(); p != stmt.end(); p++) {
        if(p->lineNum == num) {
            stmt.erase(p);
            break;
        }
    }
}

void MainWindow::dealWithCmd(QString s)
{
    if (s == "RUN") {
        runCode();
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
        throw QString("非法指令！");
    }
}


void MainWindow::pushStmt(StmtStr s)
{
    vector<StmtStr>::iterator p;
    for(p = stmt.begin(); p != stmt.end(); p++) {
        if(s.lineNum == p->lineNum) {
            *p = s;
            break;
        }
    }
    if (p == stmt.end()) {
        stmt.push_back(s);
        sort(stmt.begin(), stmt.end(), cmp);
    }
}

bool MainWindow::cmp(StmtStr a, StmtStr b)
{
    return a.lineNum < b.lineNum;
}

void MainWindow::changeCodeDisplay()
{
    vector<StmtStr>::iterator p;
    ui->CodeDisplay->clear();
    for(p = stmt.begin(); p != stmt.end(); p++) {
        ui->CodeDisplay->append((*p).s);
    }
}

void MainWindow::runCode()
{
    vector<Statement*> stmtCode;
    vector<StmtStr>::iterator p;
    for(p = stmt.begin(); p != stmt.end(); p++) {
        runCodeLine(p->lineNum, (p->s).section(' ', 1));
    }
}

void MainWindow::runCodeLine(int num, QString ss)
{
    QString first = ss.section(' ', 0, 0);
    QString second = ss.section(' ', 1, 1);
    if (first == "REM") {
        RemStmt* s = new RemStmt(num, ss.section(' ', 1));
        s->run();
    } else if (first == "LET") {

    } else if (first == "PRINT" && second != "") {
        PrintStmt* s = new PrintStmt(num, ss.section(' ', 1));
        s->run();
    } else if (first == "INPUT" && second != "" && ss.section(' ', 2) == "") {
        checkVaildName(second);
        InputStmt* s = new InputStmt(num, second);
        s->run();
    } else if (first == "GOTO" && stringIsPosNum(second) && ss.section(' ', 2) == "") {
        GotoStmt* s = new GotoStmt(num, second.toInt());
        s->run();
    } else if (first == "IF") {

    } else if (first == "END" && ss.section(' ', 1) == "") {
        EndStmt* s = new EndStmt(num);
        s->run();
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

