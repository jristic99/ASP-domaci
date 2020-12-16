#include "Graph.h"
#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>

struct WeightedEdge
{
public:
	WeightedEdge() {}
	WeightedEdge(Node *node1, Node *node2, int weight) {
		this->node1 = node1;
		this->node2 = node2;
		this->weight = weight;
	}

	Node *node1;
	Node *node2;
	int weight;

	friend bool operator<(const WeightedEdge& we1, const WeightedEdge& we2);
};


bool operator<(const WeightedEdge & we1, const WeightedEdge & we2)
{
	return (we1.weight > we2.weight);
}


list<WeightedEdge> primsAlgorith(Graph &graph)
{
	list<NodeHeader> headers = graph.getHeaders();
	set<Node*> visited = {};
	priority_queue<WeightedEdge> pq_edges = {};
	unsigned int num_nodes = graph.getNumNodes();
	list<WeightedEdge> minSpannigTree = {};

	NodeHeader first = headers.front();
	visited.insert(first.node);

	for (auto edge : first.edges)
		pq_edges.push(WeightedEdge(first.node, edge.neighbour, edge.weight));

	while (visited.size() < num_nodes)
	{
		WeightedEdge next_edge;
		Node *added_node;
		do
		{
			next_edge = pq_edges.top();
			pq_edges.pop();
			added_node = next_edge.node2;
		} while (visited.find(next_edge.node1) != visited.end() &&
			visited.find(next_edge.node2) != visited.end());

		minSpannigTree.push_back(next_edge);
		visited.insert(added_node);

		NodeHeader *added_node_header = nullptr;
		for (auto &header : headers)
			if (header.node == added_node)
			{
				added_node_header = &header;
				break;
			}

		for (auto edge : added_node_header->edges)
		{
			if (visited.find(edge.neighbour) == visited.end())
				pq_edges.push(WeightedEdge(added_node, edge.neighbour, edge.weight));
		}
	}

	return minSpannigTree;
}

void printEdges(list<WeightedEdge> list)
{
	for (auto edge : list)
		cout << " " << edge.node1->getId() << "-" << edge.node2->getId() << " tezine: " << edge.weight << endl;
}


void printAllPaths(vector<vector<vector<int>>> paths, vector<int> dist, int v, set<pair<int, int>> unbuilt)
{
	vector<vector<pair<int, vector<int>>>> paths_sorted;

	paths_sorted.push_back({});
	for (unsigned int i = 1; i < paths.size(); i++)
	{
		paths_sorted.push_back({});
		if (i != v)
		{
			for (unsigned int j = 0; j < paths[i].size(); j++)
			{
				int numUnbuilt = 0;
				paths_sorted[i].push_back({});
				unsigned int k;
				for (k = 0; k < paths[i][j].size() - 1; k++)
				{
					int n1 = paths[i][j][k];
					int n2 = paths[i][j][k + 1];
					if (unbuilt.find(make_pair(n1, n2)) != unbuilt.end())
						numUnbuilt++;
				}
				int last_node = paths[i][j][k];
				if (unbuilt.find(make_pair(last_node, i)) != unbuilt.end())
					numUnbuilt++;

				paths_sorted[i][j] = make_pair(numUnbuilt, paths[i][j]);
			}
		}
		vector <pair<int, vector<int>>> temp = paths_sorted[i];
		sort(temp.begin(), temp.end());

	}

	for (unsigned int i = 1; i < paths_sorted.size(); i++)
		if (i != v)
		{
			cout << "Putanje do cvora " << i << " su minimalne duzine " << dist[i] << ":" << endl;
			for (unsigned int j = 0; j < paths_sorted[i].size(); j++)
			{
				for (unsigned int k = 0; k < paths_sorted[i][j].second.size(); k++)
				{
					cout << paths_sorted[i][j].second[k] << " ";
				}
				cout << i << endl << "Broj neizgradjenih puteva: " << paths_sorted[i][j].first << endl;
			}
			cout << endl;
		}

}

