//#include <iostream>
//#include <queue>
//#include <vector>
//
//using namespace std;
//
//class Node
//{
//public:
//	//����� x, y ��ǥ
//	int x, y;
//	//��������κ����� �̵� ���
//	int g;
//	//������������ ���� �̵� ��� 
//	int h;
//	//g + h
//	int f;
//
//	//�� ����� �θ� ���
//	Node* parent;
//
//	Node(int x, int y) :
//		x(x), y(y), g(0), h(0), f(0), parent(nullptr) {}
//
//	//f �� �� ���� ������ �����ε�
//	bool operator < (const Node& other) const
//	{
//		return f > other.f;
//	}
//};
//
//vector<Node*> AStar (Node* start, Node* goal)
//{
//	//���� ����Ʈ: ���� �湮���� ���� ����
//	priority_queue<Node*> openList;
//	openList.push(start);
//
//	//�湮�� ������ �����ϴ� �迭
//	vector<Node*> visited;
//
//	//���� ����� g���� h���� �ʱ�ȭ
//	start->g = 0;
//	start->h = abs(start->x - goal->x) + abs(start->y - goal->y);
//	start->f = start->g + start->h;
//
//	//�������� ������ ������ �ݺ�
//	while (!openList.empty())
//	{
//		//���� ��� ���� (f���� ���� ���� ���)
//		Node* current = openList.top();
//		openList.pop();
//
//		//���� ��尡 �������� ������ ��� ����
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
//		//���� ��带 �湮�� ���鿡 �߰�
//		visited.push_back(current);
//
//		//���� ����� �̿��� ���鿡 ���� ó��
//		int dx[4] = { 1, 0, -1, 0 };
//		int dy[4] = { 0, 1, 0, -1 };
//		
//		for (int i = 0; i < 4; i++)
//		{
//			int nX = current->x + dx[i];
//			int nY = current->y + dy[i];
//
//			//�̿��� ��尡 ������ ����� ��� ����
//			if (nX < 0 || nX >= N || nY < 0 || nY >= M)
//			{
//				continue;
//			}
//
//			//�̿��� ��� ����
//			Node* neighbor = new Node(nX, nY);
//			neighbor->g = current->g + 1;
//			neighbor->h = abs(neighbor->x - goal->x) + abs(neighbor->y - goal->y);
//			neighbor->f = neighbor->g + neighbor->h;
//			neighbor->parent = current;
//
//			//�̿��� ��尡 �̹� �湮�� ����� ��� ����
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
//			//�̿��� ��尡 �̹� ���� ����Ʈ�� �ִ� ��� ����� ���Ͽ� �� ���� ������� ����
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
//			//�̿��� ��带 ���� ����Ʈ�� �߰�
//			openList.push(neighbor);
//		}
//	}
//
//	//�������� ������ �� ���� ��� �� ���� ��ȯ
//	return vector<Node*>();
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