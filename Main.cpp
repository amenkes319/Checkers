#include <iostream>
#include <string>
#include <regex>
#include "Board.h"

#define DIFFICULTY 3

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
	bool comp = true;
	int row, col, tRow, tCol;
	Position startPos, targetPos;
	std::unordered_map<Position, Position> moves;
	b.PrintBoard();
	while (1) {
		if (b.GetBlackPieces() == 0) {
			std::cout << "Red wins!" << std::endl;
			break;
		}
		else if (b.GetRedPieces() == 0) {
			std::cout << "Black wins!" << std::endl;
			break;
		}

		std::cout << "Black: " << b.GetBlackPieces() << std::endl;
		std::cout << "Red: " << b.GetRedPieces() << std::endl;

		if (comp && !blackTurn) {
			auto ai = b.LookAhead(DIFFICULTY * 2, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), blackTurn);
			startPos = ai.first.second;
			targetPos = ai.first.first;
			moves = b.PossibleMoves(startPos);

			std::cout << "******************" << std::endl;
			std::cout << "AI move: " << startPos << " --> " << targetPos << ", score: " << ai.second << std::endl;
			std::cout << "******************" << std::endl;
		}
		else {
			std::cout << "Enter " << (blackTurn ? "black" : "red") << " piece to move: ";
			getline(std::cin, input);
			if (!regex_match(input, match)) {
				std::cout << "Invalid input" << std::endl;
				continue;
			}
			// char to int
			row = input[0] - '0';
			col = input[2] - '0';
			startPos = { row, col };

			// Positive or negative depending on if black or red turn
			if ((blackTurn ? 1 : -1) * b.At(row, col) > 0) {
				moves = b.PossibleMoves(startPos);
				std::cout << "Valid piece" << std::endl;
				do {
					std::cout << "Enter target: ";
					getline(std::cin, input);

					if (!regex_match(input, match)) {
						// see if input is "back"
						if (input == "back") {
							break;
						}

						continue;
					}

					tRow = input[0] - '0';
					tCol = input[2] - '0';
					targetPos = { tRow, tCol };

					if (!moves.count(targetPos)) {
						std::cout << "Invalid target" << std::endl;
					}/* else {
						b.Move(startPos, targetPos, moves, abs(targetPos.row - startPos.row) > 1 || abs(targetPos.col - startPos.col) > 1);
						b.PrintBoard();
					}*/
				} while (!moves.count(targetPos));
			}
			else {
				std::cout << "Invalid piece" << std::endl;
			}
		}
		b.Move(startPos, targetPos, moves, abs(targetPos.row - startPos.row) > 1 || abs(targetPos.col - startPos.col) > 1);
		b.PrintBoard();
		blackTurn = !blackTurn;
		
	}

	return 0;
}