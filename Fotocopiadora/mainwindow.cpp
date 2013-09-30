#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "photocopier.h"
#include <iostream>
#include <QTimer>

using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, Qt::transparent);
    sample_palette.setColor(QPalette::WindowText, Qt::white);

    //ui->label_4->setAutoFillBackground(true);
    ui->label_4->setPalette(sample_palette);
    ui->label_5->setPalette(sample_palette);
    ui->label_6->setPalette(sample_palette);
    ui->label_7->setPalette(sample_palette);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int dato =(int) ui->spinBox2->value();
    //cout <<dato<< endl;
    createClients(1,dato);
}

void MainWindow::on_pushButton1_clicked()
{
    int dato =(int) ui->spinBox->value();
    createClients(0,dato);

}

void MainWindow::update()
{
    QString studens = QString::number(getStudents());
    ui->label_6->setText(studens); // Studens

    QString teacher = QString::number(getTeacher());
    ui->label_7->setText(teacher); // Theacher
}

/**
QPalette sample_palette;
sample_palette.setColor(QPalette::Window, Qt::white);
sample_palette.setColor(QPalette::WindowText, Qt::blue);

sample_label->setAutoFillBackground(true);
sample_label->setPalette(sample_palette);
sample_label->setText("What ever text");
  */
