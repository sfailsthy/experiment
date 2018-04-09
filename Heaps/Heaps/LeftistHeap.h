#pragma once
#ifndef LEFTISTHEAP_H
#define LEFTISTHEAP_H

#include <vector>
#include <algorithm>
#include <iostream>

template<typename Comparable>
class LeftistHeap
{
public:
	LeftistHeap() :root{ nullptr } {}
	LeftistHeap(const LeftistHeap &rhs) :root{ nullptr }
	{
		root = clone(rhs.root);
	}

	LeftistHeap(LeftistHeap &&rhs) :root{ rhs.root }
	{
		rhs.root = nullptr;
	}

	~LeftistHeap() { makeEmpty(); }

	LeftistHeap& operator=(const LeftistHeap &rhs) {
		LeftistHeap copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	LeftistHeap& operator=(LeftistHeap &&rhs) {
		std::swap(root, rhs.root);
		return *this;
	}


	bool isEmpty() const { return root == nullptr; }
	const Comparable& findMin() const { return root->element; }

	void insert(const Comparable &x) {
		root = merge(new LeftistNode{ x }, root); 
	}

	void insert(Comparable &&x) {
		root = merge(new LeftistNode{ std::move(x) }, root);
	}

	void deleteMin() {
		LeftistNode *oldRoot = root;
		root = merge(root->left, root->right);
		delete oldRoot;
	}

	void deleteMin(Comparable &minItem) {
		minItem = findMin();
		deleteMin();
	}

	void makeEmpty() {
		reclaimMemory(root);
		root = nullptr;
	}

	void merge(LeftistHeap &rhs) {
		if (this == &rhs) {
			return;
		}
		root = merge(root, rhs.root);
		rhs.root = nullptr;
	}

	void print() { printTree(root); }

private:
	///////LeftistNode
	struct LeftistNode
	{
		Comparable element;
		LeftistNode *left;
		LeftistNode *right;
		int npl;

		LeftistNode(const Comparable &e, LeftistNode *lt = nullptr,
			LeftistNode *rt = nullptr, int np = 0)
			:element{ e }, left{ lt }, right{ lt }, npl{ np } {}
		LeftistNode(Comparable &&e, LeftistNode *lt = nullptr,
			LeftistNode *rt = nullptr, int np = 0)
			:element{ e }, left{ lt }, right{ lt }, npl{ np } {}
	};
	
	int maxLevel(LeftistNode* node) {
		if (node == nullptr) {
			return 0;
		}
		return std::max(maxLevel(node->left), maxLevel(node->right)) + 1;
	}

	bool all_is_null(std::vector<LeftistNode*> nodes) {
		for (LeftistNode* node : nodes) {
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

	void printNodes(std::vector<LeftistNode*> nodes, int level, int max_level) {
		int floor = max_level - level;
		int endgeLines = static_cast<int>(pow(2, std::max(floor - 1, 0)));
		int firstSpaces = static_cast<int>(pow(2, floor) - 1);
		int betweenSpaces = static_cast<int>(pow(2, floor + 1) - 1);
		print_white_spaces(firstSpaces);

		std::vector<LeftistNode*> newNodes;
		for (LeftistNode* node : nodes) {
			if (node != nullptr) {
				std::cout << node->element;
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

	void printTree(LeftistNode *root) {
		int max_level = maxLevel(root);
		printNodes(std::vector<LeftistNode*>{root}, 1, max_level);
	}
	///////LeftistNode

	LeftistNode *root;
	LeftistNode* merge(LeftistNode *h1, LeftistNode *h2) {
		if (h1 == nullptr) {
			return h2;
		}
		if (h2 == nullptr) {
			return h1;
		}
		if (h1->element < h2->element) {
			return merge1(h1, h2);
		}
		else {
			return merge1(h2, h1);
		}
	}
	LeftistNode* merge1(LeftistNode *h1, LeftistNode *h2) {
		if (h1->left == nullptr) {
			h1->left = h2;
		}
		else {
			h1->right = merge(h1->right, h2);
			if (h1->left->npl < h1->right->npl) {
				swapChildren(h1);
			}
			h1->npl = h1->right->npl + 1;
		}
		return h1;
	}

	void swapChildren(LeftistNode *t) {
		LeftistNode *tmp = t->left;
		t->left = t->right;
		t->right = tmp;
	}

	void reclaimMemory(LeftistNode *t) {
		if (t != nullptr) {
			reclaimMemory(t->left);
			reclaimMemory(t->right);
			delete t;
		}
	}

	LeftistNode* clone(LeftistNode *t) const {
		if (t == nullptr) {
			return nullptr;
		}
		else {
			return new LeftistNode{ t->element,clone(t->left),clone(t->right),t->npl };
		}
	}
};
#endif // !LEFTISTHEAP_H
