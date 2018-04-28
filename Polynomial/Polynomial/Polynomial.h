#pragma once
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>

class Polynomial
{
	friend Polynomial operator+(const Polynomial&, const Polynomial&);
	friend Polynomial operator-(const Polynomial&, const Polynomial&);
	friend Polynomial operator*(const Polynomial&, const Polynomial&);
	friend std::pair<Polynomial,Polynomial> operator/(const Polynomial&, const Polynomial&);
	friend Polynomial mul_term_by_all_terms(std::pair<int, int> t, Polynomial P);

public:
	Polynomial() = default;
	Polynomial(int coeff, int exp) :poly(1) {
		poly[0] = std::make_pair(coeff, exp);
	}
	Polynomial(std::vector<std::pair<int, int>> vec) :poly(vec.size()) {
		for (int i = 0; i != vec.size(); ++i) {
			poly[i] = std::move(vec[i]);
		}
	}
	Polynomial(const Polynomial &rhs) :
		poly(rhs.poly.size()) {
		for (int i = 0; i != rhs.poly.size(); ++i) {
			poly[i] = rhs.poly[i];
		}
	}

	Polynomial(Polynomial &&rhs) :
		poly(std::move(rhs.poly)) {}

	Polynomial& operator=(const Polynomial &rhs) {
		Polynomial copy = rhs;
		std::swap(*this, copy);
		return *this;
	}
	Polynomial& operator=(Polynomial &&rhs) {
		std::swap(poly, rhs.poly);
		return *this;
	}
	~Polynomial() {
		poly.clear();
	}

	void push_back(int coeff, int exp) {
		poly.push_back({ coeff,exp });
	}

	void print() {
		for (auto &item : poly) {
			std::cout << "(" << item.first << " , " << item.second << ") ";
		}
		std::cout << std::endl;
	}

	bool empty() const { return poly.empty(); }

private:
	//(coeff,exp)
	std::vector<std::pair<int, int>> poly;

	std::pair<int,int> first_term() const {
		return poly[0];
	}
	Polynomial rest_terms() const {
		std::vector<std::pair<int, int>> tmp(poly.begin() + 1, poly.end());
		return Polynomial(tmp);
	}

};

Polynomial operator+(const Polynomial&, const Polynomial&);
Polynomial operator-(const Polynomial&, const Polynomial&);
Polynomial operator*(const Polynomial&, const Polynomial&);
std::pair<Polynomial, Polynomial> operator/(const Polynomial&, const Polynomial&);
Polynomial mul_term_by_all_terms(std::pair<int, int> t, Polynomial P);
#endif // !POLYNOMIAL_H
