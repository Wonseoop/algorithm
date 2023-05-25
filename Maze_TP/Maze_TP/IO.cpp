//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <queue>
//
//#define WALL 1 //벽
//#define PATH 0 //길
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
//			//미로를 나타내는 정보
//			//길 : 0
//			//벽 : 1
//			if (ch == '1' || ch == '0' || ch == '\n')
//			{
//				//열 길이 증가
//				if (ch == '\n')
//					r++;
//				//행 길이 증가
//				if (r == 0)
//					c++;
//			}
//		}
//		++r;
//
//		file.clear();
//		//파일 포인터 맨 앞으로 이동
//		file.seekg(0);
//
//		int row = 0;
//		int col = 0;
//
//		//동적할당
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
//					//벽
//					map[row][col] = 1;
//				else if (ch == '0')
//					//길
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
//				std::cout << "□";
//			}
//			else if (map[i][j] == 9)
//			{
//				std::cout << "●";
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
//		//범위 벗어나는 경우 무시
//		if (nX < 0 || nX >= r || nY < 0 || nY >= c)
//		{
//			continue;
//		}
//
//		//벽일 경우 무시
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
//// 함수: 주어진 좌표가 유효한 이동 위치인지 확인합니다.
//bool IsValidMove(int x, int y)
//{
//	// 유효한 좌표 범위 내에 있는지 확인
//	if (x >= 0 && x < r && y >= 0 && y < c)
//	{
//		// 해당 좌표가 벽이 아니면 유효한 이동 위치입니다.
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
//	// 현재 위치를 방문 표시합니다.
//	map[startX][startY] = 9;
//
//	/*int x = startX;
//	int y = startY;*/
//
//	// x, y가 쥐의 위치이고 쥐가 x, y + 1을 향해 쥐가 바라보고 있다면
//	// x-1, y가 쥐의 오른쪽이고
//	// x+1, y가 쥐의 왼쪽임
//
//	//쥐가 바라보고 있는 방향 변수 설정해주도록 하자
//	/*int dirX = 0;
//	int dirY = 0;*/
//
//	//그에 따른 쥐의 왼쪽 위치 변수를 설정
//	/*int left_X = 0;
//	int left_Y = 0;*/
//
//	//쥐의 초기 위치가 맨 위쪽일 경우
//	if (startY == 0)
//	{
//		//쥐가 아래쪽을 바라보도록 함
//		dirY = 1;
//		dirX = 0;
//
//		leftY = dirY + 1;
//		leftX = dirX;
//	}
//	//쥐의 초기 위치가 맨 아래쪽일 경우
//	else if (startY == r - 1)
//	{
//		//쥐가 위쪽을 바라보도록 함
//		dirY = -1;
//		dirX = 0;
//
//		leftY = dirY - 1;
//		leftX = dirX;
//	}
//	//쥐의 초기 위치가 맨 왼쪽일 경우
//	else if (startX == 0)
//	{
//		//쥐가 오른쪽을 밥라보도록 함
//		dirY = 0;
//		dirX = 1;
//
//		leftY = dirY;
//		leftX = dirX - 1;
//	}
//	//쥐의 초기 위치가 맨 오른쪽일 경우
//	else if (startX == c - 1)
//	{
//		//쥐가 왼쪽을 바라보도록 함
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
//	// 쥐의 초기 방향과 좌측 좌표 설정
//	int dirX = 0;
//	int dirY = 0;
//	int leftX = 0;
//	int leftY = 0;
//
//	// 쥐의 바라보는 방향과 좌측 좌표 설정하는 함수 호출
//	SetMouseDirection(x, y, dirX, dirY, leftX, leftY); 
//
//	while (true)
//	{
//		// 현재 위치에서 좌회전 방향으로 이동 가능한지 확인
//		int nextLefzkzkdhxhrqlqjsdlwtX = x + leftX;
//		int nextLeftY = y + leftY;
//
//		if (IsValidMove(leftX, leftY))
//		{
//			// 이동하고 방문 표시
//			x = nextLeftX;
//			y = nextLeftY;
//			map[x][y] = 9;
//
//			// 다음 이동을 위해 쥐의 바라보는 방향과 좌측 좌표 업데이트
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
//					std::cout << "미로를 탐색할 수 없습니다.\n";
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
//			std::cout << "목적지에 도착했습니다.\n";
//			break;
//		}
//	}
//}
//
//int main(void)
//{
//	char filename[100];
//	std::cout << "파일 이름: ";
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