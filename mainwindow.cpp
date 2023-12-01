#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    mode = RUN;
    var = "";
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
    varTable.clear();
}

void MainWindow::codeLineEdit_return()
{
    try {
        QString input = ui->cmdLineEdit->text();
        ui->cmdLineEdit->clear();
        if(mode == RUN) {
            readStrLine(input);
            changeCodeDisplay();
        } else if(mode == WAIT) {
            runInput(input);
        }
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

void MainWindow::dealWithCmd(QString input)
{
    if (input == "RUN") {
        runCode();
    } else if (input == "LOAD") {
        loadFile();
    } else if (input == "LIST") {
        // 啥也不干
    } else if (input == "CLEAR") {
        clearAll();
    } else if (input == "HELP") {
        // 待完成
    } else if (input == "QUIT") {
        exit(0);
    } else {
        input = input.simplified();
        QString first = input.section(' ', 0, 0);
        QString second = input.section(' ', 1, 1);
        if(first == "LET" && input.section(' ', 2, 2) == "=") {
            input = input.replace("=", " = ").simplified();
            LetStmt* s = new LetStmt(0, second, input.section(' ', 3));
            s->run(varTable);
        } else if(first == "PRINT" && second != "") {
            PrintStmt* s = new PrintStmt(0, input.section(' ', 1));
            s->run(varTable);
            ui->textBrowser->append(QString::number(s->getchildVal(0)));
        } else if(first == "INPUT" && second != "" && input.section(' ', 2) == "") {
            InputStmt* s = new InputStmt(0, input.section(' ', 1));
            startWait(s);
        } else {
            throw QString("非法指令！");
        }
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
                ui->treeDisplay->clear();
                for (const auto& pair : code) {
                    showSyntaxTree(pair.second);
                }
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
    QString num = QString::number(s->lineNum);
    QString treeNode = num + " ";
    switch(s->type) {
        case REM:
            treeNode += ("REM " + s->getRunTime());
            break;
        case LET:
            treeNode += ("LET = " + s->getRunTime());
            break;
        case PRINT:
            treeNode += ("PRINT " + s->getRunTime());
            break;
        case INPUT:
            treeNode += ("INPUT " + s->getRunTime());
            break;
        case GOTO:
            treeNode += ("GOTO " + s->getRunTime());
            break;
        case IF:
            treeNode += ("IF THEN " + s->getRunTime());
            break;
        case END:
            treeNode += ("END " + s->getRunTime());
            break;
    }
    return treeNode;
}

void MainWindow::runCodeLine(Statement* s, int& nextLineNum)
{
    s->run(varTable);
    stmtType type = s->type;
    if(type == INPUT) {
        startWait(s);
    } else if (type == PRINT) {
        ui->textBrowser->append(QString::number(s->getchildVal(0)));
    } else if (type == GOTO) {
        nextLineNum = s->getchildVal(0);
    } else if (type == IF) {
        if(s->getchildVal(1) != 0) {
            nextLineNum = s->getchildVal(1);
        }
    } else if (type == END) {
        nextLineNum = -1;
    }
}

void MainWindow::startWait(Statement* s)
{
    ui->cmdLineEdit->clear();
    ui->cmdLineEdit->setText("? ");
    mode = WAIT;
    var = s->getChildName();
    waitForInput.exec();
}

void MainWindow::runInput(QString input)
{
    if(input[0] == "?") {
        input = input.simplified().section(" ", 1);
    }
    input = input.replace("-", "- ").simplified();
    int pos = 1;
    if(input[0] == "-") {
        pos = -1;
        input = input.section(" ", 1);
    }
    if(stringIsPosNum(input)) {
        varTable[var] = pos * input.toInt();
        ui->cmdLineEdit->clear();
        mode = RUN;
        waitForInput.quit();
    } else {
        ui->cmdLineEdit->setText("? ");
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

