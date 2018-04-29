#pragma once

#ifndef POLYGLIST_H
#define POLYGLIST_H

#include <utility>
#include <vector>
#include <iostream>

using term = std::pair<int, std::vector<int>>;//系数，各指数域
enum elemTag { ATOM, LIST };

struct PolyNode
{
	elemTag tag;
	int exp;//指数域
	union {
		int coef;//系数域
		PolyNode *head;//表结点的表头指针
	};
	PolyNode *tail;
	PolyNode() :
		tag{ LIST }, head{ nullptr }, tail{ nullptr } {}

	PolyNode(elemTag t, int e) :
		tag{ t }, exp{ e }, head{ nullptr }, tail{ nullptr } {}
};

class PolyGList
{
	friend PolyGList operator+(const PolyGList&, const PolyGList&);
	friend PolyGList operator-(const PolyGList&, const PolyGList&);

public:
	PolyGList()=default;
	PolyGList(const PolyGList &rhs) {
		head = new PolyNode;
		head->tag = rhs.head->tag;
		head->exp = rhs.head->exp;
		head->head = rhs.head->head;
		head->tail = rhs.head->tail;
	}

	PolyGList& operator=(const PolyGList &rhs) {
		PolyGList copy = rhs;
		std::swap(*this, copy);
		return *this;
	}
	PolyGList& operator=(PolyGList &&rhs) {
		std::swap(head, rhs.head);
		return *this;
	}
	~PolyGList()=default;
	void create();
	void print() const {
		print(head->head);
		std::cout << std::endl;
	}

	int value(std::vector<int> &vec) const {
		return value(head->head, vec);
	}

	void negate() {
		negate(head->head);
	}
	
	PolyGList operator+=(const PolyGList &rhs);

private:
	//按照主变元的指数从大到小，即head的exp(指数域)最大
	PolyNode *head;
	PolyNode* createTerm(term t,int n);
	void insert(PolyNode *&p, PolyNode *pb);
	void print(PolyNode*) const;//有bug待调
	int value(PolyNode*, std::vector<int>&) const;
	void negate(PolyNode*);
};


PolyGList operator+(const PolyGList&, const PolyGList&);
PolyGList operator-(const PolyGList&, const PolyGList&);
#endif // !POLYGLIST_H
