#include <iostream>

#include <fstream>

#include <queue>

#include <vector>

using namespace std;

#define PATH 0                      //길

#define WALL 1                      //벽

#define EXIT 10                     //출구

#define ENTER 11                    //입구

//변수

int row, col;                       //가로 세로 길이

bool findExitA = false;              //A* 알고리즘으로 목적지를 찾았는지 확인하는 변수

bool arriveExit = false;            //목적지 도착했는지 확인하는 변수

bool fileOpen = true;               //파일이 잘 열렸는지 확인하는 변수

int mExitX = NULL, mExitY = NULL;   //쥐가 알게된 출구 정보

int scanX, scanY;                   //쥐가 스캔한 지점

//미로 저장 관련 변수

int** mapToprint;                 //txt파일로 미로 정보를 받아올 변수 = 출력할 미로 정보

int** map;                          //갱신할 미로 정보

int** memory;                       //쥐가 기억하는 위치 정보를 저장

//방문한 지역인지 확인

bool** visited;

//쥐가 다시 방문한 지점 기록 -> 최적 경로 위한 변수

bool** mVisited;

//스캔했을 때 5X5 범위

int** isScan;;

int energe;                         //에너지

int useEnerge;                     //사용한 에너지량

double mp;                          //마력

double useMp;                      //사용한 마력량

int useScan;                       //스캔 횟수

int breakWall = 1;                 //남은 벽 부수기 횟수

//최적 경로에 대한 변수

int aEnerge;

int aUseEnerge;

//함수선언

void Input_File(char* filename);                        //파일 입력 함수

void Output_File();                                     //결과 출력 함수

void Mark_Exit(int r, int c);                          //출구 찾는 함수

bool CanScan_Inside(int x, int y);                       //내각 지역 탐색(스캔 가능 여부 확인)

bool CanScan_Outside(int x, int y);                      //외각 지역 탐색(스캔 가능 여부 확인)

void Scan(int x, int y);                                //스캔 함수

void Find_Blockedway(int x, int y);                           //막힌 길 탐색 함수

bool MouseMap_3X3(int x, int y);                   //쥐 시점에서의 미로 정보 저장 및 출구 찾았는지 확인하는 함수

int Heuristic(int x, int y, int goalX, int goalY);      //휴리스틱 함수

void AStar(int x, int y);                               //A* 알고리즘 수행 함수

void AStar_Memory(int x, int y);                        //쥐가 기억하는 길에 대한 A* 알고리즘 수행 함수

void Find_Path_Left(int startX, int startY);                //길 탐색(좌수법) 함수

void Find_Breakway(int x, int y);                      //벽부수기 함수

//파일 입출력 함수

//입력 파일

void Input_File(char* filename)

{

    ifstream file(filename);   //파일 읽어오기

    if (file.is_open())

    {

        int r = 0;

        int c = 0;

        while (!file.eof())

        {

            char ch = (char)file.get();

            if (ch == '0' || ch == '1' || ch == '\n')

            {

                if (ch == '\n')

                {

                    if (r == row)

                    {

                        break;

                    }

                    r++;

                    c = 0;

                    continue;

                }

                //벽

                if (ch == '1')

                {

                    if (c < col)

                    {

                        map[r][c] = WALL;

                    }

                }

                //길

                else if (ch == '0')

                {

                    if (c < col)

                    {

                        map[r][c] = PATH;

                    }

                }

                c++;

            }

        }

        for (int i = 0; i < row; i++)

        {

            for (int j = 0; j < col; j++)

            {

                mapToprint[i][j] = map[i][j];

                memory[i][j] = 8;       //메모리 배열에 지도 복사

            }

        }

        energe = row * col * 2;         //가로 세로 길이(map 크기)에 따라 에너지 초기화

        aEnerge = energe;

        mp = 3.0;                       //초기 마력으로 초기화

        Mark_Exit(row, col);           //출구 표시 함수

    }

    else

    {

        fileOpen = false;

        cout << "파일이 잘못 입력되었습니다.\n";

    }

}

//결과 파일

void Output_File()

{

    for (int i = 0; i < row; i++)

    {

        for (int j = 0; j < col; j++)

        {

            //스캔한 지점 확인

            if (isScan[i][j] == 1)

            {

                cout << "☎";

            }

            //맵 출력

            else if (mapToprint[i][j] == WALL)

            {

                cout << "□";

            }

            //방문 기록

            else if (mVisited[i][j] == true)

            {

                cout << "◆";

            }

            else if (visited[i][j] == true)

            {

                cout << "●";

            }

            //쥐가 저장한 미로 출력

            /*else if (memory[i][j] == WALL)

            {

                cout << "▦";

            }

            else if (memory[i][j] == PATH)

            {

                cout << "□";

            }*/

            //출입구 확인을 위해 작성

            else if (map[i][j] == ENTER)

            {

                cout << "♣";

            }

            else if (map[i][j] == EXIT)

            {

                cout << "★";

            }

            else

            {

                cout << "  ";

            }

        }

        cout << "\n";

    }

}

//출구 표시 함수

//가로세로 길이를 받아와서 for문을 통해 출구를 탐색함

