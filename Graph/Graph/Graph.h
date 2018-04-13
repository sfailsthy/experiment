#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>
#include <map>

struct Vertex {
	int id;
	int indegree, outdegree;
	Vertex *prev;
	int dist;
	bool mark;
	std::vector<std::pair<Vertex*, int>> adj;//(from,to,weight);
	Vertex() = default;
	Vertex(int i) :id{ i } {
		prev = nullptr;
	}
};

struct Edge {
	Vertex *from, *to;
	int weight;
	Edge() = default;
	Edge(Vertex* u, Vertex *v, int w = 1) :
		from{ u }, to{ v }, weight{ w } {}
	bool operator<(const Edge& rhs) {
		return weight > rhs.weight;
	}
};

class Graph
{
public:
	Graph() = default;
	Graph(const Graph &rhs);
	Graph(Graph &&rhs);
	Graph& operator=(const Graph &rhs);
	Graph& operator=(Graph &&rhs);
	~Graph();

	void addEdge(int from, int to, int weight = 1);
	void topoSort();
	void unweighted(int from);
	void dijkstra(int from);
	void printPath(Vertex *v);
	void printPaths(int s);
	void print();

private:
	std::map<int, Vertex*> m;
	std::vector<Vertex*> vertices;
	std::vector<Edge> edges;
	int V, E;

	void computeDegree();
};
#endif // !GRAPH_H
