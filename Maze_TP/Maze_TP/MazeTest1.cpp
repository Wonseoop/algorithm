#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;

#define WALL 1 //벽
#define PATH 0 //길
#define VISIT 9//방문
#define ENTER 11//입구

int r, c;
int** map = NULL;
bool visited[1000][1000] = { false, };
void Mark_Exits(int r, int c);;

void Input_File(char* filename)
{
    std::ifstream file(filename);
    r = 0;
    c = 0;

    if (file.is_open())
    {
        while (!file.eof())
        {
            char ch = (char)file.get();
            //미로를 나타내는 정보
            //길 : 0
            //벽 : 1
            if (ch == '1' || ch == '0' || ch == '\n')
            {
                //열 길이 증가
                if (ch == '\n')
                    r++;
                //행 길이 증가
                if (r == 0)
                    c++;
            }
        }
        ++r;

        file.clear();
        //파일 포인터 맨 앞으로 이동
        file.seekg(0);

        int row = 0;
        int col = 0;

        //동적할당
        map = new int* [r];
        map[row] = new int[c];

        while (!file.eof())
        {
            char ch = (char)file.get();
            if (ch == '0' || ch == '1' || ch == '\n')
            {
                if (ch == '\n')
                {
                    row++;
                    if (row == r)
                        break;
                    col = 0;
                    map[row] = new int[c];
                    continue;
                }

                if (ch == '1')
                    //벽
                    map[row][col] = WALL;
                else if (ch == '0')
                    //길
                    map[row][col] = PATH;
                col++;
            }
        }

        Mark_Exits(r, c);
    }
}

void Output_File()
{
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (map[i][j] == WALL)
            {
                std::cout << "□";
            }
            else if (visited[i][j] == true)//방문여부
            {
                std::cout << "●";
            }
            else if (map[i][j] == 10)
            {
                std::cout << "★";
            }
            else
            {
                std::cout << "  ";
            }
        }
        std::cout << "\n";
    }
}

void Mark_Exits(int r, int c)
{
    for (int j = 0; j < c; j++) {
        // Check the top row
        if (map[0][j] == PATH) {
            map[0][j] = 10; // Mark as exit
        }

        // Check the bottom row
        if (map[r - 1][j] == PATH) {
            map[r - 1][j] = 10; // Mark as exit
        }
    }

    for (int i = 0; i < r; i++) {
        // Check the left column
        if (map[i][0] == PATH) {
            map[i][0] = 10; // Mark as exit
        }

        // Check the right column
        if (map[i][c - 1] == PATH) {
            map[i][c - 1] = 10; // Mark as exit
        }
    }
    map[0][1] = ENTER;
}


void View_Mouse(int x, int y)
{

}

