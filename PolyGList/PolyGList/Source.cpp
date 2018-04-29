#include <iostream>
#include "PolyGList.h"

int main() {
	PolyGList p;
	p.create();
	std::vector<int> vec = { -3,5,4 };
	std::cout << p.value(vec) << std::endl;
	p.print();
	PolyGList q;
	q.create();
	(p - q).print();
	p.print();
	system("pause");
}