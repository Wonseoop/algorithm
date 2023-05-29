#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include <QLabel>
#include <QFile.h>
#include <QTextStream>
#include <QtCore>
#include <iostream>
#include <fstream>
#include <QTimer>

#define WALL "1" //벽
#define PATH "0" //길
#define VISIT "9" //방문
#define ENTER "2" //입구
//define EXIT "10" 출구는 10



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void displayMaze(const QString& fileName);
    void Mark_Exits(int r, int c);
    void View_Path2(int startX, int startY);
    bool CanScanInSide(int x, int y);           //내각지역 스캔가능 위치
    bool CanScanOutSide(int x, int y);          //외각 지역 "
    void Scan(int x, int y);                    //스캔 메서드
    void allocateCells();                       //동적할당
    void resetMaze();                           //초기화
    void isBlocked(int x, int y);               //스캔했을 때 막힌 길 탐색 함수
    void append_Point(int x, int y);            //Point 추가
    void showPath();
    void startPathAnimation();

private slots:
    void on_lineEdit_returnPressed();

    void on_pushButton_clicked();





private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