void dijkstrasAlgorithm(Graph &graph, int v_id, set<pair<int, int>> unbuilt)
{
	list<NodeHeader> headers = graph.getHeaders();
	NodeHeader *v_header = nullptr;
	Node *v = nullptr;
	for (auto &header : headers)
		if (header.node->getId() == v_id)
		{
			v = header.node;
			v_header = &header;
			break;
		}

	int n = graph.getNumNodes();
	vector<int> dist(n + 1, INT_MAX);
	vector<bool> visited(n + 1, 0);

	vector<vector<vector<int>>> all_paths(n + 1, vector<vector<int>>(0, vector<int>()));


	dist[v_id] = 0; visited[v_id] = true;
	all_paths[v_id].push_back({ v_id });


	vector<vector<int>> prev_node(n + 1, vector<int>(0)); //mozda ne treba
	vector<int> num_of_paths(n + 1, 0); // mozda ne treba



	for (auto &edge : v_header->edges)
	{
		int curr_node = edge.neighbour->getId();
		dist[curr_node] = edge.weight;
		prev_node[curr_node] = { v_id };
		all_paths[curr_node].push_back({ v_id });
		num_of_paths[curr_node] = 1;
	}

	for (int k = 1; k < n; k++)
	{
		int curr_min = INT_MAX;
		int i_min = 0;
		for (unsigned int i = 0; i < dist.size(); i++)
			if (visited[i] == false && dist[i] < curr_min)
			{
				curr_min = dist[i];
				i_min = i;
			}

		visited[i_min] = true;


		NodeHeader *curr_header = graph.getHeader(i_min);

		for (auto &edge : curr_header->edges)
		{
			int j = edge.neighbour->getId();
			if (visited[j] == false)
			{
				if (dist[j] > dist[i_min] + edge.weight)
				{
					dist[j] = dist[i_min] + edge.weight;
					prev_node[j] = { i_min };
					num_of_paths[i_min] = 1;
					all_paths[j].clear();

					for (unsigned int p = 0; p < all_paths[i_min].size(); p++)
					{
						vector<int> temp = all_paths[i_min][p];
						temp.push_back(i_min);
						all_paths[j].push_back(temp);
					}
				}
				else if (dist[j] == dist[i_min] + edge.weight)
				{
					num_of_paths[i_min]++;
					prev_node[j].push_back(i_min);

					for (unsigned int p = 0; p < all_paths[i_min].size(); p++)
					{
						vector<int> temp = all_paths[i_min][p];
						temp.push_back(i_min);
						all_paths[j].push_back(temp);
					}
				}
			}
		}
	}
	//printPaths(prev_node, n, v_id);
	printAllPaths(all_paths, dist, v_id, unbuilt);
	int bla = 5;
}


int main()
{
	Graph *graph;
	set<pair<int, int>> unbuilt;

	while (1)
	{
		graph = new Graph();
		cout << "Unesite broj cvorova grafa: ";
		int n;
		cin >> n;
		graph->initializeGraph(n);

		cout << "1. Minimalno obuhvatno stablo - Primov algoritam" << endl
			<< "2. Najkrace rastojanje od jednog cvora do svih ostalih - Dajkstrin algoritam" << endl
			<< "3. Prekid programa" << endl;

		cout << "Izabrati problem: ";
		int problem;
		cin >> problem;

		list<WeightedEdge> minSpanningTree;

		switch (problem)
		{
		case 1:
			while (1)
			{
				cout << "Uneti granu i njenu tezinu. Za prekid dodavanja grana uneti 0: " << endl;
				int n1, n2, w;
				cin >> n1;
				if (n1 == 0) break;
				cin >> n2 >> w;
				graph->addEdge(n1, n2, w);
			}
			minSpanningTree = primsAlgorith(*graph);
			printEdges(minSpanningTree);
			cout << endl;
			break;

		case 2:
			unbuilt = {};
			cout << "Uneti indeks v: ";
			int v;
			cin >> v;

			while (1)
			{
				cout << "Uneti izgradjen put i njegovu duzinu. Za prekid dodavanja grana uneti 0: " << endl;
				int n1, n2, w;
				cin >> n1;
				if (n1 == 0) break;
				cin >> n2 >> w;
				graph->addEdge(n1, n2, w);
			}

			while (1)
			{
				cout << "Uneti neizgradjen put i njegovu mogucu duzinu. Za prekid dodavanja grana uneti 0: " << endl;
				int n1, n2, w;
				cin >> n1;
				if (n1 == 0) break;
				cin >> n2 >> w;
				graph->addEdge(n1, n2, w);
				unbuilt.insert(make_pair(n1, n2)); unbuilt.insert(make_pair(n2, n1));
			}
			dijkstrasAlgorithm(*graph, v, unbuilt);
			break;
		case 3:
			exit(1);
			break;
		default:
			cout << "Neispravan unos";
			break;
		}
		delete graph;
	}

	return 0;
}
