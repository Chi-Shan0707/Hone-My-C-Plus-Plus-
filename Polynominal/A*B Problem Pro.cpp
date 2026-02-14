#include <bits/stdc++.h>

using namespace std;

struct BigInt {
	static constexpr int BASE = 10000;
	static constexpr int BASE_DIGITS = 4;
	vector<int> digits;

	BigInt() = default;

	explicit BigInt(const string& s) {
		fromString(s);
	}

	void trim() {
		while (!digits.empty() && digits.back() == 0) {
			digits.pop_back();
		}
	}

	bool isZero() const {
		return digits.empty();
	}

	void fromString(string s) {
		digits.clear();

		size_t pos = 0;
		while (pos < s.size() && s[pos] == '0') {
			++pos;
		}
		if (pos == s.size()) {
			return;
		}
		s = s.substr(pos);

		for (int i = static_cast<int>(s.size()); i > 0; i -= BASE_DIGITS) {
			int start = max(0, i - BASE_DIGITS);
			int value = 0;
			for (int j = start; j < i; ++j) {
				value = value * 10 + (s[j] - '0');
			}
			digits.push_back(value);
		}
		trim();
	}

	string toString() const {
		if (isZero()) {
			return "0";
		}

		string result = to_string(digits.back());
		for (int i = static_cast<int>(digits.size()) - 2; i >= 0; --i) {
			string block = to_string(digits[i]);
			result += string(BASE_DIGITS - static_cast<int>(block.size()), '0') + block;
		}
		return result;
	}

	static vector<long long> karatsuba(const vector<long long>& a, const vector<long long>& b) {
		int n = static_cast<int>(a.size());
		vector<long long> result(2 * n, 0);

		if (n <= 32) {
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					result[i + j] += a[i] * b[j];
				}
			}
			return result;
		}

		int half = n / 2;

		vector<long long> a0(a.begin(), a.begin() + half);
		vector<long long> a1(a.begin() + half, a.end());
		vector<long long> b0(b.begin(), b.begin() + half);
		vector<long long> b1(b.begin() + half, b.end());

		// z0 = a0 * b0
		vector<long long> z0 = karatsuba(a0, b0);
		// z2 = a1 * b1
		vector<long long> z2 = karatsuba(a1, b1);

		// Compute (a0 + a1) and (b0 + b1), then recurse:
		// z1 = (a0 + a1) * (b0 + b1) - z0 - z2
		for (int i = 0; i < half; ++i) {
			a1[i] += a0[i];
			b1[i] += b0[i];
		}

		vector<long long> z1 = karatsuba(a1, b1);
		for (size_t i = 0; i < z0.size(); ++i) {
			z1[i] -= z0[i];
		}
		for (size_t i = 0; i < z2.size(); ++i) {
			z1[i] -= z2[i];
		}

		// Combine:
		// result = z0 + (z1 << half) + (z2 << (2 * half))
		for (size_t i = 0; i < z0.size(); ++i) {
			result[i] += z0[i];
		}
		for (size_t i = 0; i < z1.size(); ++i) {
			result[i + half] += z1[i];
		}
		for (size_t i = 0; i < z2.size(); ++i) {
			result[i + 2 * half] += z2[i];
		}

		return result;
	}

	static vector<int> multiplyDigits(const vector<int>& lhs, const vector<int>& rhs) {
		if (lhs.empty() || rhs.empty()) {
			return {};
		}

		vector<long long> a(lhs.begin(), lhs.end());
		vector<long long> b(rhs.begin(), rhs.end());

		int n = 1;
		while (n < static_cast<int>(max(a.size(), b.size()))) {
			n <<= 1;
		}

		a.resize(n, 0);
		b.resize(n, 0);

		vector<long long> product = karatsuba(a, b);

		vector<int> result(product.size(), 0);
		long long carry = 0;
		for (size_t i = 0; i < product.size(); ++i) {
			long long current = product[i] + carry;
			result[i] = static_cast<int>(current % BASE);
			carry = current / BASE;
		}

		while (carry > 0) {
			result.push_back(static_cast<int>(carry % BASE));
			carry /= BASE;
		}

		while (!result.empty() && result.back() == 0) {
			result.pop_back();
		}
		return result;
	}

	BigInt operator*(const BigInt& other) const {
		BigInt out;
		if (isZero() || other.isZero()) {
			return out;
		}
		out.digits = multiplyDigits(digits, other.digits);
		out.trim();
		return out;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string a, b;
	if (!(cin >> a >> b)) {
		return 0;
	}

	BigInt A(a), B(b);
	BigInt C = A * B;
	cout << C.toString() << '\n';
	return 0;
}
