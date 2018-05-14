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

template<typename Object>
struct BinaryNode
{
	Object val;
	BinaryNode *parent;
	BinaryNode *left;
	BinaryNode *right;
	int height;

	BinaryNode(const Object &x, BinaryNode *p = nullptr,
		BinaryNode *l = nullptr, BinaryNode *r = nullptr, int h = 0) :
		val{ x }, parent{ p }, left{ l }, right{ r }, height{ h } {}
	BinaryNode(Object &&x, BinaryNode *p = nullptr, BinaryNode *l = nullptr,
		BinaryNode *r = nullptr, int h = 0) :
		val{ std::move(x) }, parent{ p }, left{ l }, right{ r }, height{ h } {}


	BinaryNode(const Object &x, BinaryNode *l = nullptr, BinaryNode *r = nullptr, int h = 0) :
		val{ x }, left{ l }, right{ r }, height{ h } {}
	BinaryNode(Object &&x, BinaryNode *l = nullptr, BinaryNode *r = nullptr, int h = 0) :
		val{ std::move(x) }, left{ l }, right{ r }, height{ h } {}
	
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


template<typename Object>
void preorder(BinaryNode<Object> *root, int indent = 0) {
	if (root) {
		std::cout << root->val << " ";
		preorder(root->left);
		preorder(root->right);
	}
}

template<typename Object>
void inorder(BinaryNode<Object> *root) {
	if (root) {
		inorder(root->left);
		std::cout << root->val << " ";
		inorder(root->right);
	}
}

template<typename Object>
void postorder(BinaryNode<Object> *root) {
	if (root) {
		postorder(root->left);
		postorder(root->right);
		std::cout << root->val << " ";
	}
}

template<typename Object>
void bfs(BinaryNode<Object> *root) {
	if (root) {
		std::queue<BinaryNode<Object>*> que;
		que.push(root);

		while (!que.empty()) {
			BinaryNode<Object> *p = que.front();
			std::cout << p->val << " ";
			que.pop();
			if (p->left) {
				que.push(p->left);
			}
			if (p->right) {
				que.push(p->right);
			}
		}
		std::cout << std::endl;
	}
}

template<typename Object>
int maxLevel(BinaryNode<Object>* node) {
	if (node == nullptr) {
		return 0;
	}
	return std::max(maxLevel(node->left), maxLevel(node->right)) + 1;
}

template<typename Object>
bool all_is_null(std::vector<BinaryNode<Object>*> nodes) {
	for (BinaryNode<Object>* node : nodes) {
		if (node != nullptr) {
			return false;
		}
	}
	return true;
}

void print_white_spaces(int count) {
	for (int i = 0; i != count; ++i) {
		std::cout << " ";
	}
}

template<typename Object>
void printNodes(std::vector<BinaryNode<Object>*> nodes, int level, int max_level) {
	int floor = max_level - level;
	int endgeLines = static_cast<int>(pow(2, std::max(floor - 1, 0)));
	int firstSpaces = static_cast<int>(pow(2, floor) - 1);
	int betweenSpaces = static_cast<int>(pow(2, floor + 1) - 1);
	print_white_spaces(firstSpaces);

	std::vector<BinaryNode<Object>*> newNodes;
	for (BinaryNode<Object>* node : nodes) {
		if (node != nullptr) {
			std::cout << node->val;
			newNodes.push_back(node->left);
			newNodes.push_back(node->right);
		}
		else {
			newNodes.push_back(nullptr);
			newNodes.push_back(nullptr);
			std::cout << " ";
		}

		print_white_spaces(betweenSpaces);
	}
	std::cout << std::endl;

	if (all_is_null(newNodes)) {
		return;
	}

	for (int i = 1; i <= endgeLines; i++) {
		for (int j = 0; j < nodes.size(); j++) {
			print_white_spaces(firstSpaces - i);
			if (nodes[j] == nullptr) {
				print_white_spaces(endgeLines * 2 + i + 1);
				continue;
			}

			if ((nodes[j]->left) != nullptr) {
				std::cout << "/";
			}
			else {
				print_white_spaces(1);
			}
			print_white_spaces(2 * i - 1);

			if ((nodes[j]->right) != nullptr) {
				std::cout << "\\";
			}
			else {
				print_white_spaces(1);
			}
			print_white_spaces(endgeLines * 2 - i);
		}
		std::cout << std::endl;
	}

	printNodes(newNodes, level + 1, max_level);

}

template<typename Object>
void printTree(BinaryNode<Object> *root) {
	int max_level = maxLevel(root);
	printNodes(std::vector<BinaryNode<Object>*>{root}, 1, max_level);
}

template<typename Object>
std::pair<BinaryNode<Object>*,std::vector<std::pair<Object, std::string>>>
 huffmanEncode(const std::vector<Object> &vec) {
	BinaryNode<Object> *left, *right, *parent;
	auto cmp = [](const BinaryNode<Object> *lhs, const BinaryNode<Object> *rhs) {
		return lhs->val > rhs->val;
	};
	std::priority_queue<BinaryNode<Object>*, std::vector<BinaryNode<Object>*>, decltype(cmp)> que(cmp);
	std::vector<BinaryNode<Object>*> nodes;

	for (int i = 0; i < vec.size(); ++i) {
		BinaryNode<Object> *p = new BinaryNode<Object>{ vec[i] ,nullptr,nullptr,nullptr };
		que.push(p);
		nodes.push_back(p);
	}

	while (que.size() > 1) {
		left = que.top();
		que.pop();
		right = que.top();
		que.pop();

		parent = new BinaryNode<Object>{ left->val + right->val,nullptr,left,right };
		left->parent = right->parent = parent;
		que.push(parent);
	}
	
	std::vector<std::pair<Object, std::string>> codes;
	for (int i = 0; i < vec.size(); ++i) {
		std::string str = "";
		BinaryNode<Object> *cur, *par;
		for (cur = nodes[i], par = nodes[i]->parent; par != nullptr; cur = par, par = par->parent) {
			if (par->left == cur) {
				str += "0";
			}
			else {
				str += "1";
			}
			
		}
		std::reverse(str.begin(), str.end());
		codes.push_back({ nodes[i]->val, str });
	}
	return { que.top(),codes };
}

template<typename Object>
void huffmanDecode(const std::string &str, BinaryNode<Object> *root) {
	BinaryNode<Object> *p = root;
	for (char c : str) {
		if (c == '0') {
			p = p->left;
		}
		else if (c == '1') {
			p = p->right;
		}

		if (p->left == nullptr&&p->right == nullptr) {
			std::cout << p->val << " ";
			p = root;
		}
	}
	std::cout << std::endl;
}

#endif // !BINARYNODE_H
