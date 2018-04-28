#include <utility>
#include <algorithm>
#include <iostream>
#include <vector>
#include "Polynomial.h"

int main()
{
	Polynomial p1;
	p1.push_back(-1, 0);
	p1.push_back(1, 5);
	p1.print();
	Polynomial p2;
	p2.push_back(-1, 0);
	p2.push_back(1, 2);
	p2.print();
	(p1 + p2).print();
	(p1 - p2).print();
	(p1 * p2).print();
	system("pause");
}