#include <iostream>
#include <string>
#include <regex>
#include "Board.h"

int main() {
	Board b;
//	b.Move({ 2, 1 }, { 3, 2 });
//	b.Move({ 5, 4 }, { 4, 3 });
//	b.PrintBoard();
//	std::unordered_set<Position> jumps = b.PossibleMoves({ 3, 2 });
//
//	for (auto jump : jumps) {
//		std::cout << jump << std::endl;
//	}

	std::regex match("([0-7](,|\\s)[0-7])");
	std::string input;
	bool blackTurn = true;
	int row, col, tRow, tCol;
	Position piecePos, targetPos;
	std::unordered_set<Position> jumps;
	b.PrintBoard();
	while (1) {
		std::cout << "Enter " << (blackTurn ? "black" : "red") << " piece to move: ";
		getline(std::cin, input);
		if (!regex_match(input, match)) {
			std::cout << "Invalid input" << std::endl;
			continue;
		}
		// char to int
		row = input[0] - '0';
		col = input[2] - '0';
		piecePos = { row, col };
		jumps = b.PossibleMoves(piecePos);
		
		for (auto jump : jumps) {
			std::cout << jump << "Hash: " << std::hash<Position>()(jump) << std::endl;
		}
		
		// Positive or negative depending on if black or red turn
		if ((blackTurn ? 1 : -1) * b.At(row, col) > 0) {
			std::cout << "Valid piece" << std::endl;
			blackTurn = !blackTurn;
			do {
				std::cout << "Enter target: ";
				getline(std::cin, input);

				if (!regex_match(input, match)) {
					continue;
				}

				tRow = input[0] - '0';
				tCol = input[2] - '0';
				targetPos = { tRow, tCol };
				
				std::cout << jumps.count(targetPos) << std::endl;
				if (!jumps.count(targetPos)) {
					std::cout << "Invalid target" << std::endl;
				} else {
					b.Move(piecePos, targetPos);
					b.PrintBoard();
				}
			} while (!jumps.count(targetPos));
		} else {
			std::cout << "Invalid piece" << std::endl;
		}
	}

	return 0;
}