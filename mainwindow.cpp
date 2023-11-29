#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnClearCode, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ui->btnLoadCode, SIGNAL(clicked()), this, SLOT(loadFile()));
    connect(ui->btnRunCode, SIGNAL(clicked()), this, SLOT(runCode()));
    connect(ui->cmdLineEdit, SIGNAL(returnPressed()), this, SLOT(codeLineEdit_return()));
}

MainWindow::~MainWindow()
{
    delete ui;
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
        stmt.erase(input.toInt());
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
    stmt[num] = input;
}

bool MainWindow::stringIsPosNum(QString s)
{
    QRegExp regExp("[0-9]+");
    if(regExp.exactMatch(s)) {
        return true;
    }
    return false;
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

void MainWindow::changeCodeDisplay()
{
    ui->CodeDisplay->clear();
    for (const auto& pair : stmt) {
        ui->CodeDisplay->append(pair.second);
    }
}

void MainWindow::runCode()
{
    try {
        ui->textBrowser->clear();
        ui->treeDisplay->clear();
        map<int, Statement*> code;
        for (const auto& pair : stmt) {
            QString input = pair.second;
            input = input.replace("=", " = ").replace(">", " > ").replace("<", " < ");
            buildSyntaxTree(pair.first, (input.section(' ', 1)).simplified(), code);
        }
        if(!code.empty()) {
            int curLineNum = 0;
            int lastLineNum = (*code.rbegin()).first;
            int nextLineNum = (*code.begin()).first;
            do {
                curLineNum = nextLineNum;
                Statement* s = code[curLineNum];
                if(curLineNum != lastLineNum) {
                    auto it = code.upper_bound(curLineNum);
                    nextLineNum = it->first;
                }
                runCodeLine(s, nextLineNum);
            } while((curLineNum != lastLineNum || nextLineNum != lastLineNum) && nextLineNum != -1);
        }
    } catch (QString error) {
        QMessageBox::critical(this, "Error", error);
    }
}

void MainWindow::buildSyntaxTree(int num, QString ss, map<int, Statement*>& code)
{
    QString first = ss.section(' ', 0, 0);
    QString second = ss.section(' ', 1, 1);
    Statement* s;
    if (first == "REM") {
        s = new RemStmt(num, ss.section(' ', 1));
    } else if (first == "LET" && ss.section(' ', 2, 2) == "=") {
        s = new LetStmt(num, ss.section(' ', 1, 1), ss.section(' ', 3));
    } else if (first == "PRINT" && second != "") {
        s = new PrintStmt(num, ss.section(' ', 1));
    } else if (first == "INPUT" && second != "" && ss.section(' ', 2) == "") {
        s = new InputStmt(num, second);
    } else if (first == "GOTO" && stringIsPosNum(second) && ss.section(' ', 2) == "") {
        s = new GotoStmt(num, second.toInt());
    } else if (first == "IF") {
        s = new IfStmt(num, ss.section(' ', 1));
    } else if (first == "END" && ss.section(' ', 1) == "") {
        s = new EndStmt(num);
    } else {
        throw QString("非法输入！");
    }
    code[num] = s;
    showSyntaxTree(s);
}

void MainWindow::showSyntaxTree(Statement* s)
{
    QString res = getTreeNode(s);
    queue<Exp*> que;
    Exp* tmp;
    int numOfT = 1;
    if(!(s->child).empty()) {
        for (Exp* ch : s->child) {
            que.push(ch);
        }
    }

    while(!que.empty()) {
        int size = que.size();
        for(int j = 0; j < size; j++) {
            res += '\n';
            tmp = que.front();
            que.pop();
            for(int i = 0; i < numOfT; i++) {
                res += "    ";
            }
            res += tmp->name;
            if(!(tmp->child).empty()) que.push(tmp->child[0]);
            if((tmp->child).size() == 2) que.push(tmp->child[1]);
        }
        numOfT += 1;
    }
    ui->treeDisplay->append(res);
}

QString MainWindow::getTreeNode(Statement* s)
{
    QString treeNode = "";
    QString num = QString::number(s->lineNum);
    switch(s->type) {
        case REM:
            treeNode = num + " " + "REM";
            break;
        case LET:
            treeNode = num + " " + "LET =";
            break;
        case PRINT:
            treeNode = num + " " + "PRINT";
            break;
        case INPUT:
            treeNode = num + " " + "INPUT";
            break;
        case GOTO:
            treeNode = num + " " + "GOTO";
            break;
        case IF:
            treeNode = num + " " + "IF THEN";
            break;
        case END:
            treeNode = num + " " + "END";
            break;
    }
    return treeNode;
}

void MainWindow::runCodeLine(Statement* s, int& nextLineNum)
{
    s->run(varTable);
    stmtType type = s->type;
    if(type == INPUT) {
        //待完成
    } else if (type == PRINT) {
        ui->textBrowser->append(QString::number(s->child[0]->val));
    } else if (type == GOTO) {
        nextLineNum = s->child[0]->val;
    } else if (type == IF) {
        if(s->child[1]->val != 0) {
            nextLineNum = s->child[1]->val;
        }
    } else if (type == END) {
        nextLineNum = -1;
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

