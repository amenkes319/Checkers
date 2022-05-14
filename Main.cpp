#include <iostream>
#include "Board.h"

int main() {
	Board b;
	b.Move({ 2, 1 }, { 3, 2 });
	b.Move({ 5, 4 }, { 4, 3 });
	b.PrintBoard();
	std::unordered_set<Position> jumps = b.Jumps({ 3, 2 });

	for (auto jump : jumps) {
		std::cout << jump << std::endl;
	}

	return 0;
}