void Mark_Exit(int r, int c)

{

    for (int j = 0; j < c; j++) {

        // 맨 위 체크

        if (map[0][j] == PATH)

        {

            map[0][j] = EXIT;         //출구표시

        }

        // 맨 끝에 체크

        if (map[r - 1][j] == PATH)

        {

            map[r - 1][j] = EXIT;     //출구표시

        }

    }

    for (int i = 0; i < r; i++)

    {

        // 맨 왼쪽 체크

        if (map[i][0] == PATH)

        {

            map[i][0] = EXIT;         //출구표시

        }

        // 맨 오른쪽 체크

        if (map[i][c - 1] == PATH)

        {

            map[i][c - 1] = EXIT;     //출구표시

        }

    }

    map[0][1] = ENTER;

}

// 위치가 스캔 가능한 위치인지 확인하는 함수

// 내각지역 탐색

bool CanScan_Inside(int x, int y)

{

    //스캔 가능한 지역

    if (x > 2 && x < row - 3 && y > 2 && y < col - 3)

    {

        return true;

    }

    return false;

}

// 외각지역 탐색

bool CanScan_Outside(int x, int y)

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

//스캔 함수

void Scan(int x, int y)

{

    scanX = x;

    scanY = y;

    //스캔한 곳 표시

    isScan[x][y] = 1;

    //스캔했을 때 미로 정보  memory에 저장

    for (int i = x - 2; i <= x + 2; i++)

    {

        for (int j = y - 2; j <= y + 2; j++)

        {

            if (map[i][j] == WALL)

            {

                memory[i][j] = WALL;

            }

            else if (map[i][j] == PATH)

            {

                memory[i][j] = PATH;

            }

            else if (map[i][j] == EXIT)

            {

                mExitX = i;

                mExitY = j;

                memory[i][j] = EXIT;

                AStar(x, y);

            }

        }

    }

    for (int i = x - 2; i <= x + 2; i++)

    {

        for (int j = y - 2; j <= y + 2; j++)

        {

            if (i != x && j != y)

            {

                //방문하지 않은 길일 경우

                if (memory[i][j] == PATH && !visited[i][j])

                {

                    //막힌 길이 있는지 확인

                    Find_Blockedway(i, j);

                }

            }

        }

    }

}

//스캔했을 때 막힌 길 탐색 함수

void Find_Blockedway(int x, int y)

{

    //상하좌우 방향에 대한 좌표 변화량

    int dx[4] = { -1, 0, 1, 0 };

    int dy[4] = { 0, -1, 0, 1 };

    //벽의 개수 세기 위한 변수

    int cnt_WALL = 0;

    for (int i = 0; i < 4; i++)

    {

        //상하좌우 방향의 새로운 좌표

        int nx = x + dx[i];

        int ny = y + dy[i];

        //nx, ny 좌표가 벽일 경우

        if (memory[nx][ny] == WALL)

        {

            //벽의 개수 증가

            cnt_WALL++;

        }

    }

    //기준점의 4방향에서 벽이 3개 있으면 막힌 길이라고 판단

    if (cnt_WALL == 3)

    {

        //막힌 길을 벽이라고 표시하도록 함

        memory[x][y] = WALL;

        map[x][y] = WALL;

        for (int i = 0; i < 4; i++)

        {

            //상하좌우 방향의 새로운 좌표

            int nx = x + dx[i];

            int ny = y + dy[i];

            //nx, ny 좌표가 길이고, 스캔한 지점이 아닐 경우

            if (memory[nx][ny] == PATH)

            {

                if (nx != scanX || ny != scanY)

                {

                    //막힌길인지 검사

                    Find_Blockedway(nx, ny);

                }

            }

        }

    }

}

//쥐 시점에서의 미로 정보 저장 및 출구 찾았는지 확인하는 함수

bool MouseMap_3X3(int x, int y)

{

    //상하좌우 방향에 대한 좌표 변화량

    int dx[4] = { -1, 0, 1, 0 };

    int dy[4] = { 0, -1, 0, 1 };

    for (int i = 0; i < 4; i++)

    {

        //상하좌우 방향의 새로운 좌표

        int nx = x + dx[i];

        int ny = y + dy[i];

        if (nx >= 0 && nx < row && ny >= 0 && ny < col)

        {

            //nx, ny 좌표에 대한 처리

            if (map[nx][ny] == WALL)

            {

                memory[nx][ny] = WALL;

            }

            else if (map[nx][ny] == PATH)

            {

                memory[nx][ny] = PATH;

            }

            else if (map[nx][ny] == EXIT)

            {

                mExitX = nx;

                mExitY = ny;

                memory[nx][ny] = EXIT;

                return true;

            }

        }

    }

    return false;

}

//A* 알고리즘

//A* 알고리즘 위한 노드 구조체

struct Node 

{

    int x;  //노드의 x좌표

    int y;  //노드의 y좌표

    int g;  //시작 노드로부터 현재 노드까지의 비용

    int h;  //현재 노드로부터 목표 노드까지의 휴리스틱 비용

    int f;  //총 비용 (g + h)

    Node(int x, int y, int g, int h) : x(x), y(y), g(g), h(h), f(g + h) {}

};

