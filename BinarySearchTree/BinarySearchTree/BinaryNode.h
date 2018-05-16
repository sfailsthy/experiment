#pragma once
#ifndef BINARYNODE_H
#define BINARYNODE_H

#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <utility>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <queue>
#include <string>
#include <list>

enum tag { link, thread };

template<typename Object>
struct BinaryNode
{
	Object val;
	BinaryNode *parent;
	BinaryNode *left;
	BinaryNode *right;
	int height;
	tag ltag, rtag;

	BinaryNode(const Object &x, BinaryNode *p = nullptr,
		BinaryNode *l = nullptr, BinaryNode *r = nullptr, int h = 0) :
		val{ x }, parent{ p }, left{ l }, right{ r }, height{ h } {
		ltag = rtag = link;
	}

	BinaryNode(Object &&x, BinaryNode *p = nullptr, BinaryNode *l = nullptr,
		BinaryNode *r = nullptr, int h = 0) :
		val{ std::move(x) }, parent{ p }, left{ l }, right{ r }, height{ h } {
		ltag = rtag = link;
	}


	BinaryNode(const Object &x, BinaryNode *l = nullptr, BinaryNode *r = nullptr, int h = 0) :
		val{ x }, left{ l }, right{ r }, height{ h } {
		ltag = rtag = link;
	}
	BinaryNode(Object &&x, BinaryNode *l = nullptr, BinaryNode *r = nullptr, int h = 0) :
		val{ std::move(x) }, left{ l }, right{ r }, height{ h } {
		ltag = rtag = link;
	}
	
};

/*
BinaryNode* creat(int depth = 0)
{
	int x;
	std::cin >> x;
	if (x == -1) {
		return nullptr;
	}
	else {
		BinaryNode *root = new BinaryNode;
		root->val = x;
		root->depth = depth;
		root->left = creat(depth + 1);
		root->right = creat(depth + 1);
		return root;
	}
}
*/

#endif // !BINARYNODE_H
