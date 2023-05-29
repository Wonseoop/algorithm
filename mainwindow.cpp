#include "mainwindow.h"
#include <QTimer>

using namespace std;

//배열
QList<QStringList> maze;                //보여줄 맵
QList<QStringList> memory;              //쥐의 기억속의 맵
int visited[1000][1000] = {};  //쥐의 방문 여부 체크 맵
bool Mvisited[1000][1000] = { false, };
int isScan[1000][1000] = { 0, };        //스캔 가능지역 확인 맵

//ui 셀
QLabel*** cells;
QLabel* cell;

//변수
int row;                                //행
int col;                                //열
bool FindExit = false;                  //A* 할 때 쓰려고 만들어놓은 변수
int ExitX, ExitY;
int MExitX = NULL, MExitY = NULL;
int MouseDir;                           //쥐가 바라보는 방향

//쥐의 스탯
int Energe;                             //에너지
int use_Energe;                         //사용에너지
double MP;                              //마력
double use_MP;                          //사용마력
int use_scan;                           //스캔횟수
int break_wall = 1;                     //벽 부수기 횟수

//현재 위치 좌표값 저장
struct Point {
    int x;
    int y;
    int sum;
};

Point* Move_Pass_Array = nullptr;
int Move_Pass_ArraySize = 0;




//Basic
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);}
MainWindow::~MainWindow()
{
    delete ui;
}


//이쪽 아래 부터 수정 부분


///////////////////////정의 메서드////////////////////////////////////
//파일 데이터 배열에 넣는 메서드
void MainWindow::displayMaze(const QString& fileName)
{

    resetMaze();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        ui->label->setText("파일이 존재하지 않습니다. 경로 재확인 요망");
        return;
    } else {
        ui->label->setText("파일이 존재합니다.");
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList list1 = line.split(' ');
        maze.append(list1);      //2차원 배열 생성
        memory.append(list1);   //쥐의 기억 배열

    }



    file.close();

    row = maze.size();  // 행의 크기
    col = 0;            // 열의 크기

    if (row > 0) {
        col = maze[0].size();  // 첫 번째 행의 열의 크기를 기준으로 설정
    }

    Energe = row * col * 2;         //가로 세로 길이(map 크기)에 따라 에너지 초기화
    MP = 3.0;                   //초기 마력으로 초기화
    Mark_Exits(row, col);       //출구 표시
    maze[0][1] = ENTER;         //입구 표시

    allocateCells();
}

//큐 라벨 동적 할당 메서드
void MainWindow::allocateCells()
{
    cells = new QLabel**[row];

    for (int i = 0; i < row; ++i) {
        cells[i] = new QLabel*[col];
        for (int j = 0; j < col; ++j) {
            cell = new QLabel(ui->tableWidget);
            cell->setAlignment(Qt::AlignCenter);
            cell->setFixedSize(100, 100);

            if (maze[i][j] == PATH) {
                // 통로
                cell->setStyleSheet("QLabel { background-color : white; color : white; }");
            }
            else if (maze[i][j] == WALL) {
                // 벽
                cell->setStyleSheet("QLabel { background-color : black; color : black; }");
            }
            else if (maze[i][j] == "10") {
                // 출구
                cell->setStyleSheet("background-color: blue; ");
            }


            cells[i][j] = cell;
            ui->gridLayout->addWidget(cell, i, j);
        }
    }
    cells[0][1]->setStyleSheet("QLabel { background-color: red }"); //입구

}

//초기화 메서드
void MainWindow::resetMaze()
{
    // maze 배열 초기화
    maze.clear();
    memory.clear();
    memset(visited, 0, sizeof(visited));
    memset(isScan, 0, sizeof(isScan));

    // 기존에 할당된 cells 배열 해제
    if (cells != nullptr) {
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                delete cells[i][j];
            }
            delete[] cells[i];
        }
        delete[] cells;
        cells = nullptr;
    }

    row = 0;
    col = 0;

    if (Move_Pass_Array != nullptr) {
        delete[] Move_Pass_Array;
        Move_Pass_Array = nullptr;
        Move_Pass_ArraySize = 0;
    }


}

//출구를 표시하는 메서드
void MainWindow::Mark_Exits(int r, int c){
    for (int j = 0; j < c; j++) {
        // Check the top row
        if (maze[0][j] == PATH) {
            maze[0][j] = "10"; // Mark as exit
        }

        // Check the bottom row
        if (maze[r - 1][j] == PATH) {
            maze[r - 1][j] = "10"; // Mark as exit
        }
    }

    for (int i = 0; i < r; i++) {
        // Check the left column
        if (maze[i][0] == PATH) {
            maze[i][0] = "10"; // Mark as exit
        }

        // Check the right column
        if (maze[i][c - 1] == PATH) {
            maze[i][c - 1] = "10"; // Mark as exit
        }
    }
    maze[0][1] = ENTER;
}

//내각지역 탐색
bool CanScanInSide(int x, int y)
{
    //스캔 가능한 지역
    if (x > 2 && x < row - 3 && y > 2 && y < col - 3)
    {
        return true;
    }
    return false;
}

// 외각지역 탐색
bool CanScanOutSide(int x, int y)
{
    //외벽 볼 수 있는 가능성이 있을 때
    if ((x == 2 && y >= 2 && y <= col - 3) || (x == row - 3 && y >= 2 && y <= col - 3))
    {
        return true;
    }
    else if ((y == 2 && x >= 2 && x <= row - 3) || (y == col - 3 && x >= 2 && x <= row - 3))
    {
        return true;
    }
    return false;
}

