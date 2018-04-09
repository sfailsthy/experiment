#pragma once
#ifndef BINARYHEAP_H
#define BINARYHEAP_H

#include <vector>
#include <algorithm>
#include "BinaryNode.h"

template<typename Comparable>
class BinaryHeap
{
public:
	explicit BinaryHeap(int capacity = 100) :
		array(capacity + 1), currentSize{ 0 } {}
	explicit BinaryHeap(const std::vector<Comparable> &items) :
		array(items.size() + 10), currentSize{ static_cast<int>(items.size()) } {
		for (int i = 0; i < items.size(); ++i) {
			array[i + 1] = items[i];
		}
		buildHeap();
	}

	bool isEmpty() const { return currentSize == 0; }
	const Comparable& findMin() const { return array[1]; }

	void insert(const Comparable &x);
	void insert(Comparable &&x);

	void deleteMin() {
		array[1] = std::move(array[currentSize--]);
		precolateDown(1);
	}

	void deleteMin(Comparable &minItem) {
		minItem = std::move(array[1]);
		array[1] = std::move(array[currentSize--]);
		precolateDown(1);
	}

	void makeEmpty() { currentSize = 0; }
	void print() {
		BinaryNode<Comparable> *root = nullptr;
		std::vector<Comparable> vec(array.begin() + 1, array.begin() + currentSize + 1);
		build(root, vec, 0);
		printTree(root);
	}

private:
	int currentSize;
	std::vector<Comparable> array;

	void buildHeap();
	void precolateDown(int hole);
};

template<typename Comparable>
void BinaryHeap<Comparable>::insert(const Comparable &x) {
	if (currentSize == array.size() - 1) {
		array.resize(array.size() * 2);
	}

	int hole = ++currentSize;
	Comparable copy = x;

	array[0] = std::move(copy);
	for (; x < array[hole / 2]; hole /= 2) {
		array[hole] = std::move(array[hole / 2]);
	}
	array[hole] = std::move(array[0]);
}

template<typename Comparable>
void BinaryHeap<Comparable>::insert(Comparable &&x) {
	if (currentSize == array.size() - 1) {
		array.resize(array.size() * 2);
	}

	int hole = ++currentSize;
	for (; hole > 1 && x < array[hole / 2]; hole /= 2) {
		array[hole] = std::move(array[hole / 2]);
	}
	array[hole] = std::move(x);
}

template<typename Comparable>
void BinaryHeap<Comparable>::precolateDown(int hole) {
	int child;
	Comparable tmp = std::move(array[hole]);

	for (; 2 * hole <= currentSize; hole = child) {
		child = 2 * hole;
		if (child != currentSize && array[child + 1] < array[child]) {
			++child;
		}
		if (array[child] < tmp) {
			array[hole] = std::move(array[child]);
		}
		else {
			break;
		}
	}
	array[hole] = std::move(tmp);
}

template<typename Comparable>
void BinaryHeap<Comparable>::buildHeap() {
	for (int i = currentSize / 2; i > 0; --i) {
		precolateDown(i);
	}
}

#endif // !BINARYHEAP_H
