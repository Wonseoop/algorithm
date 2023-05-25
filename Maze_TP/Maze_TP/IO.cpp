//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <queue>
//
//#define WALL 1 //��
//#define PATH 0 //��
//
//int r, c;
//int** map = NULL;
//bool visited[1000][1000] = { false, };
//
//int leftX, leftY;
//int dirX, dirY;
//
//
//void Input_File(char* filename)
//{
//	std::ifstream file(filename);
//	r = 0;
//	c = 0;
//
//	if (file.is_open())
//	{
//		while (!file.eof())
//		{
//			char ch = (char)file.get();
//			//�̷θ� ��Ÿ���� ����
//			//�� : 0
//			//�� : 1
//			if (ch == '1' || ch == '0' || ch == '\n')
//			{
//				//�� ���� ����
//				if (ch == '\n')
//					r++;
//				//�� ���� ����
//				if (r == 0)
//					c++;
//			}
//		}
//		++r;
//
//		file.clear();
//		//���� ������ �� ������ �̵�
//		file.seekg(0);
//
//		int row = 0;
//		int col = 0;
//
//		//�����Ҵ�
//		map = new int* [r];
//		map[row] = new int[c];
//
//		while (!file.eof())
//		{
//			char ch = (char)file.get();
//			if (ch == '0' || ch == '1' || ch == '\n')
//			{
//				if (ch == '\n')
//				{
//					row++;
//					if (row == r)
//						break;
//					col = 0;
//					map[row] = new int[c];
//					continue;
//				}
//
//				if (ch == '1')
//					//��
//					map[row][col] = 1;
//				else if (ch == '0')
//					//��
//					map[row][col] = 0;
//				col++;
//			}
//		}
//	}
//}
//
//void Output_File()
//{
//	for (int i = 0; i < r; i++)
//	{
//		for (int j = 0; j < c; j++)
//		{
//			if (map[i][j] == WALL)
//			{
//				std::cout << "��";
//			}
//			else if (map[i][j] == 9)
//			{
//				std::cout << "��";
//			}
//			else
//			{
//				std::cout << "  ";
//			}
//		}
//		std::cout << "\n";
//	}
//}
//
//void View_Mouse(int x, int y)
//{
//
//}
//
////DFS
//void View_Path1(int x, int y, int TYPE)
//{
//	int dx[4] = { 1, 0, -1, 0 };
//	int dy[4] = { 0, 1, 0, -1 };
//
//	for (int i = 0; i < 4; i++)
//	{
//		int nX = x + dx[i];
//		int nY = y + dy[i];
//
//		//���� ����� ��� ����
//		if (nX < 0 || nX >= r || nY < 0 || nY >= c)
//		{
//			continue;
//		}
//
//		//���� ��� ����
//		if (map[nX][nY] == WALL)
//		{
//			continue;
//		}
//		
//		if (visited[nX][nY] == true)
//		{
//			continue;
//		}
//
//		visited[nX][nY] = true;
//		
//		map[27][45] = 10;
//		map[nX][nY] = 9;
//
//		View_Path1(nX, nY, 0);
//	}
//}
//
//bool CheckLeft(int x, int y) {
//	if (y > 0 && map[x][y - 1] == WALL)
//		return true;
//	return false;
//}
//
//// �Լ�: �־��� ��ǥ�� ��ȿ�� �̵� ��ġ���� Ȯ���մϴ�.
//bool IsValidMove(int x, int y)
//{
//	// ��ȿ�� ��ǥ ���� ���� �ִ��� Ȯ��
//	if (x >= 0 && x < r && y >= 0 && y < c)
//	{
//		// �ش� ��ǥ�� ���� �ƴϸ� ��ȿ�� �̵� ��ġ�Դϴ�.
//		if (map[x][y] != 1)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//
//void SetMouseDirection(int startX, int startY, int& dirX, int& dirY, int& leftX, int& leftY)
//{
//	// ���� ��ġ�� �湮 ǥ���մϴ�.
//	map[startX][startY] = 9;
//
//	/*int x = startX;
//	int y = startY;*/
//
//	// x, y�� ���� ��ġ�̰� �㰡 x, y + 1�� ���� �㰡 �ٶ󺸰� �ִٸ�
//	// x-1, y�� ���� �������̰�
//	// x+1, y�� ���� ������
//
//	//�㰡 �ٶ󺸰� �ִ� ���� ���� �������ֵ��� ����
//	/*int dirX = 0;
//	int dirY = 0;*/
//
//	//�׿� ���� ���� ���� ��ġ ������ ����
//	/*int left_X = 0;
//	int left_Y = 0;*/
//
//	//���� �ʱ� ��ġ�� �� ������ ���
//	if (startY == 0)
//	{
//		//�㰡 �Ʒ����� �ٶ󺸵��� ��
//		dirY = 1;
//		dirX = 0;
//
//		leftY = dirY + 1;
//		leftX = dirX;
//	}
//	//���� �ʱ� ��ġ�� �� �Ʒ����� ���
//	else if (startY == r - 1)
//	{
//		//�㰡 ������ �ٶ󺸵��� ��
//		dirY = -1;
//		dirX = 0;
//
//		leftY = dirY - 1;
//		leftX = dirX;
//	}
//	//���� �ʱ� ��ġ�� �� ������ ���
//	else if (startX == 0)
//	{
//		//�㰡 �������� ��󺸵��� ��
//		dirY = 0;
//		dirX = 1;
//
//		leftY = dirY;
//		leftX = dirX - 1;
//	}
//	//���� �ʱ� ��ġ�� �� �������� ���
//	else if (startX == c - 1)
//	{
//		//�㰡 ������ �ٶ󺸵��� ��
//		dirY = 0;
//		dirX = -1;
//
//		leftY = dirY;
//		leftX = dirX + 1;
//	}
//}
//
//void View_Path2(int startX, int startY)
//{
//	// Mark the current position as visited
//	map[startX][startY] = 9;
//
//	int x = startX;
//	int y = startY;
//
//	// ���� �ʱ� ����� ���� ��ǥ ����
//	int dirX = 0;
//	int dirY = 0;
//	int leftX = 0;
//	int leftY = 0;
//
//	// ���� �ٶ󺸴� ����� ���� ��ǥ �����ϴ� �Լ� ȣ��
//	SetMouseDirection(x, y, dirX, dirY, leftX, leftY); 
//
//	while (true)
//	{
//		// ���� ��ġ���� ��ȸ�� �������� �̵� �������� Ȯ��
//		int nextLefzkzkdhxhrqlqjsdlwtX = x + leftX;
//		int nextLeftY = y + leftY;
//
//		if (IsValidMove(leftX, leftY))
//		{
//			// �̵��ϰ� �湮 ǥ��
//			x = nextLeftX;
//			y = nextLeftY;
//			map[x][y] = 9;
//
//			// ���� �̵��� ���� ���� �ٶ󺸴� ����� ���� ��ǥ ������Ʈ
//			SetMouseDirection(x, y, dirX, dirY, leftX, leftY); 
//		}
//		else
//		{
//			// Check if forward move is a valid move
//			int forwardX = x + dirX;
//			int forwardY = y + dirY;
//
//			if (IsValidMove(forwardX, forwardY))
//			{
//				// Move forward
//				x = forwardX;
//				y = forwardY;
//				map[x][y] = 9;
//			}
//			else
//			{
//				// Check if backward move is a valid move
//				int backX = x - dirX;
//				int backY = y - dirY;
//
//				if (IsValidMove(backX, backY))
//				{
//					// Move backward
//					x = backX;
//					y = backY;
//					map[x][y] = 9;
//				}
//				else
//				{
//					// Maze cannot be explored further
//					std::cout << "�̷θ� Ž���� �� �����ϴ�.\n";
//					break;
//				}
//			}
//
//			SetMouseDirection(x, y, dirX, dirY, leftX, leftY);
//		}
//
//		// Check if current position is the destination
//		if (map[x][y] == 10)
//		{
//			std::cout << "�������� �����߽��ϴ�.\n";
//			break;
//		}
//	}
//}
//
//int main(void)
//{
//	char filename[100];
//	std::cout << "���� �̸�: ";
//	std::cin >> filename;
//
//	Input_File(filename);
//
//	map[27][45] = 10;
//
//	View_Path2(0, 1);
//
//	for (int i = 0; i < r; i++)
//	{
//		for (int j = 0; j < c; j++)
//		{
//			std::cout << map[i][j] << " ";
//		}
//		std::cout << "\n";
//	}
//
//
//	Output_File();
//
//	std::cout << r << c << std::endl;
//
//	return 0;
//}