//우선순위 큐에서 노드 비교하는 함수

struct CompareNodes 

{

    bool operator()(const Node& n1, const Node& n2) 

    {

        //노드의 f값이 작을수록 더 높은 우선순위를 가지게 됨

        return n1.f > n2.f;

    }

};

//두 점 사이의 휴리스틱(예상) 비용을 계산하는 함수

int Heuristic(int x, int y, int goalX, int goalY)

{

    //맨하튼 거리 사용 -> 두 점의 x, y좌표 차이의 절대값의 합을 반환함

    return abs(x - goalX) + abs(y - goalY);

}

//A* 알고리즘 수행 함수

void AStar(int startX, int startY) 

{

    //우선순위 큐인 openNodes 생성

    priority_queue<Node, vector<Node>, CompareNodes> openNodes;

    //시작 노드부터 각 노드까지의 비용 저장하기 위한 2차원 배열 cost 생성

    int** cost = new int* [row];

    for (int i = 0; i < row; i++)

    {

        cost[i] = new int[col];

        for (int j = 0; j < col; j++) 

        {

            cost[i][j] = INT_MAX;

        }

    }

    //각 노드의 부모 노드를 저장하기 위한 2차원 배열 parent 생성

    pair<int, int>** parent = new pair<int, int>*[row];

    for (int i = 0; i < row; i++) 

    {

        parent[i] = new pair<int, int>[col];

    }

    //시작 노드 생성

    Node start(startX, startY, 0, Heuristic(startX, startY, mExitX, mExitY));

    //시작 노드의 비용과 부모를 초기화

    cost[startX][startY] = 0;

    parent[startX][startY] = make_pair(-1, -1);

    //시작 노드를 openNodes에 추가

    openNodes.push(start);

    //openNodes가 빌 때까지

    while (!openNodes.empty()) 

    {

        //f값이 가장 작은 노드를 가져옴

        Node current = openNodes.top();

        openNodes.pop();

        int x = current.x;

        int y = current.y;

        ////현재 노드가 목표 노드인지 확인

        if (memory[x][y] == EXIT)

        {

            //목표노드에 도달할 경우 경로 재구성

            while (x != startX || y != startY) 

            {

                if (!visited[x][y])

                {

                    //방문한 경로 표시

                    visited[x][y] = true;

                    energe--;

                    useEnerge++;

                    mp += 0.1;

                }

                //부모 노드 따라가며 경로 탐색

                pair<int, int> p = parent[x][y];

                x = p.first;

                y = p.second;

            }

            map[startX][startY] = PATH;

            findExitA = true;

            break;

        }

        int dx[4] = { -1, 0, 1, 0 };

        int dy[4] = { 0, -1, 0, 1 };

        //이웃 노드의 휴리스틱 비용 계산하고, 이웃 노드 생성

        for (int i = 0; i < 4; i++) 

        {

            int nx = x + dx[i];

            int ny = y + dy[i];

            //이웃 노드가 범위 안에 있는지 확인

            if (nx >= startX - 2 && nx <= startX + 2 && ny >= startY - 2 && ny <= startY + 2) 

            {

                if (map[nx][ny] != WALL)

                {

                    //현재 노드에서 이웃 노드로 가는 이동 거리 계산

                    int ng = current.g + 1;

                    //새로운 경로로 이웃 노드에 도달하는 것이 더 짧은지 확인

                    if (ng < cost[nx][ny])

                    {

                        //새로운 경로가 더 짧을 경우 비용과 부모 정보 업데이트

                        cost[nx][ny] = ng;

                        parent[nx][ny] = make_pair(x, y);

                        //이웃 노드에서 목표 노드까지의 휴리스틱 비용 계산

                        int nh = Heuristic(nx, ny, mExitX, mExitY);

                        //이웃 노드 생성 

                        Node neighbor(nx, ny, ng, nh);

                        //이웃 노드를 우선순위 큐에 추가

                        openNodes.push(neighbor);

                    }

                }

            }

        }

    }

    

    // 메모리 해제

    for (int i = 0; i < row; i++)

    {

        delete[] cost[i];

        delete[] parent[i];

    }

    delete[] cost;

    delete[] parent;

}

//최적 경로 위한 A* 알고리즘 수행 함수

void AStar_Memory(int startX, int startY)

