// ------------------------------- Graph.h -------------------------------
#ifndef GRAPH_H
#define GRAPH_H

#include <list>
using namespace std;

struct Node {
public:
	Node() {
		static int count = 1;
		id = count; count++;
	}

	int getId() {
		return id;
	}

private:
	int id;
};

struct Edge {
public:
	Node* neighbour;
	int weight;
	bool builded;

	Edge(Node* neighbour, int weight) {
		this->weight = weight;
		this->neighbour = neighbour;
	}

	Edge() {
		neighbour = nullptr;
		weight = 0;
	}

	friend bool operator==(const Edge& e1, const Edge& e2);
};

struct NodeHeader {
public:
	Node* node;
	list<Edge> edges;

	NodeHeader() {
		node = new Node();
		edges = {};
	}

	friend bool operator==(const NodeHeader& nh1, const NodeHeader& nh2);
};

class Graph {
public:
	void initializeGraph(int n);
	void printGraph();
	void addNode();
	void deleteNode(int id);
	void addEdge(int id1, int id2, int weight);
	void deleteEdge(int id1, int id2);
	void freeGraph();

	~Graph();
	list<NodeHeader> getHeaders() {
		return headers;
	}
	int getNumNodes() {
		return (int)headers.size();
	}

	NodeHeader *getHeader(int id) {
		for (auto &header : headers)
			if (header.node->getId() == id)
				return &header;
		return nullptr;
	}

	Node* findNode(int id);
private:
	void deleteEdge(Node *node1, Node *node2);
	list<NodeHeader> headers;


};

#endif




// ------------------------------- Graph.cpp -------------------------------

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
	for (NodeHeader &header : headers)
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



// ------------------------------- main.cpp -------------------------------

#include "Graph.h"
#include <iostream>


void printMenu()
{
	cout << " 1 - Kreiranje praznog grafa od n cvorova" << endl
		<< " 2 - Dodavanje cvora" << endl
		<< " 3 - Uklanjanje cvora" << endl
		<< " 4 - Dodavanje grane" << endl
		<< " 5 - Uklanjanje grane" << endl
		<< " 6 - Ispis grafa" << endl
		<< " 7 - Brisanje grafa" << endl
		<< " 8 - Prekid programa " << endl
		<< " 0 - Prikaz menija " << endl;
}

void main()
{
	Graph *graph = new Graph();

	int option;
	printMenu();

	while (1)
	{
		cout << " Uneti zeljeni broj: ";
		cin >> option;
		int id1, id2;
		switch (option)
		{
		case 1:
			cout << " Uneti broj n: ";
			int n;
			cin >> n;
			graph->initializeGraph(n);
			break;

		case 2:
			graph->addNode();
			break;

		case 3:
			cout << " Uneti indeks cvora: ";
			int id;
			cin >> id;
			graph->deleteNode(id);
			break;

		case 4:
			cout << " Uneti 2 indeksa cvorova: ";
			cin >> id1 >> id2;
			graph->addEdge(id1, id2, 0);
			break;

		case 5:
			cout << " Unesite 2 indeksa cvorova: ";
			cin >> id1 >> id2;
			graph->deleteEdge(id1, id2);
			break;

		case 6:
			cout << "------------------" << endl;
			graph->printGraph();
			cout << "------------------";
			break;

		case 7:
			graph->freeGraph();
			delete graph;
			graph = new Graph();
			break;

		case 8:
			exit(1);
			break;

		case 0:
			printMenu();
			break;

		default:
			cout << " Pogresan unos" << endl;
			break;
		}
		cout << endl;
	}
}