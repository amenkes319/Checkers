#include <iostream>
#include "Board.h"

int main() {
	Board b;
	Board c(b);
	b.Move({ 2, 1 }, { 3, 0 });

	b.PrintBoard();
	c.PrintBoard();

	return 0;
}