{

    //우선순위 큐인 mOpenNodes 생성

    priority_queue<Node, vector<Node>, CompareNodes> mOpenNodes;

    //시작 노드부터 각 노드까지의 비용 저장하기 위한 2차원 배열 cost 생성

    int** mCost = new int* [row];

    for (int i = 0; i < row; i++)

    {

        mCost[i] = new int[col];

        for (int j = 0; j < col; j++)

        {

            mCost[i][j] = INT_MAX;

        }

    }

    //각 노드의 부모 노드를 저장하기 위한 2차원 배열 parent 생성

    pair<int, int>** mParent = new pair<int, int>*[row];

    for (int i = 0; i < row; i++)

    {

        mParent[i] = new pair<int, int>[col];

    }

    //시작 노드 생성

    Node mStart(startX, startY, 0, Heuristic(startX, startY, mExitX, mExitY));

    //시작 노드의 비용과 부모를 초기화

    mCost[startX][startY] = 0;

    mParent[startX][startY] = make_pair(-1, -1);

    //시작 노드를 openNodes에 추가

    mOpenNodes.push(mStart);

    //openNodes가 빌 때까지

    while (!mOpenNodes.empty())

    {

        //f값이 가장 작은 노드를 가져옴

        Node current = mOpenNodes.top();

        mOpenNodes.pop();

        int x = current.x;

        int y = current.y;

        //현재 노드가 목표 노드인지 확인

        if (x == mExitX && y == mExitY)

        {

            //목표노드에 도달할 경우 경로 재구성

            while (x != startX || y != startY)

            {

                if (!mVisited[x][y])

                {

                    //방문한 경로 표시

                    mVisited[x][y] = true;

                    aEnerge--;

                    aUseEnerge++;

                }

                //부모 노드 따라가며 경로 탐색

                pair<int, int> p = mParent[x][y];

                x = p.first;

                y = p.second;

            }

            memory[startX][startY] = ENTER;

            findExitA = true;

            break;

        }

        int dx[4] = { -1, 0, 1, 0 };

        int dy[4] = { 0, -1, 0, 1 };

        //이웃 노드의 휴리스틱 비용 계산하고, 이웃 노드 생성

        for (int i = 0; i < 4; i++)

        {

            int nx = x + dx[i];

            int ny = y + dy[i];

            //이웃 노드가 범위 안에 있는지 확인

            if (nx >= 0 && nx < row && ny >= 0 && ny < col)

            {

                if (memory[nx][ny] != WALL && memory[nx][ny] != 8)

                {

                    //현재 노드에서 이웃 노드로 가는 이동 거리 계산

                    int ng = current.g + 1;

                    //새로운 경로로 이웃 노드에 도달하는 것이 더 짧은지 확인

                    if (ng < mCost[nx][ny])

                    {

                        //새로운 경로가 더 짧을 경우 비용과 부모 정보 업데이트

                        mCost[nx][ny] = ng;

                        mParent[nx][ny] = make_pair(x, y);

                        //이웃 노드에서 목표 노드까지의 휴리스틱 비용 계산

                        int nh = Heuristic(nx, ny, mExitX, mExitY);

                        //이웃 노드 생성 

                        Node neighbor(nx, ny, ng, nh);

                        //이웃 노드를 우선순위 큐에 추가

                        mOpenNodes.push(neighbor);

                    }

                }

            }

        }

    }

    // 메모리 해제

    for (int i = 0; i < row; i++)

    {

        delete[] mCost[i];

        delete[] mParent[i];

    }

    delete[] mCost;

    delete[] mParent;

}

//좌수법으로 길찾기 함수

void Find_Path_Left(int startX, int startY)

