#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <Qfile.h>
#include <QTextStream>
#include <QtCore>
#include <iostream>
#include <fstream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file("C:\\Users\\원현섭\\Documents\\untitled1\\maze1_kt.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        ui->label->setText("없어");
    }
    else{
        ui->label->setText("있어");
    }

    QTextStream in(&file);
    QList<QStringList> maze;

    while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList list1 = line.split(' ');
            maze.append(list1);      //2차원 배열 생성
    }

    file.close();

    QLabel* cells[51][46];
    int row=51;
    int col=46;

    // 미로 셀 생성 및 UI에 추가
    for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                QLabel* cell = new QLabel(ui->tableWidget);
                cell->setAlignment(Qt::AlignCenter);
                cell->setFixedSize(100, 100);

                if (maze[i][j] == "0") {
                    cell->setText("0");  // 통로

                    cell->setStyleSheet("QLabel { background-color : white; color : white; }");
                } else {
                    cell->setText("1");  // 벽
                    cell->setStyleSheet("background-color: black; border: 1px solid black");
                }

                cells[i][j] = cell;
                ui->gridLayout->addWidget(cell, i, j);
            }
    }




}

MainWindow::~MainWindow()
{
    delete ui;
}
