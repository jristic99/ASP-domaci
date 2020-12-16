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

class Graph{
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