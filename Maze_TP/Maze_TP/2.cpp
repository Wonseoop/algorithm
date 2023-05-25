//#include <iostream>
//#include <queue>
//#include <vector>
//
//using namespace std;
//
////미로의 크기 (예)
//const int row = 5;
//const int col = 5;
//
////미로의 벽을 나타내는 상수
//const int WALL = 1;
//
//class Point
//{
//public:
//	//노드의 x, y 좌표
//	int x, y;
//
//	Point(int x, int y) : x(x), y(y) {}
//};
//
//class Node
//{
//public:
//	//노드의 위치
//	Point position;
//	//출발점으로부터의 이동 비용
//	int g;
//	//목적지까지의 예상 이동 비용 
//	int h;
//	//g + h
//	int f;
//	//이 노드의 부모 노드
//	Node* parent;
//
//	Node(Point position) : position(position), g(0), h(0), f(0), parent(nullptr) {}
//
//	//f 값 비교 위한 연산자 오버로딩
//	bool operator < (const Node& other) const
//	{
//		return f > other.f;
//	}
//};
//
//vector<Point> AStar(int maze[row][col], Point start, Point goal)
//{
//	//오픈 리스트: 아직 방문하지 않은 노드들
//	priority_queue<Node*> openList;
//
//	//방문한 노드들을 저장하는 배열
//	vector<Node*> visited;
//
//	//시작 노드의 g값과 h값을 초기화
//	Node* startNode = new Node(start);
//	startNode->g = 0;
//	startNode->h = abs(start.x - goal.x) + abs(start.y - goal.y);
//	startNode->f = startNode->g + startNode->h;
//
//	//시작 노드를 오픈 리스트에 추가
//	openList.push(startNode);
//
//	//목적지에 도달할 때까지 반복
//	while (!openList.empty())
//	{
//		//현재 노드 선택 (f값이 가장 작은 노드)
//		Node* current = openList.top();
//		openList.pop();
//
//		//현재 노드가 목적지에 도달한 경우 종료
//		if (current->position.x == goal.x && current->position.y == goal.y)
//		{
//			vector<Point> path;
//			Node* node = current;
//			while (node != nullptr)
//			{
//				path.push_back(node->position);
//				node = node->parent;
//			}
//			//경로 뒤집기
//			reverse(path.begin(), path.end());
//			return path;
//		}
//
//		//현재 노드를 방문한 노드들에 추가
//		visited.push_back(current);
//
//		//현재 노드의 이웃한 노드들에 대해 처리
//		int dx[4] = { 1, 0, -1, 0 };
//		int dy[4] = { 0, 1, 0, -1 };
//
//		for (int i = 0; i < 4; i++)
//		{
//			int nX = current->position.x + dx[i];
//			int nY = current->position.y + dy[i];
//
//			//이웃한 노드 생성
//			Node* neighbor = new Node(Point(nX, nY));
//
//			//이웃한 노드의 g, h, f 값 계산
//			neighbor->g = current->g + 1;
//			neighbor->h = abs(nX - goal.x) + abs(nY - goal.y);
//			neighbor->f = neighbor->g + neighbor->h;
//
//			//이웃한 노드가 범위를 벗어나는 경우 무시
//			if (nX < 0 || nX >= row || nY < 0 || nY >= col)
//			{
//				continue;
//			}
//
//			//벽인 경우 무시
//			if (maze[nX][nY] == WALL)
//			{
//				continue;
//			}
//
//			//이웃한 노드가 이미 방문한 노드인 경우 무시
//			bool isVisited = false;
//			for (Node* visitedNode : visited)
//			{
//				if (visitedNode->position.x == neighbor->position.x && visitedNode->position.y == neighbor->position.y)
//				{
//					isVisited = true;
//					break;
//				}
//			}
//			if (isVisited)
//			{
//				delete neighbor;
//				continue;
//			}
//
//			//이웃한 노드가 이미 오픈 리스트에 있는 경우 비용을 비교하여 더 작은 비용으로 갱신
//			bool isOpen = false;
//			for (Node* openNode : openList)
//			{
//				if (openNode->position.x == neighbor->position.x && openNode->position.y == neighbor->position.y)
//				{
//					isOpen = true;
//					if (neighbor-> g < openNode->g)
//					{
//						openNode->g = neighbor->g;
//						openNode->f = neighbor->f;
//						openNode->parent = current;
//					}
//					break;
//				}
//			}
//			if (isOpen)
//			{
//				delete neighbor;
//				continue;
//			}
//
//			//이웃한 노드를 오픈 리스트에 추가
//			openList.push(neighbor);
//		}
//	}
//
//	//목적지에 도달할 수 없는 경우 빈 벡터 반환
//	return vector<Point>();
//}
//
//int main(void)
//{
//	//노드의 개수와 시작점, 목적지 설정
//	int N, M;
//	cout << "Enter the number of nodes: ";
//	cin >> N;
//
//	int startX, startY;
//	cout << "Enter the starting point(x, y): ";
//	cin >> startX >> startY;
//
//	int goalX, goalY;
//	cout << "Enter the goal point(x, y): ";
//	cin >> goalX >> goalY;
//
//	//시작 노드와 목적지 노드 생성
//	Node* start = new Node(startX, startY);
//	Node* goal = new Node(goalX, goalY);
//
//	//A* 알고리즘 실행
//	vector<Node*> path = AStar(start, goal);
//
//	//결과 출력
//	if (path.empty())
//	{
//		cout << "Path not found." << endl;
//	}
//	else
//	{
//		cout << "Path found: " << endl;
//		for (int i = path.size() - 1; i >= 0; i--)
//		{
//			cout << "(" << path[i]->x << ", " << path[i]->y << ") ";
//		}
//		cout << endl;
//	}
//
//	//메모리 해제
//	delete start;
//	delete goal;
//
//	for (Node* node : path)
//	{
//		delete node;
//	}
//
//	return 0;
//}