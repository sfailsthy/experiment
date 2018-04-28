#pragma once
#ifndef CROSSLIST_H
#define CROSSLIST_H

#include <vector>
#include <iostream>

struct node {
	int i, j;
	int val;
	node *right, *down;

	node(int _i, int _j, int x, node *_right = nullptr, node *_down = nullptr) :
		i(_i), j(_j), val(x), right(_right), down(_down) {}
};

class CrossList
{
	friend std::ostream& operator<<(std::ostream&, const CrossList&);
	friend CrossList operator+(const CrossList&, const CrossList&);

public:
	CrossList() = default;
	CrossList(const std::vector<std::vector<int>> &arr);
	CrossList(const CrossList &rhs);
	CrossList(CrossList &&rhs);
	CrossList& operator=(const CrossList &rhs);
	CrossList& operator=(CrossList &&rhs);
	~CrossList() {
		rhead.clear();
		chead.clear();
	}

	std::vector<std::vector<int>> getArray() const;
	CrossList transpose() const;

	void print() {
		std::cout << row << " , " << col << " , " << cnt << std::endl;
	}
private:
	int row, col, cnt;
	std::vector<node*> rhead, chead;

	void insert(node *p);
};

std::ostream& operator<<(std::ostream&, const CrossList&);
CrossList operator+(const CrossList&, const CrossList&);

#endif // !CROSSLIST_H
