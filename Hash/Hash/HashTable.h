#pragma once
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <iostream>

template<typename HashedObj>
class HashTable {
public:
	explicit HashTable(int size = 101) :array(size) {
		makeEmpty();
	}

	bool contains(const HashedObj &x) const {
		return isActive(findPos(x));
	}

	void makeEmpty() {
		currentSize = 0;
		for (auto &entry : array) {
			entry.info = EMPTY;
		}
	}

	bool insert(const HashedObj &x) {
		int currentPos = findPos(x);
		if (isActive(currentPos)) {
			return false;
		}

		array[currentPos].element = x;
		array[currentPos].info = ACTIVE;

		if (++currentSize > array.size() / 2) {
			rehash();
		}
		return true;
	}

	bool insert(HashedObj &&x) {
		int currentPos = findPos(x);
		if (isActive(currentPos)) {
			return false;
		}

		array[currentPos].element = std::move(x);
		array[currentPos].info = ACTIVE;

		if (++currentSize > array.size() / 2) {
			rehash();
		}
		return true;
	}

	bool remove(const HashedObj &x) {
		int currentPos = findPos(x);
		if (!isActive(currentPos)) {
			return false;
		}

		array[currentPos].info = DELETED;
		return true;
	}

	void print() {
		for (int i = 0; i != array.size(); ++i) {
			if (array[i].info == ACTIVE) {
				std::cout << i << " : " << array[i].element << std::endl;
			}
		}
	}
	enum EntryType { ACTIVE, EMPTY, DELETED };

private:
	struct HashEntry
	{
		HashedObj element;
		EntryType info;

		HashEntry(const HashedObj &e = HashedObj{}, EntryType i = EMPTY) :
			element{ e }, info{ i } {}
		HashEntry(HashedObj &&e, EntryType i = EMPTY) :
			element{ std::move(e) }, info{ i } {}
	};

	std::vector<HashEntry> array;
	int currentSize;

	bool isActive(int currentPos) const {
		return array[currentPos].info == ACTIVE;
	}

	int findPos(const HashedObj &x) const {
		int offset = 1;
		int currentPos = myhash(x);

		while (array[currentPos].info != EMPTY &&
			array[currentPos].element != x) {
			currentPos += offset;
			offset += 2;
			if (currentPos >= array.size()) {
				currentPos -= array.size();
			}
		}
		return currentPos;
	}

	void rehash() {
		std::vector<HashEntry> old = array;
		array.resize(2 * old.size());
		for (auto &entry : array) {
			entry.info = EMPTY;
		}

		currentSize = 0;
		for (auto &entry : old) {
			if (entry.info == ACTIVE) {
				insert(std::move(entry.element));
			}
		}
	}

	size_t myhash(const HashedObj &x) const {
		/*static std::hash<HashedObj> hf;
		return hf(x) % theLists.size();*/
		return x % array.size();
	}
};
#endif // !HASHTABLE_H
