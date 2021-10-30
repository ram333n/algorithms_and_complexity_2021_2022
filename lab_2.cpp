#include <iostream>
#include <string>
#include <vector>

bool AreCyclicShift(const std::string& source, const std::string& shifted) {
	size_t source_size =  source.size();
	size_t shifted_size = shifted.size();

	if (source_size != shifted_size) {
		return false;
	}

	std::vector<size_t> longest_prefix_suffix_val(source_size);

	size_t len = 0;
	size_t i = 1;

	longest_prefix_suffix_val[0] = 0;

	while (i < source_size) {
		if (source[i] == shifted[len]) {
			longest_prefix_suffix_val[i] = ++len;
			++i;
		}
		else {
			if (len == 0) {
				longest_prefix_suffix_val[i] = 0;
				++i;
			}
			else {
				len = longest_prefix_suffix_val[len - 1];
			}
		}
	}

	i = 0;

	for (size_t k = longest_prefix_suffix_val[source_size - 1]; k < shifted_size; ++k) {
		if (shifted[k] != source[i++]) {
			return false;
		}
	}

	return true;
}

int main() {
	std::string first, second;
	std::cout << "Enter strings\n";
	while (true) {
		std::cin >> first >> second;

		if (AreCyclicShift(first, second)) {
			std::cout << "Yes\n";
		}
		else {
			std::cout << "No\n";
		}
	}
}