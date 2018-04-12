#pragma once
#ifndef SORT_H
#define SORT_H

#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

template<typename Comparable>
void insertionSort(std::vector<Comparable> &a) {
	for (int p = 1; p != a.size(); ++p) {
		Comparable tmp = std::move(a[p]);
		int j;
		for (j = p; j > 0 && tmp < a[j - 1]; --j) {
			a[j] = std::move(a[j - 1]);
		}
		a[j] = std::move(tmp);
		print(a);
	}
}

template<typename Iterator,typename Comparator>
void insertionSort(const Iterator &begin, const Iterator &end, Comparator lessThan) {
	if (begin == end) {
		return;
	}

	Iterator j;
	for (Iterator p = begin + 1; p != end; ++p) {
		auto tmp = std::move(*p);
		for (j = p; j != begin && lessThan(tmp, *(j - 1)); --j) {
			*j = std::move(*(j - 1));
		}
		*j = std::move(tmp);
	}
}

template<typename Iterator>
void insertionSort(const Iterator &begin, const Iterator &end) {
	insertionSort(begin, end, std::less<decltype(*begin)>{});
}

template<typename Comparable>
void shellSort(std::vector<Comparable> &a) {
	for (int gap = a.size() / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < a.size(); ++i) {
			Comparable tmp = std::move(a[i]);
			int j = i;
			for (; j >= gap && tmp < a[j - gap]; j -= gap) {
				a[j] = std::move(a[j - gap]);
			}
			a[j] = std::move(tmp);
		}
		print(a);
	}
}

template<typename Comparable>
void heapSort(std::vector<Comparable> &a) {
	for (int i = a.size() / 2 - 1; i >= 0; --i) {
		percDown(a, i, a.size());
	}
	print(a);
	for (int j = a.size() - 1; j > 0; --j) {
		std::swap(a[0], a[j]);
		percDown(a, 0, j);
		print(a);
	}
}

template<typename Comparable>
void percDown(std::vector<Comparable> &a, int i, int n) {
	int child;
	Comparable tmp;

	for (tmp = std::move(a[i]); 2 * i + 1 < n; i = child) {
		child = 2 * i + 1;
		if (child != n - 1 && a[child] < a[child + 1]) {
			++child;
		}
		if (tmp < a[child]) {
			a[i] = std::move(a[child]);
		}
		else {
			break;
		}
	}
	a[i] = std::move(tmp);
}

template<typename Comparable>
void merge(std::vector<Comparable> &a, std::vector<Comparable> &tmpArray, int leftPos, int rightPos, int rightEnd) {
	int leftEnd = rightPos - 1;
	int tmpPos = leftPos;
	int numElements = rightEnd - leftPos + 1;

	while (leftPos <= leftEnd && rightPos <= rightEnd) {
		if (a[leftPos] <= a[rightPos]) {
			tmpArray[tmpPos++] = std::move(a[leftPos++]);
		}
		else {
			tmpArray[tmpPos++] = std::move(a[rightPos++]);
		}
	}

	while (leftPos <= leftEnd) {
		tmpArray[tmpPos++] = std::move(a[leftPos++]);
	}

	while (rightPos <= rightEnd) {
		tmpArray[tmpPos++] = std::move(a[rightPos++]);
	}

	for (int i = 0; i < numElements; ++i, --rightEnd) {
		a[rightEnd] = std::move(tmpArray[rightEnd]);
	}
}

template<typename Comparable>
void mergeSort(std::vector<Comparable> &a, std::vector<Comparable> &tmpArray, int left, int right) {
	if (left < right) {
		int center = (left + right) / 2;
		mergeSort(a, tmpArray, left, center);
		mergeSort(a, tmpArray, center + 1, right);
		merge(a, tmpArray, left, center + 1, right);
	}
}

template<typename Comparable>
void mergeSort(std::vector<Comparable> &a) {
	std::vector<Comparable> tmpArray(a.size());
	mergeSort(a, tmpArray, 0, a.size() - 1);
}

template<typename Comparable>
void SORT(std::vector<Comparable> &items) {
	if (items.size() > 1) {
		std::vector<Comparable> smaller;
		std::vector<Comparable> same;
		std::vector<Comparable> larger;

		auto chosenItem = items[items.size() / 2];

		for (auto &i : items) {
			if (i < chosenItem) {
				smaller.push_back(std::move(i));
			}
			else if (chosenItem < i) {
				larger.push_back(std::move(i));
			}
			else {
				same.push_back(std::move(i));
			}
		}

		SORT(smaller);
		SORT(larger);

		std::move(std::begin(smaller), std::end(smaller), std::begin(items));
		std::move(std::begin(same), std::end(same), std::begin(items) + smaller.size());
		std::move(std::begin(larger), std::end(larger), end(items) - larger.size());
	}
}

template<typename Comparable>
const Comparable& median3(std::vector<Comparable> &a, int left, int right) {
	int center = (left + right) / 2;
	if (a[center] < a[left]) {
		std::swap(a[left], a[center]);
	}
	if (a[right] < a[left]) {
		std::swap(a[left], a[right]);
	}
	if (a[right] < a[center]) {
		std::swap(a[right], a[center]);
	}

	std::swap(a[center], a[right - 1]);
	return a[right - 1];
}

template<typename Comparable>
void quicksort(std::vector<Comparable> &a, int left, int right) {
	if (left + 10 <= right) {
		const Comparable &pivot = median3(a, left, right);

		int i = left, j = right - 1;
		for (;;) {
			while (a[++i] < pivot) {}
			while (pivot < a[--j]) {}
			if (i < j) {
				std::swap(a[i], a[j]);
			}
			else {
				break;
			}
		}

		std::swap(a[i], a[right - 1]);
		quicksort(a, left, i - 1);
		quicksort(a, i + 1, right);
	}
	else {
		insertionSort(a);
	}
}

template<typename Comparable>
void quicksort(std::vector<Comparable> &a) {
	quicksort(a, 0, a.size() - 1);
}

template<typename Comparable>
void quickSelect(std::vector<Comparable> &a, int left, int right, int k) {
	if (left + 10 <= right) {
		const Comparable &pivot = median3(a, left, right);

		int i = left, j = right - 1;
		for (;;) {
			while (a[++i] < pivot) {}
			while (pivot < a[--j]) {}
			if (i < j) {
				std::swap(a[i], a[j]);
			}
			else {
				break;
			}
		}

		std::swap(a[i], a[right - 1]);

		if (k <= i) {
			quickSelect(a, left, i - 1, k);
		}
		else if (k > i + 1) {
			quickSelect(a, i + 1, right, k);
		}
	}
	else {
		insertionSort(a);
	}
}

template<typename Container>
void print(Container &c) {
	for (auto &item : c) {
		std::cout << item << " ";
	}
	std::cout << std::endl;
}

#endif // !SORT_H
