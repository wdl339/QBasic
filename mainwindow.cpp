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
    string str = input.toStdString();
    Statement* s = new Statement(str);
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

void MainWindow::changeCode()
{
    vector<Statement*>::iterator p;
    ui->CodeDisplay->clear();
    for(p = stmt.begin(); p != stmt.end(); p++) {
        ui->CodeDisplay->append(QString::fromStdString((*p)->s));
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
