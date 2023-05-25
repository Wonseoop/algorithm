//#include <iostream>
//#include <queue>
//#include <vector>
//
//using namespace std;
//
//class Node
//{
//public:
//	//노드의 x, y 좌표
//	int x, y;
//	//출발점으로부터의 이동 비용
//	int g;
//	//목적지까지의 예상 이동 비용 
//	int h;
//	//g + h
//	int f;
//
//	//이 노드의 부모 노드
//	Node* parent;
//
//	Node(int x, int y) :
//		x(x), y(y), g(0), h(0), f(0), parent(nullptr) {}
//
//	//f 값 비교 위한 연산자 오버로딩
//	bool operator < (const Node& other) const
//	{
//		return f > other.f;
//	}
//};
//
//vector<Node*> AStar (Node* start, Node* goal)
//{
//	//오픈 리스트: 아직 방문하지 않은 노드들
//	priority_queue<Node*> openList;
//	openList.push(start);
//
//	//방문한 노드들을 저장하는 배열
//	vector<Node*> visited;
//
//	//시작 노드의 g값과 h값을 초기화
//	start->g = 0;
//	start->h = abs(start->x - goal->x) + abs(start->y - goal->y);
//	start->f = start->g + start->h;
//
//	//목적지에 도달할 때까지 반복
//	while (!openList.empty())
//	{
//		//현재 노드 선택 (f값이 가장 작은 노드)
//		Node* current = openList.top();
//		openList.pop();
//
//		//현재 노드가 목적지에 도달한 경우 종료
//		if (current == goal)
//		{
//			vector<Node*> path;
//			Node* node = current;
//			while (node != nullptr)
//			{
//				path.push_back(node);
//				node = node->parent;
//			}
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
//			int nX = current->x + dx[i];
//			int nY = current->y + dy[i];
//
//			//이웃한 노드가 범위를 벗어나는 경우 무시
//			if (nX < 0 || nX >= N || nY < 0 || nY >= M)
//			{
//				continue;
//			}
//
//			//이웃한 노드 생성
//			Node* neighbor = new Node(nX, nY);
//			neighbor->g = current->g + 1;
//			neighbor->h = abs(neighbor->x - goal->x) + abs(neighbor->y - goal->y);
//			neighbor->f = neighbor->g + neighbor->h;
//			neighbor->parent = current;
//
//			//이웃한 노드가 이미 방문한 노드인 경우 무시
//			bool isVisited = false;
//			for (Node* visitedNode : visited)
//			{
//				if (visitedNode->x == neighbor->x && visitedNode->y == neighbor->y)
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
//				if (openNode->x == neighbor->x && openNode->y == neighbor->y)
//				{
//					isOpen = true;
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
//	return vector<Node*>();
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