/*
//DFS
void View_Path1(int x, int y, int TYPE)
{
    int dx[4] = { 1, 0, -1, 0 };
    int dy[4] = { 0, 1, 0, -1 };

    for (int i = 0; i < 4; i++)
    {
        int nX = x + dx[i];
        int nY = y + dy[i];

        //범위 벗어나는 경우 무시
        if (nX < 0 || nX >= r || nY < 0 || nY >= c)
        {
            continue;
        }

        //벽일 경우 무시
        if (map[nX][nY] == WALL)
        {
            continue;
        }

        if (visited[nX][nY] == true)
        {
            continue;
        }

        visited[nX][nY] = true;

        map[27][45] = 10;
        map[nX][nY] = 9;

        View_Path1(nX, nY, 0);
    }
}
*/
void View_Path2(int startX, int startY)
{
    int dx[4] = { -1, 0, 1, 0 };
    int dy[4] = { 0, -1, 0, 1 };
    int dir = 1; // 이동 방향 (0: 왼쪽, 1: 아래, 2: 오른쪽, 3: 위), 시작 방향은 아래


    // 현재 위치를 방문 표시합니다.
    visited[startX][startY] = true;
    map[startX][startY] = VISIT;

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
        if (map[nx][ny] != WALL && map[nx][ny] != ENTER) {//왼쪽이 벽이 아니라면
            x = nx;
            y = ny;//왼쪽으로 이동
            //cout << x << "," << y << endl;
            visited[x][y] = true;//방문기록
            //map[x][y] = VISIT;

        }
        else {//왼쪽이 벽이라면
            dir = (dir + 3) % 4;//원래 방향으로(오른쪽으로 회전)
            nx = x + dx[dir];
            ny = y + dy[dir];
            if (map[nx][ny] != WALL) {//앞이 뚫려있다면
                x = nx;
                y = ny;//직진

                visited[x][y] = true;//방문기록
                //map[x][y] = VISIT;
                //cout << x << "," << y << endl;
            }
            else {//왼쪽도 앞쪽도 막혀있다면.
                dir = (dir + 3) % 4;//오른쪽으로 회전
                nx = x + dx[dir];
                ny = y + dy[dir];
                if (map[nx][ny] != WALL) {//앞이 뚫려있다면
                    x = nx;
                    y = ny;//오른쪽으로 이동

                    visited[x][y] = true;//방문기록
                    //map[x][y] = VISIT;
                    //cout << x << "," << y << endl;
                }
                else {//면이 막혀있다면
                    dir = (dir + 3) % 4;//오른쪽으로 회전(유턴)
                    x = x + dx[dir];
                    y = y + dy[dir];//왔던길 다시 가기.

                    //cout << x << "," << y << endl;
                    if (map[nx][ny] != WALL) {
                        cout << "미로를 탐색할 수 없습니다." << endl;
                        break;
                    }
                }
            }
        }
        //cout << x << "," << y << endl;

        if (map[x][y] == 10) {//만약 목적지라면
            cout << "목적지 도착" << endl;
            break;
        }
    }

}

/*
        // 현재 방향을 기준으로 왼쪽으로 회전하여 다음 위치 계산
        dir= (dir + 3) % 4;
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        // 다음 위치가 길이고 아직 방문하지 않은 경우
        if (map[nx][ny] == PATH ) {
            if (!visited[nx][ny]) {
                // 방문 표시하고 현재 위치 갱신
                visited[nx][ny] = true;
                map[nx][ny] = 9;
            }
            x = nx;
            y = ny;
            //dir = (dir + 3) % 4; // 왼쪽으로 회전
        }
        // 현재 방향을 기준으로 직진하여 다음 위치 계산
        else {
            nx = x + dx[dir];
            ny = y + dy[dir];

            // 다음 위치가 길이고 아직 방문하지 않은 경우
            if (map[nx][ny] == PATH)
            {
                if (!visited[nx][ny]) {
                    // 방문 표시하고 현재 위치 갱신
                    visited[nx][ny] = true;
                    map[nx][ny] = 9;
                }
                x = nx;
                y = ny;
            }

            // 왼쪽과 오른쪽이 막혀있는 경우
            else {
                dir = (dir + 2) % 4;
                //오른쪽으로 회전
                int nx = x + dx[dir];
                int ny = y + dy[dir];
                //다음 방문할 장소가 길이라면
                if (map[nx][ny] == PATH) {
                    if (!visited[nx][ny]) {
                        // 방문 표시하고 현재 위치 갱신
                        visited[nx][ny] = true;
                        map[nx][ny] = 9;
                    }
                    x = nx;
                    y = ny;
                }
                // 현재 위치가 목적지인지 확인
                else if (map[x][y] == 10) {
                        std::cout << "목적지에 도달했습니다.\n";
                        return;
                    }

                    // 현재 위치에서 후진하여 이동
                else {
                    x = x - dx[dir];
                    y = y - dy[dir];

                    // 후진한 위치가 시작 위치인 경우, 탐색 종료
                    if (x == startX && y == startY) {
                        std::cout << "미로를 탐색할 수 없습니다.\n";
                        return;
                    }

                }
            }
        }
        */

int main(void)
{
    char filename[100];
    std::cout << "파일 이름: ";
    std::cin >> filename;

    Input_File(filename);
    View_Path2(0, 1);
    Output_File();

    std::cout << r << c << std::endl;

    return 0;
}