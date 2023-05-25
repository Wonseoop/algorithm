#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;

#define WALL 1 //��
#define PATH 0 //��
#define VISIT 9//�湮
#define ENTER 11//�Ա�

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
            //�̷θ� ��Ÿ���� ����
            //�� : 0
            //�� : 1
            if (ch == '1' || ch == '0' || ch == '\n')
            {
                //�� ���� ����
                if (ch == '\n')
                    r++;
                //�� ���� ����
                if (r == 0)
                    c++;
            }
        }
        ++r;

        file.clear();
        //���� ������ �� ������ �̵�
        file.seekg(0);

        int row = 0;
        int col = 0;

        //�����Ҵ�
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
                    //��
                    map[row][col] = WALL;
                else if (ch == '0')
                    //��
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
                std::cout << "��";
            }
            else if (visited[i][j] == true)//�湮����
            {
                std::cout << "��";
            }
            else if (map[i][j] == 10)
            {
                std::cout << "��";
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

        //���� ����� ��� ����
        if (nX < 0 || nX >= r || nY < 0 || nY >= c)
        {
            continue;
        }

        //���� ��� ����
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
    int dir = 1; // �̵� ���� (0: ����, 1: �Ʒ�, 2: ������, 3: ��), ���� ������ �Ʒ�


    // ���� ��ġ�� �湮 ǥ���մϴ�.
    visited[startX][startY] = true;
    map[startX][startY] = VISIT;

    //������ġ
    int x = startX;
    int y = startY;

    while (true) {
        dir = (dir + 1) % 4;
        //������ ���ʹ���
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        //������ ���ʿ� ������ ��ǥ
        //cout << dir << endl;
        if (map[nx][ny] != WALL && map[nx][ny] != ENTER) {//������ ���� �ƴ϶��
            x = nx;
            y = ny;//�������� �̵�
            //cout << x << "," << y << endl;
            visited[x][y] = true;//�湮���
            //map[x][y] = VISIT;

        }
        else {//������ ���̶��
            dir = (dir + 3) % 4;//���� ��������(���������� ȸ��)
            nx = x + dx[dir];
            ny = y + dy[dir];
            if (map[nx][ny] != WALL) {//���� �շ��ִٸ�
                x = nx;
                y = ny;//����

                visited[x][y] = true;//�湮���
                //map[x][y] = VISIT;
                //cout << x << "," << y << endl;
            }
            else {//���ʵ� ���ʵ� �����ִٸ�.
                dir = (dir + 3) % 4;//���������� ȸ��
                nx = x + dx[dir];
                ny = y + dy[dir];
                if (map[nx][ny] != WALL) {//���� �շ��ִٸ�
                    x = nx;
                    y = ny;//���������� �̵�

                    visited[x][y] = true;//�湮���
                    //map[x][y] = VISIT;
                    //cout << x << "," << y << endl;
                }
                else {//���� �����ִٸ�
                    dir = (dir + 3) % 4;//���������� ȸ��(����)
                    x = x + dx[dir];
                    y = y + dy[dir];//�Դ��� �ٽ� ����.

                    //cout << x << "," << y << endl;
                    if (map[nx][ny] != WALL) {
                        cout << "�̷θ� Ž���� �� �����ϴ�." << endl;
                        break;
                    }
                }
            }
        }
        //cout << x << "," << y << endl;

        if (map[x][y] == 10) {//���� ���������
            cout << "������ ����" << endl;
            break;
        }
    }

}

/*
        // ���� ������ �������� �������� ȸ���Ͽ� ���� ��ġ ���
        dir= (dir + 3) % 4;
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        // ���� ��ġ�� ���̰� ���� �湮���� ���� ���
        if (map[nx][ny] == PATH ) {
            if (!visited[nx][ny]) {
                // �湮 ǥ���ϰ� ���� ��ġ ����
                visited[nx][ny] = true;
                map[nx][ny] = 9;
            }
            x = nx;
            y = ny;
            //dir = (dir + 3) % 4; // �������� ȸ��
        }
        // ���� ������ �������� �����Ͽ� ���� ��ġ ���
        else {
            nx = x + dx[dir];
            ny = y + dy[dir];

            // ���� ��ġ�� ���̰� ���� �湮���� ���� ���
            if (map[nx][ny] == PATH)
            {
                if (!visited[nx][ny]) {
                    // �湮 ǥ���ϰ� ���� ��ġ ����
                    visited[nx][ny] = true;
                    map[nx][ny] = 9;
                }
                x = nx;
                y = ny;
            }

            // ���ʰ� �������� �����ִ� ���
            else {
                dir = (dir + 2) % 4;
                //���������� ȸ��
                int nx = x + dx[dir];
                int ny = y + dy[dir];
                //���� �湮�� ��Ұ� ���̶��
                if (map[nx][ny] == PATH) {
                    if (!visited[nx][ny]) {
                        // �湮 ǥ���ϰ� ���� ��ġ ����
                        visited[nx][ny] = true;
                        map[nx][ny] = 9;
                    }
                    x = nx;
                    y = ny;
                }
                // ���� ��ġ�� ���������� Ȯ��
                else if (map[x][y] == 10) {
                        std::cout << "�������� �����߽��ϴ�.\n";
                        return;
                    }

                    // ���� ��ġ���� �����Ͽ� �̵�
                else {
                    x = x - dx[dir];
                    y = y - dy[dir];

                    // ������ ��ġ�� ���� ��ġ�� ���, Ž�� ����
                    if (x == startX && y == startY) {
                        std::cout << "�̷θ� Ž���� �� �����ϴ�.\n";
                        return;
                    }

                }
            }
        }
        */

int main(void)
{
    char filename[100];
    std::cout << "���� �̸�: ";
    std::cin >> filename;

    Input_File(filename);
    View_Path2(0, 1);
    Output_File();

    std::cout << r << c << std::endl;

    return 0;
}