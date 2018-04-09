#include "BinaryHeap.h"
#include "BinaryNode.h"
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
	std::vector<int> vec{ 13,21,16,24,31,19,68,65,26,32 };
	BinaryHeap<int> heap(vec);
	heap.print();
	heap.insert(14);
	heap.print();
	system("pause");
}