#pragma once
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "BinaryNode.h"

template<typename Object>
std::pair<BinaryNode<Object>*, std::vector<std::pair<Object, std::string>>>
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


#endif // !HUFFMAN_H
