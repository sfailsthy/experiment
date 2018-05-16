#pragma once
#ifndef PRINTTREE_H
#define PRINTTREE_H

#include "BinaryNode.h"

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

#endif // !PRINTTREE_H
