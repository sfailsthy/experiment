#include "Polynomial.h"

Polynomial operator+(const Polynomial &lhs, const Polynomial &rhs) {
	if (lhs.empty()) {
		return rhs;
	}
	else if (rhs.empty()) {
		return lhs;
	}
	else {
		Polynomial res;
		auto p = lhs.first_term(), q = rhs.first_term();
		if (p.second > q.second) {
			res = lhs + rhs.rest_terms();
			res.push_back(q.first, q.second);
		}
		else if (p.second < q.second) {
			res = rhs + lhs.rest_terms();
			res.push_back(p.first, p.second);
		}
		else {
			res = lhs.rest_terms() + rhs.rest_terms();
			if (p.first + q.first) {
				res.push_back(p.first + q.first, p.second);
			}
		}
		
		using P = std::pair<int, int>;
		std::sort(res.poly.begin(),res.poly.end(),
			[](P a, P b) {return a.second < b.second; });
		return res;
	}
}

Polynomial operator-(const Polynomial &lhs, const Polynomial &rhs) {
	Polynomial tmp;
	for (auto &item : rhs.poly) {
		tmp.push_back(-item.first, item.second);
	}
	return lhs + tmp;
}

Polynomial mul_term_by_all_terms(std::pair<int, int> t1, Polynomial p) {
	if (p.empty()) {
		return p;
	}
	else {
		auto t2 = p.first_term();
		Polynomial res = mul_term_by_all_terms(t1, p.rest_terms());
		res.push_back(t1.first*t2.first, t1.second + t2.second);
		using P = std::pair<int, int>; 
		std::sort(res.poly.begin(), res.poly.end(),
			[](P a, P b) {return a.second < b.second; });
		return res;
	}
}

Polynomial operator*(const Polynomial &lhs, const Polynomial &rhs) {
	if (lhs.empty()) {
		return lhs;
	}
	else {
		Polynomial res = mul_term_by_all_terms(lhs.first_term(), rhs);
		res = res + lhs.rest_terms()*rhs;
		return res;
	}
}
