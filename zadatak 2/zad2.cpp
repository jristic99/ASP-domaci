#include "Graph.h"
#include <iostream>


struct WeightedEdge
{
	Node *node1;
	Node *node2;
	int weight;
};

list<WeightedEdge> primsAlgorith(Graph graph)
{

}



void main()
{
	Graph graph;
	int n = 6;
	graph.initializeGraph(6);
	graph.addEdge(1, 2, 18);
	graph.addEdge(1, 3, 33);
	graph.addEdge(1, 5, 19);
	graph.addEdge(2, 3, 14);
	graph.addEdge(2, 4, 10);
	graph.addEdge(2, 6, 6);
	graph.addEdge(3, 5, 21);
	graph.addEdge(3, 6, 11);
	graph.addEdge(4, 6, 5);
	graph.addEdge(5, 6, 16);

	graph.printGraph();
		
}