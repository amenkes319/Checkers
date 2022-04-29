#include <iostream>
#include "Board.h"

int main() {
	Board b;
	b.PrintBoard();
	Position p = { 5, 2 };
	Position p1 = { 4, 1 };
	b.Move(p, p1);
	b.PrintBoard();

	for (auto p : b.PossibleMoves(p1)) {
		std::cout << p.row << ", " << p.col << std::endl;
	}

	return 0;
}