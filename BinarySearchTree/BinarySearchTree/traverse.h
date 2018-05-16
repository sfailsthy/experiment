#pragma once
#ifndef TRAVEL_H
#define TRAVEL_H

#include "BinaryNode.h"

template<typename Object>
void preorder(BinaryNode<Object> *root) {
	if (root != nullptr) {
		std::cout << root->val << " ";
		preorder(root->left);
		preorder(root->right);
	}
}

template<typename Object>
void inorder(BinaryNode<Object> *root) {
	if (root != nullptr) {
		inorder(root->left);
		std::cout << root->val << " ";
		inorder(root->right);
	}
}

template<typename Object>
void postorder(BinaryNode<Object> *root) {
	if (root != nullptr) {
		postorder(root->left);
		postorder(root->right);
		std::cout << root->val << " ";
	}
}

template<typename Object>
void bfs(BinaryNode<Object> *root) {
	if (root != nullptr) {
		std::queue<BinaryNode<Object>*> que;
		que.push(root);

		while (!que.empty()) {
			BinaryNode<Object> *p = que.front();
			std::cout << p->val << " ";
			que.pop();
			if (p->left != nullptr) {
				que.push(p->left);
			}
			if (p->right != nullptr) {
				que.push(p->right);
			}
		}
		std::cout << std::endl;
	}
}

#endif // !TRAVEL_H
