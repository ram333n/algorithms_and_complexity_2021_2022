#include "data_types.h"

#include <iostream>
#include <vector>

using namespace std;

namespace Constants {
	const int first_prime = 2081;
	const int second_prime = 101;
	const int a = 3;
	const int b = 42;
	const vector<pair<int, int>> buckets_params = { {23, 88}, {10, 18}, {37, 87}, {5, 57} };
	const ComplexNumber default_value = { numeric_limits<int>::min(), numeric_limits<int>::min() };
}


struct UniversalHash {
	size_t operator()(ComplexNumber number, int a, int b, int first_prime, int second_prime) const {
		int k = abs(number.GetRealPart() * first_prime + number.GetImaginaryPart());
		return (a * k + b) % second_prime;
	}
};


class HashSet {
public:
	explicit HashSet(const vector<ComplexNumber>& values) : buckets_count(values.size()), buckets_(buckets_count) {
		vector<int> buckets_size(buckets_count, 0);
		for (auto number : values) {
			++buckets_size[GetBucketIndex(number)];
		}

		for (size_t i = 0; i < buckets_count; ++i) {
			buckets_[i] = Bucket(buckets_size[i] * buckets_size[i], Constants::buckets_params[i % 4].first, Constants::buckets_params[i % 4].second);
		}

		for (auto number : values) {
			const int bucket_idx = GetBucketIndex(number);
			const int pos_in_bucket = buckets_[bucket_idx].GetPosition(number);
			buckets_[bucket_idx].data_[pos_in_bucket] = number;
		}
	}

	bool Has(ComplexNumber to_find) const {
		const int bucket_idx = GetBucketIndex(to_find);
		if (buckets_[bucket_idx].data_.empty()) {
			return false;
		}
		const int pos_in_bucket = buckets_[bucket_idx].GetPosition(to_find);
		return buckets_[bucket_idx].data_[pos_in_bucket] == to_find;
	}

	void Print() const {
		for (const auto& bucket : buckets_) {
			bool is_empty = true;
			for (auto elem : bucket.data_) {
				if (elem == Constants::default_value) {
					continue;
				}
				cout << elem << ", ";
				is_empty = false;
			}

			if (!is_empty) {
				cout << endl;
			}
		}
	}

private:
	struct Bucket {
		Bucket() = default;
		Bucket(size_t size, int new_a, int new_b) : data_(size, Constants::default_value), a(new_a), b(new_b) {}

		size_t GetPosition(ComplexNumber number) const {
			return UniversalHash()(number, a, b, Constants::first_prime, Constants::second_prime) % data_.size();
		}

		vector<ComplexNumber> data_;
		int a, b;
	};

	size_t GetBucketIndex(ComplexNumber number) const {
		return UniversalHash()(number, Constants::a, Constants::b, Constants::first_prime, Constants::second_prime) % buckets_.size();
	}

	size_t buckets_count;
	vector<Bucket> buckets_;
};

template<typename T>
void Assert(T t) {
	cerr << "Test ";
	if (t) {
		cerr << "passed\n";
	}
	else {
		cerr << "failed\n";
	}
}

int main() {
	const vector<ComplexNumber> v{ {1, -1}, { 27,32 }, { -10,-5 }, { 4,37 }, {0,1}, {101,43}, {-54,23}, {3,4}, {-85,2}, {43,76}, {-12,3}, {76,-544}};
	HashSet hash_set(v);
	hash_set.Print();

	Assert(!hash_set.Has({ 5342, 6345 }));
	Assert(hash_set.Has({ 1, -1 }));
	Assert(hash_set.Has({ 3, 4 }));
	Assert(!hash_set.Has({ 0, 0 }));
	Assert(hash_set.Has({ 76, -544 }));
	Assert(hash_set.Has({ 101, 43 }));
	Assert(!hash_set.Has({ 43, 43 }));
	Assert(!hash_set.Has({ 976897, 32434 }));
	Assert(!hash_set.Has({ 1, 1 }));
}