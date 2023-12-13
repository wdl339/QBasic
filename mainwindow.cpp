#include "mainwindow.h"
#include "ui_mainwindow.h"

//
// 主界面的构造函数
//
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    program = new Program();
    ui->setupUi(this);
    connect(ui->btnClearCode, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ui->btnLoadCode, SIGNAL(clicked()), this, SLOT(loadFile()));
    connect(ui->btnRunCode, SIGNAL(clicked()), this, SLOT(runCode()));
    connect(ui->cmdLineEdit, SIGNAL(returnPressed()), this, SLOT(codeLineEdit_return()));
}

//
// 主界面的析构函数
//
MainWindow::~MainWindow()
{
    delete ui;
    delete program;
}

//
// clearAll()清除当前所有状态
//
void MainWindow::clearAll()
{
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
    // 程序状态也重置
    delete program;
    program = new Program();
}

//
// codeLineEdit_return()处理按下回车键后的操作
//
void MainWindow::codeLineEdit_return()
{
    try {
        QString input = ui->cmdLineEdit->text();
        ui->cmdLineEdit->clear();
        // 在RUN模式下，读进输入的代码，并改变display
        if(program->isRun()) {
            readStrLine(input);
            changeCodeDisplay();
            // 在WAIT模式下，处理用户输入的INPUT的值
        } else {
            runInput(input);
        }
    } catch (QString error) {
        QMessageBox::critical(this, "Error", error);
    }
}

//
// readStrLine以字符串input为参数，处理用户输入的input
//
void MainWindow::readStrLine(QString input)
{
    // 假如只输入数字，表明删除一行代码
    if(stringIsPosNum(input)) {
        program->erase(input.toInt());
        return;
    }
    // 对输入按空格分段，如果输入的第一段不是行号，说明输入了指令
    QString first = firstPart(input);
    if(!stringIsPosNum(first)) {
        dealWithCmd(input.simplified());
        return;
    }
    int num = first.toInt();
    if(num >= 1000000) {
        throw QString("行号错误");
    }
    // 有行号，program储存输入的语句
    program->add(num, input);
}

//
// dealWithCmd以字符串input为参数，处理input代表的指令
//
void MainWindow::dealWithCmd(QString input)
{
    // 常见指令*6
    if (input == "RUN") {
        runCode();
    } else if (input == "LOAD") {
        loadFile();
    } else if (input == "LIST") {
        // LIST啥也不干
    } else if (input == "CLEAR") {
        clearAll();
    } else if (input == "HELP") {
        help();
    } else if (input == "QUIT") {
        exit(0);
    } else {
        // 到此，可能是不带行号的语句*3
        if(isLet(input)) {
            // 这样操作是防止等号前后不加空格的情况，后面也有类似语句
            input = input.replace("=", " = ").simplified();
            LetStmt* s = new LetStmt(0, secondPart(input), afterThirdPart(input));
            s->run(program->varTable);
        } else if(isPrint(input)) {
            PrintStmt* s = new PrintStmt(0, afterFirstPart(input));
            s->run(program->varTable);
            // 在ui上打印出
            ui->textBrowser->append(QString::number(s->getchildVal(0)));
        } else if(isInput(input)) {
            InputStmt* s = new InputStmt(0, afterFirstPart(input));
            startWait(s);
        } else {
            // 处理非法指令
            throw QString("非法指令");
        }
    }
}

//
// help()弹出help界面
//
void MainWindow::help()
{
    QString s = "这是一个Basic解释器\n"
                "实现了LET PRINT INPUT GOTO IF REM END语句\n"
                "还有 RUN LOAD CLEAR HELP QUIT指令\n"
                "支持+ - * / MOD **运算，仅限于int类型\n"
                "IF语句支持= > <的比较\n"
                "大概就说这么多吧";
    QMessageBox mess(QMessageBox::NoIcon, tr("HELP"), s);
    QPushButton* button1 = (mess.addButton(tr("OK"), QMessageBox::ActionRole));
    mess.setDefaultButton(button1);
    mess.setStyleSheet("QLabel{min-width: 600px; min-height: 300px; font-size: 24px;}");
    mess.exec();
    if (mess.clickedButton() == button1)
        return;
}

//
// changeCodeDisplay()在每次输入新语句后，重新显示代码
//
void MainWindow::changeCodeDisplay()
{
    ui->CodeDisplay->clear();
    // map类型的stmt会自动将语句排好序
    for (const auto& pair : program->stmt) {
        ui->CodeDisplay->append(pair.second);
    }
}

