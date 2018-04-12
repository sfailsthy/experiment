#include "BinaryHeap.h"
#include "BinaryNode.h"
#include "LeftistHeap.h"
#include "BinomialQueue.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

int main(int argc, char *argv[]) {
	/*std::vector<int> vec{ 13,21,16,24,31,19,68,65,26,32 };
	BinaryHeap<int> heap(vec);
	heap.print();
	heap.insert(14);
	heap.print();*/

	/*LeftistHeap<int> heap1,heap2;
	std::vector<int> vec1{ 3,10,8,21,14,17,23,26 };
	for (const auto &i : vec1) {
		heap1.insert(i);
	}
	heap1.print();

	for (const auto &i : { 6,7,12,18,24,37,18,33 }) {
		heap2.insert(i);
	}
	heap2.print();
	heap1.merge(heap2);
	heap1.print();*/
	BinomialQueue<int> que;
	std::vector<int> vec{ 1,2,3,4,5,6,7 };
	que.insert(1);
	que.insert(2);
	que.print();
	system("pause");
}