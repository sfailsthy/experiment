#include "Graph.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <limits>
#include <functional>

const int infinity = std::numeric_limits<int>::max();

Graph::Graph(const Graph &rhs) :
	vertices(rhs.vertices.size()), edges(rhs.edges.size()),
	V{ rhs.V }, E{ rhs.E } {
	for (int i = 0; i != rhs.vertices.size(); ++i) {
		vertices[i] = rhs.vertices[i];
	}
	for (int i = 0; i != rhs.edges.size(); ++i) {
		edges[i] = rhs.edges[i];
	}
	for (auto &item : rhs.m) {
		m[item.first] = item.second;
	}
}

Graph::Graph(Graph &&rhs) :
	vertices{ std::move(rhs.vertices) }, edges{ std::move(rhs.edges) },
	m{ std::move(rhs.m) }, V{ rhs.V }, E{ rhs.E } {}

Graph& Graph::operator=(const Graph &rhs) {
	Graph copy = rhs;
	std::swap(*this, copy);
	return *this;
}

Graph& Graph::operator=(Graph &&rhs) {
	std::swap(vertices, rhs.vertices);
	std::swap(edges, rhs.edges);
	std::swap(m, rhs.m);
	std::swap(V, rhs.V);
	std::swap(E, rhs.E);
	return *this;
}

Graph::~Graph() {
	vertices.clear();
	edges.clear();
	m.clear();
}

void Graph::addEdge(int from, int to, int weight) {
	Vertex *u = nullptr, *v = nullptr;
	if (m.find(from) != m.end()) {
		u = m[from];
	}
	else {
		u = new Vertex{ from };
	}

	if (m.find(to) != m.end()) {
		v = m[to];
	}
	else {
		v = new Vertex{ to };
	}

	u->adj.push_back({ v,weight });
	edges.push_back(Edge{ u,v,weight });

	if (m.find(from) == m.end()) {
		vertices.push_back(u);
		m[from] = u;
	}

	if (m.find(to) == m.end()) {
		vertices.push_back(v);
		m[to] = v;
	}
	V = vertices.size();
	E = edges.size();
}

void Graph::print() {
	computeDegree();
	std::cout << "Vertices: ";
	for (auto &v : vertices) {
		std::cout << v->id << " ";
	}
	std::cout << std::endl;
	std::cout << "Vertices indegree outdegree" << std::endl;
	for (auto &v : vertices) {
		std::cout << v->id << "        "<<v->indegree<<"        "<<v->outdegree<<std::endl;
	}

	std::cout << "Edges:" << std::endl;
	for (auto &v : vertices) {
		for (auto &item : v->adj) {
			auto w = item.first;
			std::cout << v->id << "--->" << w->id << ": " << item.second << std::endl;
		}
	}
}

void Graph::computeDegree() {
	for (auto &v : vertices) {
		v->indegree = 0;
		v->outdegree = 0;
	}

	for (auto &v : vertices) {
		for (auto &item : v->adj) {
			auto &w = item.first;
			w->indegree++;
			v->outdegree++;
		}
	}
}

void Graph::topoSort() {
	computeDegree();
	std::queue<Vertex*> que;
	std::vector<Vertex*> order;
	int counter = 0;

	for (auto &v : vertices) {
		if (v->indegree == 0) {
			que.push(v);
		}
	}

	while (!que.empty()) {
		std::cout << "Indegree: ";
		for (auto &v : vertices) {
			std::cout << v->indegree << " ";
		}
		std::cout << std::endl;

		auto v = que.front();
		que.pop();
		order.push_back(v);
		++counter;

		for (auto &item : v->adj) {
			auto w = item.first;
			if (--w->indegree == 0) {
				que.push(w);
			}
		}
	}

	if (counter != V) {
		std::cerr << "A cycle was found!" << std::endl;
	}
	else {
		std::cout << "toposort: ";
		for (auto &v : order) {
			std::cout << v->id << " ";
		}
		std::cout << std::endl;
	}
}

void Graph::unweighted(int from) {
	std::queue<Vertex*> que;
	for (auto &v : vertices) {
		v->dist = infinity;
	}

	Vertex *s = m[from];
	s->dist = 0;
	que.push(s);

	while (!que.empty()) {
		Vertex *v = que.front();
		que.pop();

		for (auto &item : v->adj) {
			Vertex *w = item.first;
			if (w->dist == infinity) {
				w->dist = v->dist + 1;
				w->prev = v;
				que.push(w);
			}
		}
	}
}

void Graph::printPath(Vertex *v) {
	if (v->prev != nullptr) {
		printPath(v->prev);
		std::cout << " --> ";
	}
	std::cout << v->id;
}

void Graph::printPaths(int s) {
	for (auto &v : vertices) {
		if (v->id != s) {
			std::cout << s << " to " << v->id << "(dist: " << v->dist << "): ";
			printPath(v);
			std::cout << std::endl;
		}
	}
}

void Graph::dijkstra(int from) {
	Vertex *s = m[from];

	for (auto &v : vertices) {
		v->dist = infinity;
	}

	s->dist = 0;
	using P = std::pair<int, Vertex*>;//(to,dist)
	std::priority_queue<P, std::vector<P>, std::greater<P>> que;
	que.push(P(0,s));

	while (!que.empty()) {
		P p = que.top();
		que.pop();

		Vertex *v = p.second;
		if (v->dist < p.first) continue;

		for (auto &item : v->adj) {
			Vertex *w = item.first;
			int dist = item.second;
			if (v->dist + dist < w->dist) {
				w->dist = v->dist + dist;
				w->prev = v;
				que.push(P(w->dist, w));
			}
		}
	}
}