{

    int dx[4] = { -1, 0, 1, 0 };

    int dy[4] = { 0, -1, 0, 1 };

    // 이동 방향 (0: 왼쪽, 1: 아래, 2: 오른쪽, 3: 위), 시작 방향은 아래

    int dir = 1;

    // 현재 위치를 방문 표시합니다.

    visited[startX][startY] = true;

    //입구는 방문했더라도 입구로 표시

    map[0][1] = ENTER;  

    memory[0][1] = ENTER;

    //현재위치

    int x = startX;

    int y = startY;

    while (true)

    {

        dir = (dir + 1) % 4;

        //현재의 왼쪽방향

        int nx = x + dx[dir];

        int ny = y + dy[dir];

        //왼쪽이 벽이 아니라면

        if (map[nx][ny] != WALL && map[nx][ny] != ENTER)

        {

            //왼쪽으로 이동

            x = nx;

            y = ny;

            energe--;

            useEnerge++;

            mp += 0.1;

            //스캔 가능할 경우 스캔하기

            if (CanScan_Inside(x, y) && mp >= 6)

            {

                Scan(x, y);

                mp -= 3;

                useMp += 3;

                useScan++;

                if (breakWall > 0 && mExitX != NULL && mExitY != NULL)

                {

                    Find_Breakway(x, y);

                }

            }

            else if (CanScan_Outside(x, y) && mp >= 3)

            {

                Scan(x, y);

                mp -= 3;

                useMp += 3;

                useScan++;

                if (breakWall > 0 && mExitX != NULL && mExitY != NULL) 

                {

                    Find_Breakway(x, y);

                }

            }

            //방문기록

            visited[x][y] = true;

            //현재의 사방면에 목적지가 있다면

            if (MouseMap_3X3(x, y) || findExitA == true)

            {

                arriveExit = true;

                cout << "목적지 도착\n";

                break;

            }

        }

        //왼쪽이 벽이라면

        else

        {

            //원래 방향으로(오른쪽으로 회전)

            dir = (dir + 3) % 4;

            nx = x + dx[dir];

            ny = y + dy[dir];

            

            //범위가 아닐 경우

            if (nx < 0 or ny < 0) 

            {

                break;

            }

            //시작점으로 돌아온다면:출구에 접근할 수 없다는 뜻

            if (map[x][y] == ENTER)

            {

                cout << "미로를 탐색할 수 없습니다." << endl;

                break;

            }

            //앞이 뚫려있다면

            else if (map[nx][ny] != WALL)

            {

                //직진

                x = nx;

                y = ny;

                energe--;

                useEnerge++;

                mp += 0.1;

                if (CanScan_Inside(x, y) && mp >= 6)

                {

                    Scan(x, y);

                    mp -= 3;

                    useMp += 3;

                    useScan++;

                    if (breakWall > 0 && mExitX != NULL && mExitY != NULL) 

                    {

                        Find_Breakway(x, y);

                    }

                }

                else if (CanScan_Outside(x, y) && mp >= 3)

                {

                    Scan(x, y);

                    mp -= 3;

                    useMp += 3;

                    useScan++;

                    if (breakWall > 0 && mExitX != NULL && mExitY != NULL) 

                    {

                        Find_Breakway(x, y);

                    }

                }

                //방문기록

                visited[x][y] = true;   

                //현재의 사방면에 목적지가 있다면

                if (MouseMap_3X3(x, y) || findExitA == true)

                {

                    arriveExit = true;

                    cout << "목적지 도착\n";

                    break;

                }

            }

            //왼쪽도 앞쪽도 막혀있다면.

            else

            {

                //오른쪽으로 회전

                dir = (dir + 3) % 4;

                nx = x + dx[dir];

                ny = y + dy[dir];

                

                //앞이 뚫려있다면

                if (map[nx][ny] != WALL)

                {

                    //오른쪽으로 이동

                    x = nx;

                    y = ny;

                    energe--;

                    useEnerge++;

                    mp += 0.1;

                    if (CanScan_Inside(x, y) && mp >= 6)

                    {

                        Scan(x, y);

                        mp -= 3;

                        useMp += 3;

                        useScan++;

                        if (breakWall > 0 && mExitX != NULL && mExitY != NULL) 

                        {

                            Find_Breakway(x, y);

                        }

                    }

                    else if (CanScan_Outside(x, y) && mp >= 3)

                    {

                        Scan(x, y);

                        mp -= 3;

                        useMp += 3;

                        useScan++;

                        if (breakWall > 0 && mExitX != NULL && mExitY != NULL) 

                        {

                            Find_Breakway(x, y);

                        }

                    }

                    //방문기록

                    visited[x][y] = true;   

                    //현재의 사방면에 목적지가 있다면

                    if (MouseMap_3X3(x, y) || findExitA == true)

                    {

                        arriveExit = true;

                        cout << "목적지 도착\n";

                        break;

                    }

                }

                //면이 막혀있다면

                else

                {

                    //오른쪽으로 회전(유턴)

                    dir = (dir + 3) % 4;

                    // 왔던길 다시 가기.

                    x = x + dx[dir];

                    y = y + dy[dir];

                    energe--;

                    useEnerge++;

                    mp += 0.1;

                    if (CanScan_Inside(x, y) && mp >= 6)

                    {

                        Scan(x, y);

                        mp -= 3;

                        useMp += 3;

                        useScan++;

                        if (breakWall > 0 && mExitX != NULL && mExitY != NULL) 

                        {

                            Find_Breakway(x, y);

                        }

                    }

                    else if (CanScan_Outside(x, y) && mp >= 3)

                    {

                        Scan(x, y);

                        mp -= 3;

                        useMp += 3;

                        useScan++;

                        if (breakWall > 0 && mExitX != NULL && mExitY != NULL) 

                        {

                            Find_Breakway(x, y);

                        }

                    }

                    if (map[nx][ny] != WALL)

                    {

                        cout << "미로를 탐색할 수 없습니다." << endl;

                        break;

                    }

                }

            }

        }

        //만약 목적지라면

        if (map[x][y] == EXIT || findExitA == true)

        {

            mExitX = x;

            mExitY = y;

            memory[x][y] = EXIT;

            arriveExit = true;

            cout << "목적지 도착" << endl;

            break;

        }

    }

    if (!arriveExit)

    {

        cout << "목적지를 탐색할 수 없습니다." << endl;

    }

}

//벽 부수기 함수

void Find_Breakway(int x, int y) 

