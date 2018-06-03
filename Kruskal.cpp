#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
using namespace std;

/***********边表***********/
class EdgeNode
{
public:
	int v;
	int w;
	int dist;
	EdgeNode *next;
	EdgeNode(int _v, int _w, int _dist,EdgeNode *n = NULL):v(_v), w(_w), dist(_dist),next(n) {}
};
/************顶点表z************/
class VertexNode
{
public:
	long int data;
	EdgeNode* firstEdge = NULL; 
};
class mycompare
{
public:
	bool reverse;
	mycompare(const bool &re = true)
	{
		reverse = re;
	}

	bool operator() (EdgeNode *a, EdgeNode *b) const
	{
		if(reverse) return (a->dist > b->dist); //从小到大排列
		else {
			return (a->dist < b->dist);
		}
	}
};
/***************图的数据结构***************/
class Graph
{
public:
	Graph()
	{
		ifstream fin("clustering1.txt");
		string line;
		stringstream stream;
		if(getline(fin, line))
		{
			stream << line;
			stream >> numVertexes;
		}
		init_vertex();
		while(getline(fin, line))
		{
			int v, w, dis;
			stream.clear();
			stream << line;
			stream >> v;
			stream >> w;
			stream >> dis;
			addEdge(v, w, dis);			
		}

	}
/*****************加边，头插法**************/
	void addEdge(int _v, int _w, int _dist)
	{
		EdgeNode *enode = new EdgeNode(_v-1, _w-1, _dist);
		enode->next = vertex[_v-1].firstEdge;
		vertex[_v-1].firstEdge = enode;
	}
/***************顶点表初始化*************/
	void init_vertex()
	{
		vertex.resize(numVertexes);
		for(int i = 0; i < numVertexes; i++)
		{
			vertex[i].data = i;	
		}
	}

public:
	int numVertexes;
//	int numEdges;
	vector<VertexNode> vertex;
//	vector<Edge> edges;
};
/***********并查集，quick-union，挺好用的***********/
class UnionFind
{
public:
	UnionFind(int len)
	{
		length = len;
		rank.resize(len);
		parent.resize(len);
		for(int i = 0; i < length; i++)
		{
			rank[i] = 0;
			parent[i] = i;
		}
	}

	bool unite(int x, int y)
	{
		int a = find(x);
		
		int b = find(y);
		if(a == b)
		{
			return false;
		} else {
			if(rank[a] == rank[b])
			{
				parent[a] = b;
				rank[b]++;
			} else if(rank[a] < rank[b]) {
				parent[a] = b;
			} else {
				parent[b] = a;
			}
		}
		return true;
	}
		
	
	int find(int x)
	{
		if(parent[x] == x)
		{
			return x;
		} else {
			return find(parent[x]);
		}
	}
	 

public:
	vector<int> rank;
	vector<int> parent;
	int length;
};
//优先队列根据边的权重排列,
//union-find来识别会形成环的边
/*************优先队列+并查集***********/
class Kruskal
{
public:
	Kruskal(int k, Graph graph)
	{

		length = graph.numVertexes;
		int cnt = length;
		UnionFind set(length);
		init_pq(graph);
		while(cnt > k)
		{
			EdgeNode *temp = new EdgeNode(0, 0, 0);			
			temp = pq.top();
			pq.pop();
			int x = temp->v;
			int y = temp->w;
			if(set.unite(x, y))
			{
				cnt--;
			}
			delete temp;
		}
		while(!pq.empty())
		{
			EdgeNode *p = new EdgeNode(0,0,0);
			p = pq.top();
			if(set.find(p->v) != set.find(p->w))
			{
				cout << "Max spacing:";
				cout <<  p->dist << endl;
				break;
			}
			pq.pop();
			delete p;
		}
		ofstream fout;
		fout.open("output.txt");
		for(int i = 0; i < length; i++)
		{
			fout << "parent[" << i << "]:" << set.parent[i] << endl;
		}
	}	

	void init_pq(Graph graph)
	{
		for(int i = 0; i < length; i++)	
		{
			EdgeNode *p = new EdgeNode(0,0,0);
			p = graph.vertex[i].firstEdge;
			while(p)
			{
				pq.push(p);
				p = p->next;
			}
			delete p;
		}
	}
public:
	int length;
	//优先队列
	priority_queue<EdgeNode*, vector<EdgeNode*>, mycompare> pq;
};

int main()
{
	clock_t start, end;
	start = clock();
	Graph graph;
	Kruskal kruskal(4, graph);
	end = clock();
	cout << "running time:" << (double)(end-start) / CLOCKS_PER_SEC << "s" << endl;
	return 0;
}