//
// runCode正式运行代码
//
void MainWindow::runCode()
{
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
    program->clearUseCount();
    try {
        map<int, Statement*> code;
        // 做map<int, QString>到map<int, Statement*>的转化
        for (const auto& pair : program->stmt) {
            QString input = pair.second;
            input = input.replace("=", " = ").replace(">", " > ").replace("<", " < ");
            strToStmt(pair.first, (afterFirstPart(input)).simplified(), code);
        }
        if(!code.empty()) {
            int curLineNum = 0;
            int lastLineNum = (*code.rbegin()).first;
            int nextLineNum = (*code.begin()).first;
            do {
                curLineNum = nextLineNum;
                Statement* s = code[curLineNum];
                // 如果当前不是最后一句，那么找下一句
                if(curLineNum != lastLineNum) {
                    auto it = code.upper_bound(curLineNum);
                    nextLineNum = it->first;
                }
                runCodeLine(s, nextLineNum, code);
                ui->treeDisplay->clear();
                for (const auto& pair : code) {
                    showSyntaxTree(pair.second);
                }
            } while((curLineNum != lastLineNum || nextLineNum != lastLineNum) && nextLineNum != -1);
            // 终止条件：nextLineNum为-1意味着遇到END，curLineNum和nextLineNum都不在最后
            // （有可能curLineNum在最后，但最后一句是GOTO到前面的语句，这样也不能退出）
        }
    } catch (QString error) {
        QMessageBox::critical(this, "Error", error);
    }
}

//
// strToStmt以num，字符串input和代码表的引用为参数，将input转化为Statement放入code中
//
void MainWindow::strToStmt(int num, QString input, map<int, Statement*>& code)
{
    QString first = firstPart(input);
    QString second = secondPart(input);
    QString afterFirst = afterFirstPart(input);
    Statement* s;
    try {
        if (first == "REM") {
            s = new RemStmt(num, afterFirst);
        } else if (isLet(input)) {
            s = new LetStmt(num, second, afterThirdPart(input));
        } else if (isPrint(input)) {
            s = new PrintStmt(num, afterFirst);
        } else if (isInput(input)) {
            s = new InputStmt(num, second);
        } else if (isGoto(input)) {
            s = new GotoStmt(num, second.toInt());
        } else if (first == "IF") {
            s = new IfStmt(num, afterFirst);
        } else if (isEnd(input)) {
            s = new EndStmt(num);
        } else {
            throw QString("非法语句");
        }
    } catch (QString error) {
        QString info = "行号" + QString::number(num) + " " + error;
        QMessageBox::critical(this, "Error", info);
        s = new ErrorStmt(num);
    }
    code[num] = s;
    showSyntaxTree(s);
}

//
// showSyntaxTree以语句s为参数，将s的表达式树输出到treeDisplay
//
void MainWindow::showSyntaxTree(Statement* s)
{
    QString res = s->syntaxTreeStr(program);
    ui->treeDisplay->append(res);
}

//
// runCodeLine以语句s，nextLineNum的引用和代码表为参数，运行这一行语句
// 并应对可能对nextLineNum的修改
//
void MainWindow::runCodeLine(Statement* s, int& nextLineNum, map<int, Statement*>& code)
{
    try {
        s->run(program->varTable);
        stmtType type = s->type;
        if(type == INPUT) {
            startWait(s);
        } else if (type == PRINT) {
            ui->textBrowser->append(QString::number(s->getchildVal(0)));
        } else if (type == GOTO) {
            int num = s->getchildVal(0);
            if (code.find(num) == code.end()) {
                throw QString("跳转到不存在的行号");
            } else {
                nextLineNum = num;
            }
        } else if (type == IF) {
            int num = s->getchildVal(1);
            // num为0表示不需要跳转
            if(num != 0) {
                if (code.find(num) == code.end()) {
                    throw QString("跳转到不存在的行号");
                } else {
                    nextLineNum = num;
                }
            }
        } else if (type == END) {
            nextLineNum = -1;
        }
    } catch (QString error) {
        ui->textBrowser->append("error 行号" + QString::number(s->getLineNum()) + " " + error);
    }
}

//
// startWait以INPUT语句s为参数，设置s要等待输入的变量，开始等待
//
void MainWindow::startWait(Statement* s)
{
    ui->cmdLineEdit->clear();
    ui->cmdLineEdit->setText("? ");
    program->startWaitVar(s->getWaitVarName());
    waitForInput.exec();
}

//
// runInput以字符串input为变量，处理等待时用户的输入
//
void MainWindow::runInput(QString input)
{
    // 用户可能会把"?"提示符删掉，但是无所谓
    if(input[0] == "?") {
        input = afterFirstPart(input.simplified());
    }
    input = input.replace("-", "- ").simplified();
    int pos = 1;
    if(input[0] == "-") {
        // 输入负数
        pos = -1;
        input = afterFirstPart(input);
    }
    if(stringIsPosNum(input)) {
        program->setWaitVar(pos * input.toInt());
        ui->cmdLineEdit->clear();
        waitForInput.quit();
    } else {
        // 错误输入
        ui->cmdLineEdit->setText("? ");
        QMessageBox::critical(this, "Error", "请输入数字");
    }
}

//
// loadFile加载文件
//
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
