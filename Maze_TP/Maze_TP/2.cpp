//#include <iostream>
//#include <queue>
//#include <vector>
//
//using namespace std;
//
////�̷��� ũ�� (��)
//const int row = 5;
//const int col = 5;
//
////�̷��� ���� ��Ÿ���� ���
//const int WALL = 1;
//
//class Point
//{
//public:
//	//����� x, y ��ǥ
//	int x, y;
//
//	Point(int x, int y) : x(x), y(y) {}
//};
//
//class Node
//{
//public:
//	//����� ��ġ
//	Point position;
//	//��������κ����� �̵� ���
//	int g;
//	//������������ ���� �̵� ��� 
//	int h;
//	//g + h
//	int f;
//	//�� ����� �θ� ���
//	Node* parent;
//
//	Node(Point position) : position(position), g(0), h(0), f(0), parent(nullptr) {}
//
//	//f �� �� ���� ������ �����ε�
//	bool operator < (const Node& other) const
//	{
//		return f > other.f;
//	}
//};
//
//vector<Point> AStar(int maze[row][col], Point start, Point goal)
//{
//	//���� ����Ʈ: ���� �湮���� ���� ����
//	priority_queue<Node*> openList;
//
//	//�湮�� ������ �����ϴ� �迭
//	vector<Node*> visited;
//
//	//���� ����� g���� h���� �ʱ�ȭ
//	Node* startNode = new Node(start);
//	startNode->g = 0;
//	startNode->h = abs(start.x - goal.x) + abs(start.y - goal.y);
//	startNode->f = startNode->g + startNode->h;
//
//	//���� ��带 ���� ����Ʈ�� �߰�
//	openList.push(startNode);
//
//	//�������� ������ ������ �ݺ�
//	while (!openList.empty())
//	{
//		//���� ��� ���� (f���� ���� ���� ���)
//		Node* current = openList.top();
//		openList.pop();
//
//		//���� ��尡 �������� ������ ��� ����
//		if (current->position.x == goal.x && current->position.y == goal.y)
//		{
//			vector<Point> path;
//			Node* node = current;
//			while (node != nullptr)
//			{
//				path.push_back(node->position);
//				node = node->parent;
//			}
//			//��� ������
//			reverse(path.begin(), path.end());
//			return path;
//		}
//
//		//���� ��带 �湮�� ���鿡 �߰�
//		visited.push_back(current);
//
//		//���� ����� �̿��� ���鿡 ���� ó��
//		int dx[4] = { 1, 0, -1, 0 };
//		int dy[4] = { 0, 1, 0, -1 };
//
//		for (int i = 0; i < 4; i++)
//		{
//			int nX = current->position.x + dx[i];
//			int nY = current->position.y + dy[i];
//
//			//�̿��� ��� ����
//			Node* neighbor = new Node(Point(nX, nY));
//
//			//�̿��� ����� g, h, f �� ���
//			neighbor->g = current->g + 1;
//			neighbor->h = abs(nX - goal.x) + abs(nY - goal.y);
//			neighbor->f = neighbor->g + neighbor->h;
//
//			//�̿��� ��尡 ������ ����� ��� ����
//			if (nX < 0 || nX >= row || nY < 0 || nY >= col)
//			{
//				continue;
//			}
//
//			//���� ��� ����
//			if (maze[nX][nY] == WALL)
//			{
//				continue;
//			}
//
//			//�̿��� ��尡 �̹� �湮�� ����� ��� ����
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
//			//�̿��� ��尡 �̹� ���� ����Ʈ�� �ִ� ��� ����� ���Ͽ� �� ���� ������� ����
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
//			//�̿��� ��带 ���� ����Ʈ�� �߰�
//			openList.push(neighbor);
//		}
//	}
//
//	//�������� ������ �� ���� ��� �� ���� ��ȯ
//	return vector<Point>();
//}
//
//int main(void)
//{
//	//����� ������ ������, ������ ����
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
//	//���� ���� ������ ��� ����
//	Node* start = new Node(startX, startY);
//	Node* goal = new Node(goalX, goalY);
//
//	//A* �˰��� ����
//	vector<Node*> path = AStar(start, goal);
//
//	//��� ���
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
//	//�޸� ����
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