{

    //탐색 시작 위치

    int findX = x;      //세로

    int findY = y;      //가로

    //출구 위치

    int exitX = mExitX;

    int exitY = mExitY;

    //출구가 탐색 위치의 오른쪽에 위치

    if (exitY > findY) 

    {

        //출구가 탐색 위치의 위쪽에 위치(오른쪽 위)

        if (exitX < findX) 

        {

            //벽을 뚫어야하는 경우가 2번이하

            if (findX - 2 == exitX and findY + 2 == exitY) 

            {

                //출구가 위쪽 벽면

                if (exitX == 0) 

                {

                    bool canBreakWall = false;

                    for (int i = findX; i <= exitX; i++) 

                    {

                        for (int j = exitY; j <= findY; j++) 

                        {

                            //출구 바로 아래쪽은 제외

                            if (i == exitX + 1 && j == exitY) 

                            {

                                continue;  

                            }

                            //현재 위치 제외

                            if (i == findX && j == findY) 

                            {

                                continue;  

                            }

                            if (memory[i][j] == 0) 

                            {

                                canBreakWall = true;

                                x = i;

                                y = j;

                                break;

                            }

                        }

                        if (canBreakWall) break;

                    }

                    //벽 부수기 수행

                    if (canBreakWall) 

                    {

                        cout << "벽 부심" << endl;

                        cout << x << "," << y << endl;

                        breakWall = 0;

                        //같은 x좌표에 있는 벽(본인의 위,아래)을 길로 만들어주면 된다.

                        if (x == findX) 

                        {

                            map[findX][findY + 1] = PATH;

                            map[findX][findY + 2] = PATH;

                        }

                        else if (x == findX - 1) 

                        {

                            map[findX - 1][findY] = PATH;

                            map[findX - 1][findY + 1] = PATH;

                        }

                        //나머지 else는 벽쪽임

                    }

                }

                //출구가 오른쪽 벽면

                else 

                {

                    bool canBreakWall = false;

                    for (int i = exitX; i <= findX; i++) 

                    {

                        for (int j = findY; j <= exitY; j++) 

                        {

                            //출구 바로 왼쪽은 제외

                            if (i == exitY - 1 && j == exitX) 

                            {

                                continue;  

                            }

                            //현재 위치 제외

                            if (i == findX && j == findY) 

                            {

                                continue;  

                            }

                            if (memory[i][j] == 0) 

                            {

                                canBreakWall = true;

                                x = i;

                                y = j;

                                break;

                            }

                        }

                        if (canBreakWall) break;

                    }

                    if (canBreakWall) 

                    {

                        //벽 부수기 수행

                        cout << "벽 부심" << endl;

                        cout << x << "," << y << endl;

                        breakWall = 0;

                        //같은 y좌표에 있는 벽(본인의 위,아래)을 길로 만들어주면 된다.

                        if (y == findY) 

                        {

                            map[findX - 1][findY] = PATH;

                            map[findX - 2][findY] = PATH;

                        }

                        else if (y == findY + 1) {

                            map[findX][findY + 1] = PATH;

                            map[findX - 1][findY + 1] = PATH;

                        }

                        //나머지 else는 벽임

                    }

                }

            }

            //벽을 뚫어야하는 경우가 1번 이하

            else 

            {

                //현재 위치에서 양 옆이 막혀있다면

                if (memory[findX + 1][findY] == 1 and memory[findX][findY - 1] == 1) 

                {

                    //한쪽만 뚫어줘도 길은 열림

                    map[findX + 1][findY] = PATH;

                }

            }

        }

        //출구가 현재 위치의 아래쪽에 위치(오른쪽 아래)

        else if (exitX > findX) 

        {

            //벽을 뚫어야하는 경우가 2번이하

            if (findX + 2 == exitX and findY + 2 == exitY) 

            {

                //출구가 아래쪽 벽면

                if (exitX == row) 

                {

                    bool canBreakWall = false;

                    for (int i = findX; i <= exitX; i++) 

                    {

                        for (int j = findY; j <= exitY; j++) 

                        {

                            //출구 바로 위쪽은 제외

                            if (i == exitX - 1 && j == exitY) 

                            {

                                continue;  

                            }

                            //현재 위치 제외

                            if (i == findX && j == findY) 

                            {

                                continue;  

                            }

                            if (memory[i][j] == 0) 

                            {

                                canBreakWall = true;

                                x = i;

                                y = j;

                                break;

                            }

                        }

                        if (canBreakWall) break;

                    }

                    if (canBreakWall) 

                    {

                        // 벽 부수기 수행

                        cout << "벽 부심" << endl;

                        cout << x << "," << y << endl;

                        breakWall = 0;

                        // 같은 x좌표에 있는 벽(본인의 위,아래)을 길로 만들어주면 된다.

                        if (x == findX) 

                        {

                            map[findX][findY + 1] = 0;

                            map[findX][findY + 2] = 0;

                        }

                        else if (x == findX + 1) 

                        {

                            map[findX + 1][findY] = 0;

                            map[findX + 1][findY + 1] = 0;

                        }

                        //나머지 else는 벽임

                    }

                }

                //출구가 오른쪽 벽면

                else 

                {

                    bool canBreakWall = false;

                    for (int i = findX; i <= exitX; i++) 

                    {

                        for (int j = findY; j <= exitY; j++) 

                        {

                            //출구 바로 왼쪽은 제외

                            if (i == exitX && j == exitY - 1) 

                            {

                                continue;  

                            }

                            //현재 위치 제외

                            if (i == findX && j == findY) 

                            {

                                continue;  

                            }

                            if (memory[i][j] == 0) 

                            {

                                canBreakWall = true;

                                x = i;

                                y = j;

                                break;

                            }

                        }

                        if (canBreakWall) break;

                    }

                    if (canBreakWall) 

                    {

                        //벽 부수기 수행

                        cout << "벽 부심" << endl;

                        cout << x << "," << y << endl;

                        breakWall = 0;

                        //같은 y좌표에 있는 벽(본인의 위,아래)을 길로 만들어주면 된다.

                        if (y == findY) 

                        {

                            map[findX + 1][findY] = 0;

                            map[findX + 2][findY] = 0;

                        }

                        else if (y == findY + 1) 

                        {

                            map[findX + 1][findY + 1] = 0;

                            map[findX][findY + 1] = 0;

                        }

                        //나머지 else는 벽쪽임

                    }

                }

            }

            //벽을 뚫어야하는 경우가 1번 이하

            else 

            {

                //현재 위치에서 양 옆이 막혀있다면

                if (memory[findX + 1][findY] == 1 and memory[findX][findY + 1] == 1) 

                {

                    //한쪽만 뚫어줘도 길은 열림

                    map[findX + 1][findY] = PATH;

                }

            }

        }

    }

    // 출구가 현재 위치의 왼쪽에 위치

    else if (exitY < findY) 

    {

        //출구가 현재 위치의 위쪽에 위치(왼쪽 위)

        if (exitX < findX) 

        {

            //벽을 뚫어야하는 경우가 2번이하

            if (findX - 2 == exitX and findY - 2 == exitY) 

            {

                //출구가 위쪽 벽면

                if (exitX == 0) 

                {

                    bool canBreakWall = false;

                    for (int i = exitX; i <= findX; i++) 

                    {

                        for (int j = exitY; j <= findY; j++) 

                        {

                            //출구 바로 아래쪽은 제외

                            if (i == exitX + 1 && j == exitY) 

                            {

                                continue;  

                            }

                            //현재 위치 제외

                            if (i == findX && j == findY) 

                            {

                                continue;  

                            }

                            if (memory[i][j] == 0) 

                            {

                                canBreakWall = true;

                                x = i;

                                y = j;

                                break;

                            }

                        }

                        if (canBreakWall) break;

                    }

                    if (canBreakWall) 

                    {

                        // 벽 부수기 수행

                        cout << "벽 부심" << endl;

                        cout << x << "," << y << endl;

                        breakWall = 0;

                        // 같은 x좌표에 있는 벽(본인의 위,아래)을 길로 만들어주면 된다.

                        if (x == findX) 

                        {

                            map[findX][findY - 1] = 0;

                            map[findX][findY - 2] = 0;

                        }

                        else if (x == findX + 1) 

                        {

                            map[findX + 1][findY] = 0;

                            map[findX + 1][findY - 1] = 0;

                        }

                        //나머지 else는 벽쪽임

                    }

                }

                //출구가 왼쪽 벽면

                else if (exitY == 0) 

                {

                    bool canBreakWall = false;

                    for (int i = exitX; i <= findX; i++) 

                    {

                        for (int j = exitY; j <= findY; j++) 

                        {

                            //출구 바로 오른쪽은 제외

                            if (i == exitX && j == exitY + 1) 

                            {

                                continue;  

                            }

                            //현재 위치 제외

                            if (i == findX && j == findY) 

                            {

                                continue;  

                            }

                            if (memory[i][j] == 0) 

                            {

                                canBreakWall = true;

                                x = i;

                                y = j;

                                break;

                            }

                        }

                        if (canBreakWall) break;

                    }

                    if (canBreakWall) 

                    {

                        // 벽 부수기 수행

                        cout << "벽 부심" << endl;

                        cout << x << "," << y << endl;

                        breakWall = 0;

                        // 같은 y좌표에 있는 벽(본인의 위,아래)을 길로 만들어주면 된다.

                        if (y == findY) 

                        {

                            map[findX - 1][findY] = 0;

                            map[findX - 2][findY] = 0;

                        }

                        else if (y == findY - 1) 

                        {

                            map[findX][findY - 1] = 0;

                            map[findX - 1][findY - 1] = 0;

                        }

                        //나머지 else는 벽쪽임

                    }

                }

            }

            //벽을 뚫어야하는 경우가 1번 이하

            else 

            {

                //현재 위치에서 양 옆이 막혀있다면

                if (memory[findX - 1][findY] == 1 and memory[findX][findY - 1] == 1) 

                {

                    //한쪽만 뚫어줘도 길은 열림

                    map[findX - 1][findY] = PATH;

                }

            }

        }

        //출구가 현재 위치의 아래쪽에 위치(왼쪽 아래)

        else if (exitY > y) 

        {

            //벽을 뚫어야하는 경우가 2번이하

            if (findX + 2 == exitX and findY - 2 == exitY) 

            {

                //출구가 아래쪽 벽면

                if (exitX == row) 

                {

                    bool canBreakWall = false;

                    for (int i = exitX; i <= findX; i++) 

                    {

                        for (int j = findY; j <= exitY; j++) 

                        {

                            //출구 바로 위쪽은 제외

                            if (i == exitX && j == exitY - 1) 

                            {

                                continue;  

                            }

                            //현재 위치 제외

                            if (i == findX && j == findY) 

                            {

                                continue;  

                            }

                            if (memory[i][j] == 0) 

                            {

                                canBreakWall = true;

                                x = i;

                                y = j;

                                break;

                            }

                        }

                    }

                    if (canBreakWall) 

                    {

                        // 벽 부수기 수행

                        cout << "벽 부심" << endl;

                        cout << x << "," << y << endl;

                        breakWall = 0;

                        // 같은 x좌표에 있는 벽(본인의 위,아래)을 길로 만들어주면 된다.

                        if (x == findX) 

                        {

                            map[findX][findY - 1] = 0;

                            map[findX][findY - 2] = 0;

                        }

                        else if (x == findX + 1) 

                        {

                            map[findX + 1][findY] = 0;

                            map[findX + 1][findY - 1] = 0;

                        }

                        //나머지 else는 벽쪽임

                    }

                }

                //출구가 왼쪽 벽면

                else if (exitY == 0) 

                {

                    bool canBreakWall = false;

                    for (int i = exitX; i <= findX; i++) 

                    {

                        for (int j = exitY; j <= findY; j++) 

                        {

                            //출구 바로 오른쪽은 제외

                            if (i == exitX && j == exitY + 1) 

                            {

                                continue;  

                            }

                            //현재 위치 제외

                            if (i == findX && j == findY) 

                            {

                                continue;  

                            }

                            if (memory[i][j] == 0) 

                            {

                                canBreakWall = true;

                                x = i;

                                y = j;

                                break;

                            }

                        }

                        if (canBreakWall) break;

                    }

                    if (canBreakWall) 

                    {

                        // 벽 부수기 수행

                        cout << "벽 부심" << endl;

                        cout << x << "," << y << endl;

                        breakWall = 0;

                        // 같은 y좌표에 있는 벽(본인의 위,아래)을 길로 만들어주면 된다.

                        if (y == findY) 

                        {

                            map[findX + 1][findY] = 0;

                            map[findX + 2][findY] = 0;

                        }

                        else if (y == findY - 1) 

                        {

                            map[findX][findY - 1] = 0;

                            map[findX + 1][findY - 1] = 0;

                        }

                        //나머지 else는 벽쪽임

                    }

                }

            }

            //벽을 뚫어야하는 경우가 1번 이하

            else 

            {

                //현재 위치에서 양 옆이 막혀있다면

                if (memory[findX - 1][findY] == 1 and memory[findX][findY + 1] == 1) 

                {

                    //한쪽만 뚫어줘도 길은 열림

                    map[findX - 1][findY] = PATH;

                }

            }

        }

    }

}

