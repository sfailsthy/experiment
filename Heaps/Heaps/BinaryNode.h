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

template<typename Object>
struct BinaryNode
{
	Object val;
	BinaryNode *left;
	BinaryNode *right;
	int height;

	BinaryNode() :left{ nullptr }, right{ nullptr } {}
	BinaryNode(const Object &x, BinaryNode *l = nullptr, BinaryNode *r = nullptr, int h = 0) :
		val{ x }, left{ l }, right{ r }, height{ h } {}
	BinaryNode(Object &&x, BinaryNode *l = nullptr, BinaryNode *r = nullptr, int h = 0) :
		val{ std::move(x) }, left{ l }, right{ r }, height{ h } {}
};

template<typename Object>
void build(BinaryNode<Object>* &root, const std::vector<Object> &vec, int index) {
	if (index < vec.size()) {
		root = new BinaryNode<Object>{ vec[index] };
		build(root->left, vec, 2 * index + 1);
		build(root->right, vec, 2 * index + 2);
	}
}

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

#endif // !BINARYNODE_H
