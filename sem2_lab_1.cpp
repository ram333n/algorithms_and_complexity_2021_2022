#include "data_types.h"

#include <iostream>
#include <vector>

using namespace std;

namespace Constants { //константи дл€ демонстрац≥њ роботи
	const int first_prime = 2081;
	const int second_prime = 101;
	const int third_prime = 31;
	const int a = 3;
	const int b = 42;
	const vector<pair<int, int>> buckets_params = { {23, 88}, {10, 18}, {37, 87}, {5, 57} };
	const ComplexNumber default_value = { numeric_limits<int>::min(), numeric_limits<int>::min() };
	const vector<ComplexNumber> default_vector = {};
}


struct UniversalHash { //хешуванн€ дл€ комплексних чисел
	size_t operator()(ComplexNumber number, int a, int b, int first_prime, int second_prime) const {
		int k = number.GetRealPart() * first_prime + number.GetImaginaryPart();
		return (a * k + b) % second_prime;
	}
};


struct VectorHash { //хешуванн€ вектору
	size_t operator()(const vector<ComplexNumber>& v, int a, int b, int first_prime, int second_prime) const {
		size_t result = 0;
		int cur_power = 1;

		for (auto num : v) {
			result += number_hasher(num, a, b, first_prime, second_prime) * cur_power;
			cur_power *= Constants::third_prime;
		}
		return result;
	}

private:
	UniversalHash number_hasher;
};

void PrintVector(const vector<ComplexNumber>& values) { //¬ив≥д вектора в консоль
	cout << "{";
	for (const auto& v : values) {
		cout << v << " ";
	}
	cout << "}" << endl;
}

template <typename Hasher>
class HashSet { //хеш-таблиц€
public:
	explicit HashSet(const vector<vector<ComplexNumber>>& values) : buckets_count(values.size()), buckets_(buckets_count) {
		vector<int> buckets_size(buckets_count, 0);
		for (auto v : values) {
			++buckets_size[GetBucketIndex(v)];
		}

		for (size_t i = 0; i < buckets_count; ++i) {
			buckets_[i] = Bucket(buckets_size[i] * buckets_size[i], Constants::buckets_params[i % 4].first, Constants::buckets_params[i % 4].second);
		}

		for (auto v : values) {
			const int bucket_idx = GetBucketIndex(v);
			const int pos_in_bucket = buckets_[bucket_idx].GetPosition(v);
			buckets_[bucket_idx].data_[pos_in_bucket] = v;
		}
	}

	bool Has(const vector<ComplexNumber>& to_find) const { //пошук вектора в хеш-таблиц≥
		const int bucket_idx = GetBucketIndex(to_find);
		if (buckets_[bucket_idx].data_.empty()) {
			return false;
		}
		const int pos_in_bucket = buckets_[bucket_idx].GetPosition(to_find);
		return buckets_[bucket_idx].data_[pos_in_bucket] == to_find;
	}

	void Print() const { //друк таблиц≥
		for (const auto& bucket : buckets_) {
			for (const auto& v : bucket.data_) {
				PrintVector(v);
			}
			cout << "----------------------------------" << endl;
		}
	}

private:
	struct Bucket { //п≥дтаблиц€ з параметрами дл€ хешуванн€
		Bucket() = default;
		Bucket(size_t size, int new_a, int new_b) : data_(size, Constants::default_vector), a(new_a), b(new_b) {}

		size_t GetPosition(const vector<ComplexNumber>& values) const {
			return Hasher()(values, a, b, Constants::first_prime, Constants::second_prime) % data_.size();
		}

		vector<vector<ComplexNumber>> data_;
		int a = 0, b = 1;
	};

	size_t GetBucketIndex(const vector<ComplexNumber>& values) const {
		return Hasher()(values, Constants::a, Constants::b, Constants::first_prime, Constants::second_prime) % buckets_.size();
	}

	size_t buckets_count;
	vector<Bucket> buckets_;
};

void Assert(bool t) {
	cerr << "Test ";
	if (t) {
		cerr << "passed\n";
	}
	else {
		cerr << "failed\n";
	}
}

//int main() {
//	const vector<vector<ComplexNumber>> v {
//		{{3,2}, {5, -4}},
//		{{-2,-1}},
//		{{7,7},{-2,10},{1,3}}
//	};
//	HashSet<VectorHash> hash_set(v);
//	hash_set.Print();
//
//	Assert(!hash_set.Has({ { 5342, 6345 } }));
//	Assert(hash_set.Has({ {3,2}, {5, -4} }));
//	Assert(!hash_set.Has({ { 0, 1 } }));
//	Assert(hash_set.Has({ {-2,-1} }));
//	Assert(hash_set.Has({ {7,7},{-2,10},{1,3} }));
//}