//좌선법 정의 메서드
void MainWindow::View_Path2(int startX, int startY){
    int dx[4] = { -1, 0, 1, 0 };
    int dy[4] = { 0, -1, 0, 1 };
    int dir = 1; // 이동 방향 (0: 왼쪽, 1: 아래, 2: 오른쪽, 3: 위), 시작 방향은 아래


    // 현재 위치를 방문 표시합니다.
    visited[startX][startY] +=1;
    maze[startX][startY] = VISIT;

    //현재위치
    int x = startX;
    int y = startY;

    while (true) {
        dir = (dir + 1) % 4;
        //현재의 왼쪽방향
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        //현재의 왼쪽에 놓여진 좌표
        //cout << dir << endl;
        if (maze[nx][ny] != WALL && maze[nx][ny] != ENTER) {//왼쪽이 벽이 아니라면
            x = nx;
            y = ny;//왼쪽으로 이동
            //cout << x << "," << y << endl;
            visited[x][y] +=1;//방문기록
            //maze[x][y] = VISIT;
            append_Point(x,y, visited[x][y]);


        }
        else {//왼쪽이 벽이라면
            dir = (dir + 3) % 4;//원래 방향으로(오른쪽으로 회전)
            nx = x + dx[dir];
            ny = y + dy[dir];
            if (maze[nx][ny] != WALL) {//앞이 뚫려있다면
                x = nx;
                y = ny;//직진

                visited[x][y]  +=1;//방문기록
                append_Point(x,y, visited[x][y]);
                //maze[x][y] = VISIT;
                //cout << x << "," << y << endl;
            }
            else {//왼쪽도 앞쪽도 막혀있다면.
                dir = (dir + 3) % 4;//오른쪽으로 회전
                nx = x + dx[dir];
                ny = y + dy[dir];
                if (maze[nx][ny] != WALL) {//앞이 뚫려있다면
                    x = nx;
                    y = ny;//오른쪽으로 이동

                    visited[x][y] +=1;//방문기록
                    append_Point(x,y, visited[x][y]);
                    //maze[x][y] = VISIT;
                    //cout << x << "," << y << endl;
                }
                else {//면이 막혀있다면
                    dir = (dir + 3) % 4;//오른쪽으로 회전(유턴)
                    x = x + dx[dir];
                    y = y + dy[dir];//왔던길 다시 가기.
                    append_Point(x,y, visited[x][y]);

                    //cout << x << "," << y << endl;
                    if (maze[nx][ny] != WALL) {
                        cout << "미로를 탐색할 수 없습니다." << endl;
                        break;
                    }
                }
            }
        }
        //cout << x << "," << y << endl;

        if (maze[x][y] == "10") {//만약 목적지라면

            break;
        }
    }

}



//포인트 추가 메서드
void MainWindow::append_Point(int x, int y, int sum){
    Point* newMovePassArray = new Point[Move_Pass_ArraySize + 1];

    // 기존 배열의 원소를 새로운 배열로 복사
    for (int i = 0; i < Move_Pass_ArraySize; ++i) {
        newMovePassArray[i] = Move_Pass_Array[i];
    }

    // 새로운 원소 추가
    newMovePassArray[Move_Pass_ArraySize].x = x;
    newMovePassArray[Move_Pass_ArraySize].y = y;
    newMovePassArray[Move_Pass_ArraySize].sum = sum;

    // 이전 배열 메모리 해제
    delete[] Move_Pass_Array;

    // 업데이트된 배열로 포인터 및 크기 업데이트
    Move_Pass_Array = newMovePassArray;
    ++Move_Pass_ArraySize;

}

//경로 보여주는 메서드
void MainWindow::showPath()
{
    static int currentIndex = 0;  // 현재 보여지고 있는 배열 원소의 인덱스
    if (currentIndex < Move_Pass_ArraySize-1) {
        int x = Move_Pass_Array[currentIndex].x;
        int y = Move_Pass_Array[currentIndex].y;

        if(Move_Pass_Array[currentIndex].sum==1){

            cells[x][y]->setStyleSheet("QLabel { background-color: rgb(60, 179, 113) }");
        }
        else if (Move_Pass_Array[currentIndex].sum>1){
            cells[x][y]->setStyleSheet("QLabel { background-color: green }");
        }
        currentIndex++;
    } else {
        // 배열 모든 원소를 보여준 후에는 타이머를 멈추고 초기화
        QTimer* timer = qobject_cast<QTimer*>(sender());
        if (timer) {
            timer->stop();
            currentIndex = 0;
        }
    }
}






//////////////////////이벤트 메서드////////////////////////////
//파일이름 입력받는 메서드
void MainWindow::on_lineEdit_returnPressed()
{
    QString fileName = ui->lineEdit->text();
    displayMaze(fileName);
}

// 이벤트 메서드 내에서 Qtimer를 시작하는 함수
void MainWindow::startPathAnimation()
{
    // 500ms마다 showPath() 메서드를 호출하는 타이머 생성 및 시작
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::showPath);
    timer->start(20);



}


//좌선법 실행부 메서드
void MainWindow::on_pushButton_clicked()
{

    View_Path2(0, 1);     //move_count 반환


    startPathAnimation();





}