//main 함수

int main(void)

{

    char filename[100];

    cout << "파일 이름: ";

    cin >> filename;

    //가로, 세로 길이 입력

    cout << "가로 길이: ";

    cin >> col;

    cout << "세로 길이: ";

    cin >> row;

    cout << "\n";

    //동적할당

    mapToprint = new int* [row];

    map = new int* [row];

    memory = new int* [row];

    visited = new bool* [row];

    mVisited = new bool* [row];

    isScan = new int* [row];

    for (int i = 0; i < row; i++)

    {

        mapToprint[i] = new int[col];

        map[i] = new int[col];

        memory[i] = new int[col];

        visited[i] = new bool[col];

        mVisited[i] = new bool[col];

        isScan[i] = new int[col];

    }

    //부울 변수 초기화

    for (int i = 0; i < row; i++)

    {

        for (int j = 0; j < col; j++)

        {

            visited[i][j] = false;

            mVisited[i][j] = false;

        }

    }

    Input_File(filename);   //txt파일 입력

    if (fileOpen)

    {

        //초기 값  출력

        cout << "초기 에너지 : " << energe << endl;

        cout << "초기 마력 : " << mp << endl;

    }

    Find_Path_Left(0, 1);       //길찾기 함수 실행

    Output_File();          //결과 출력

    for (int i = 0; i < row; i++)

    {

        for (int j = 0; j < col; j++)

        {

            isScan[i][j] = 0;

            visited[i][j] = false;

            findExitA = false;

        }

    }

    //종료 값 출력

    cout << "가로 길이: " << col << "\n";

    cout << "세로 길이: " << row << "\n";

    cout << "종료 에너지 : " << energe << endl;

    cout << "사용 에너지 : " << useEnerge << endl;

    cout << "종료 마력 : " << mp << endl;

    cout << "사용 마력 : " << useMp << endl;

    cout << "스캔 횟수 : " << useScan << endl;

    cout << "벽뚫기 횟수 : " << breakWall << endl;

    cout << "\n";

    //초기 값  출력

    cout << "초기 에너지 : " << aEnerge << endl;

    AStar_Memory(0, 1);

    

    if (findExitA == true)

    {

        cout << "목적지 도착\n";

    }

    else

    {

        cout << "목적지를 탐색하지 못하였습니다.\n";

    }

    Output_File();          //결과 출력

    //최적 경로에 대한 종료 값 출력

    cout << "가로 길이: " << col << "\n";

    cout << "세로 길이: " << row << "\n";

    cout << "종료 에너지 : " << aEnerge << endl;

    cout << "사용 에너지 : " << aUseEnerge << endl;

    //메모리 해제

    for (int i = 0; i < row; i++)

    {

        delete[] mapToprint[i];

        delete[] map[i];

        delete[] memory[i];

        delete[] visited[i];

        delete[] mVisited[i];

        delete[] isScan[i];

    }

    delete[] mapToprint;

    delete[] map;

    delete[] memory;

    delete[] visited;

    delete[] mVisited;

    delete[] isScan;

    

    return 0;

}
