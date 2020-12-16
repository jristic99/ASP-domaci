#include "Graph.h"
#include <iostream>

void Graph::initializeGraph(int n)
{
	freeGraph();
	for (int i = 0; i < n; i++)
	{
		headers.push_back(NodeHeader());
	}
}

void Graph::printGraph()
{
	for (NodeHeader header : headers)
	{
		if (header.edges.empty())
			cout << "Cvor " << header.node->getId() << " nema susede" << endl;
		else {
			cout << "Cvor " << header.node->getId() << " ima susede:";
			for (Edge edge : header.edges)
			{
				cout << " " << edge.neighbour->getId();
				if (edge.weight != 0) cout << "(" << edge.weight << ")";
			}
			cout << endl;
		}
	}
}

void Graph::addNode()
{
	headers.push_back(NodeHeader());
}

void Graph::deleteNode(int id)
{
	bool found = false;
	for(NodeHeader &header : headers)
	{
		if (header.node->getId() != id)
		{
			list<Edge> tmp_e = header.edges;
			for (Edge edge : tmp_e)
				if (edge.neighbour->getId() == id)
					header.edges.remove(edge);
		}
	}


	list<NodeHeader> tmp_h = headers;
	for (NodeHeader &header : tmp_h)
	{
		if (header.node->getId() == id)
		{
			headers.remove(header);
			delete header.node;
			found = true;
		}
	}

	if (found == false)
	{
		cout << "Ne postoji cvor tog indeksa" << endl;
		return;
	}
}

void Graph::addEdge(int id1, int id2, int weight)
{
	Node *node1 = findNode(id1); 
	Node *node2 = findNode(id2);

	if (node1 == nullptr)
	{
		cout << "Ne postoji cvor indeksa " << id1;
		return;
	}

	if (node2 == nullptr)
	{
		cout << "Ne postoji cvor indeksa " << id2;
		return;
	}

	for (NodeHeader &header : headers)
	{
		if (header.node->getId() == id1)
			header.edges.push_back(Edge(node2, weight));

		if (header.node->getId() == id2)
			header.edges.push_back(Edge(node1, weight));
	}

}

void Graph::deleteEdge(int id1, int id2)
{
	Node *node1 = findNode(id1);
	Node *node2 = findNode(id2);

	if (node1 == nullptr)
	{
		cout << "Ne postoji cvor indeksa" << id1;
		return;
	}

	if (node2 == nullptr)
	{
		cout << "Ne postoji cvor indeksa" << id2;
		return;
	}


	deleteEdge(node1, node2);
	deleteEdge(node2, node1);
	
}

Node *Graph::findNode(int id)
{
	for (auto header : headers)
		if (header.node->getId() == id)
			return header.node;
	return nullptr;
}

void Graph::deleteEdge(Node *node1, Node *node2)
{
	for (auto &header : headers)
		if (header.node == node1)
		{
			list<Edge> tmp_e = header.edges;
			for (auto edge : tmp_e)
				if (edge.neighbour == node2)
				{
					header.edges.remove(edge);
					break;
				}
		}
}

void Graph::freeGraph()
{
	for (auto header : headers)
		delete header.node;
	headers = {};
}

Graph::~Graph()
{
	freeGraph();
}

bool operator==(const Edge &e1, const Edge &e2)
{
	bool b = (e1.neighbour->getId() == e2.neighbour->getId());
	return b;
}

bool operator==(const NodeHeader &nh1, const NodeHeader &nh2)
{
	bool b = (nh1.node->getId() == nh2.node->getId());
	return b;
}
