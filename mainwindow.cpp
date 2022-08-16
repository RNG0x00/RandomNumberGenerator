#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <QToolButton>
#include <QPixmap>
#include <QStyle>
#include <QtTextToSpeech/QTextToSpeech>
bool notstopped=true,currunning=false;
int currentnum,index,disindex,prevnum[10000],gcnt;

bool prevnumfind(int gofind){
    for(int i=0;i<gcnt;i++)
        if(prevnum[i]==gofind)
            return true;
    return false;
}

bool MainWindow::findavoid(int gofind){
    for(int i=0;i<ui->listWidget->count();i++)
        if(ui->listWidget->item(i)->text().toInt()==gofind)
            return true;
    return false;
}

bool MainWindow::allfinish(){
    for(int cnt=ui->lineEdit->text().toInt();cnt<=ui->lineEdit_2->text().toInt();cnt+=ui->lineEdit_3->text().toInt())
        if(!prevnumfind(cnt))
            return false;
    return true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Random Number Generator");
    setWindowFlag(Qt::FramelessWindowHint);
    int wide = width();
    QToolButton *minButton = new QToolButton(this);
    QToolButton *closeButton= new QToolButton(this);
    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    minButton->setIcon(minPix);
    closeButton->setIcon(closePix);
    minButton->setGeometry(wide-45,5,20,20);
    closeButton->setGeometry(wide-25,5,20,20);
    minButton->setStyleSheet("background-color:transparent;");
    closeButton->setStyleSheet("background-color:red;");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(minButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
    std::ifstream ifs;
    ifs.open("history.dat");
    ifs>>index;
    ifs.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(currunning){
        ui->label_12->setText("Already generating.");
        ui->label_12->setStyleSheet("background-color:red;\n"
                                    "font:15px \"Lucida Sans\";");
        return ;
    }
    if(allfinish()){
        ui->label_12->setText("No usable numbers. Abort operation.");
        ui->label_12->setStyleSheet("background-color:red;\n"
                                    "font:15px \"Lucida Sans\";");
        return ;
    }
    notstopped=true;
    currunning=true;
    for(int cnt=ui->lineEdit->text().toInt();notstopped;cnt+=ui->lineEdit_3->text().toInt()){
        if(cnt>ui->lineEdit_2->text().toInt()){
            cnt=ui->lineEdit->text().toInt();
            continue;
        }
        if(prevnumfind(cnt))continue;
        if(findavoid(cnt))continue;
        if(cnt<ui->lineEdit_4->text().toInt()||cnt>ui->lineEdit_5->text().toInt())ui->lcdNumber->display(cnt);
        QApplication::processEvents();
    }
    if(ui->checkBox_2->isChecked()){
        int rule;
        std::ifstream ifs;
        ifs.open("default.rule");
        for(int cnt=0;cnt<index;cnt++)
            ifs>>rule;
        rule=0;
        ifs>>rule;
        ifs.close();
        if(rule)
            ui->lcdNumber->display(rule);
    }
    ui->lcdNumber_2->display(currentnum);
    currentnum=ui->lcdNumber->value();
    index++;
    disindex++;
    prevnum[gcnt]=ui->lcdNumber->value();
    gcnt++;
    ui->textBrowser->append("Number "+QString::number(disindex)+":"+QString::number(ui->lcdNumber->value()));
    ui->label_11->setText("Total:"+QString::number(disindex)+" Numbers");
    ui->lcdNumber_3->display(disindex);
    ui->label_12->setText("The number is "+QString::number(ui->lcdNumber->value()));
    ui->label_12->setStyleSheet("background-color:green;\n"
                                "font:15px \"Lucida Sans\";");
    std::ofstream ofs;
    ofs.open("history.dat",std::ios::trunc);
    ofs<<index;
    ofs.close();
}

void MainWindow::on_pushButton_2_clicked()
{
    notstopped=false;
    currunning=false;
}

void MainWindow::on_pushButton_3_clicked()
{
    QTextToSpeech *voice=new QTextToSpeech;
    voice->setLocale(QLocale::English);
    voice->say(QString::number(ui->lcdNumber->value()));
}

void MainWindow::on_pushButton_4_clicked()
{
    std::ofstream ofs;
    ofs.open("default.rule",std::ios::trunc);
    ofs<<ui->lineEdit_6->text().toStdString();
    ofs.close();
    std::ofstream reofs;
    reofs.open("history.dat",std::ios::trunc);
    reofs<<0;
    reofs.close();
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->lineEdit_6->clear();
}

void MainWindow::on_pushButton_6_clicked()
{
    std::ofstream ofs;
    ofs.open("history.dat",std::ios::trunc);
    ofs.close();
    for(int cnt=0;cnt<gcnt;cnt++)prevnum[cnt]=0;
    gcnt=0;
    currentnum=0;
    index=0;
    disindex=0;
    ui->label_12->setText("Finished!");
    ui->label_12->setStyleSheet("background-color:green;\n"
                                "font:15px \"Lucida Sans\";");
    ui->lcdNumber->display(0);
    ui->lcdNumber_2->display(0);
    ui->lcdNumber_3->display(0);
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->listWidget->addItem("Edit...");
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->pushButton_8->setEnabled(true);
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->listWidget->takeItem(ui->listWidget->currentRow());
}
