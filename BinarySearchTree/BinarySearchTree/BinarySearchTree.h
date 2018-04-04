#pragma once
#include "BinaryNode.h"
#include <iostream>
#include <algorithm>

template<typename Comparable>
class BinarySearchTree
{
public:
	BinarySearchTree() :root{ nullptr } {}
	BinarySearchTree(const BinarySearchTree &rhs) :root{ nullptr } {
		root = clone(rhs.root);
	}

	BinarySearchTree(BinarySearchTree &&rhs) :root{ rhs.root } {
		rhs.root = nullptr;
	}
	~BinarySearchTree() { makeEmpty(); }

	BinaryNode<Comparable>* getRoot() const {
		return root;
	}

	const Comparable& findMin() const {
		return findMin(root)->val;
	}

	const Comparable& findMax() const {
		return findMax(root)->val;
	}

	bool contains(const Comparable &x) const {
		return contains(x, root);
	}

	bool isEmpty() const {
		return root == nullptr;
	}

	void printTree(std::ostream &out = std::cout) const {
		if (isEmpty()) {
			out << "Empty tree" << std::endl;
		}
		else {
			printTree(root, out);
		}
	}

	void makeEmpty() { makeEmpty(root); }
	void insert(const Comparable &x) { insert(x, root); }
	void insert(Comparable &&x) { insert(std::move(x), root); }
	void remove(const Comparable &x) { remove(x, root); }

	BinarySearchTree& operator=(const BinarySearchTree &rhs) {
		BinarySearchTree copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	BinarySearchTree& operator=(BinarySearchTree &&rhs) {
		std::swap(root, rhs.root);
		return *this;
	}

private:
	BinaryNode<Comparable> * root;
	int height(BinaryNode<Comparable> *t) const {
		return t == nullptr ? -1 : t->height;
	}
	static const int ALLOWED_IMBALANCE = 1;

	void insert(const Comparable &x, BinaryNode<Comparable>* &t) {
		if (t == nullptr) {
			t = new BinaryNode<Comparable>{ x,nullptr,nullptr };
		}
		else if (x < t->val) {
			insert(x, t->left);
		}
		else if (x > t->val) {
			insert(x, t->right);
		}

		balance(t);
	}

	void insert(Comparable &&x, BinaryNode<Comparable>* &t) {
		if (t == nullptr) {
			t = new BinaryNode<Comparable>{ std::move(x),nullptr,nullptr };
		}
		else if (x < t->val) {
			insert(std::move(x), t->left);
		}
		else if (x > t->val) {
			insert(std::move(x), t->right);
		}

		balance(t);
	}

	void remove(const Comparable &x, BinaryNode<Comparable>* &t) {
		if (t == nullptr) {
			return;
		}
		else if (x < t->val) {
			remove(x, t->left);
		}
		else if (x > t->val) {
			remove(x, t->right);
		}
		else if (t->left != nullptr&&t->right != nullptr) {
			//two children
			t->val = findMin(t->right)->val;
			remove(t->val, t->right);
		}
		else {
			BinaryNode<Comparable> *oldNode = t;
			t = (t->left != nullptr) ? t->left : t->right;
			delete oldNode;
		}
		balance(t);
	}

	BinaryNode<Comparable>* findMin(BinaryNode<Comparable> *t) const {
		if (t == nullptr) {
			return nullptr;
		}
		else if (t->left == nullptr) {
			return t;
		}
		else {
			return findMin(t->left);
		}
	}

	BinaryNode<Comparable>* findMax(BinaryNode<Comparable> *t) const {
		if (t != nullptr) {
			while (t->right != nullptr) {
				t = t->right;
			}
		}
		return t;
	}

	bool contains(const Comparable &x, BinaryNode<Comparable> *t) const {
		if (t == nullptr) {
			return false;
		}
		else if (x < t->val) {
			return contains(x, t->left);
		}
		else if (x > t->val) {
			return contains(x, t->right);
		}
		else {
			return true;
		}
	}

	void makeEmpty(BinaryNode<Comparable>* &t) const {
		if (t != nullptr) {
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = nullptr;
	}

	void printTree(BinaryNode<Comparable> *t, std::ostream &out) const {
		if (t != nullptr)
		{
			printTree(t->left, out);
			out << t->val << std::endl;
			printTree(t->right, out);
		}
	}

	BinaryNode<Comparable>* clone(BinaryNode<Comparable> *t) const {
		if (t == nullptr) {
			return nullptr;
		}
		else {
			return new BinaryNode<Comparable>{ t->val,clone(t->left),clone(t->right) };
		}
	}

	void rotateWithLeftChild(BinaryNode<Comparable>* &k2) {
		BinaryNode<Comparable> *k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;
		k2->height = std::max(height(k2->left), height(k2->right)) + 1;
		k1->height = std::max(height(k1->left), height(k1->right)) + 1;
		k2 = k1;
	}

	void rotateWithRightChild(BinaryNode<Comparable>* &k1) {
		BinaryNode<Comparable> *k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;
		k2->height = std::max(height(k2->left), height(k2->right)) + 1;
		k1->height = std::max(height(k1->left), height(k1->right)) + 1;
		k1 = k2;
	}

	void doubleWithLeftChild(BinaryNode<Comparable>* &k3) {
		rotateWithRightChild(k3->left);
		rotateWithLeftChild(k3);
	}

	void doubleWithRightChild(BinaryNode<Comparable>* &k1) {
		rotateWithLeftChild(k1->right);
		rotateWithRightChild(k1);
	}

	void balance(BinaryNode<Comparable>* &t) {
		if (t == nullptr) {
			return;
		}

		if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) {
			if (height(t->left->left) >= height(t->left->right)) {
				rotateWithLeftChild(t);
			}
			else {
				doubleWithLeftChild(t);
			}
		}
		else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) {
			if (height(t->right->right) >= height(t->right->left)) {
				rotateWithRightChild(t);
			}
			else {
				doubleWithRightChild(t);
			}
		}
		t->height = std::max(height(t->left), height(t->right)) + 1;
	}
};