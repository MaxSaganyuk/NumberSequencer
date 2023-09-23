#include "../../NumberSequencer.h"
#include <iostream>

int main() {
	std::vector<llint> a = "2 5 10 17 ... (10)"_nseq;
	for (auto i : a) {
		std::cout << i << ' ';
	}
}