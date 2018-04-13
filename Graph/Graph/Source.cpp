#include "Graph.h"
#include <iostream>

int main() {
	Graph g;
	/*g.addEdge(1, 2);
	g.addEdge(1, 3);
	g.addEdge(1, 4);
	g.addEdge(2, 4);
	g.addEdge(2, 5);
	g.addEdge(3, 6);
	g.addEdge(4, 3);
	g.addEdge(4, 6);
	g.addEdge(4, 7);
	g.addEdge(5, 4);
	g.addEdge(5, 7);
	g.addEdge(7, 6);
	g.print();
	g.topoSort();*/

	g.addEdge(1, 2, 2);
	g.addEdge(1, 4, 1);
	g.addEdge(2, 4, 3);
	g.addEdge(2, 5, 10);
	g.addEdge(3, 1, 4);
	g.addEdge(3, 6, 5);
	g.addEdge(4, 3, 2);
	g.addEdge(4, 5, 2);
	g.addEdge(4, 6, 8);
	g.addEdge(4, 7, 4);
	g.addEdge(5, 7, 6);
	g.addEdge(7, 6, 1);
	g.dijkstra(1);
	g.printPaths(1);
	system("pause");
}