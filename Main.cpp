#include <iostream>
#include <string>
#include <regex>
#include "Board.h"
#include "AI.h"

#define DIFFICULTY 3

int main() {
	Board board;
	std::regex match("([0-7](,|\\s)[0-7])");
	std::string input;
	bool blackTurn = true;
	bool comp = true;
	int row, col, tRow, tCol;
	Position startPos, targetPos;
	std::unordered_map<Position, Position> moves;
	AI ai;
	
	board.PrintBoard();
	while (1) {
		if (board.GetBlackPieces() == 0) {
			std::cout << "Red wins!" << std::endl;
			break;
		}
		else if (board.GetRedPieces() == 0) {
			std::cout << "Black wins!" << std::endl;
			break;
		}

		std::cout << "Black: " << board.GetBlackPieces() << std::endl;
		std::cout << "Red: " << board.GetRedPieces() << std::endl;

		if (comp && !blackTurn) {
			auto aiMove = ai.LookAhead(board ,DIFFICULTY * 2, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), blackTurn);
			startPos = aiMove.first.second;
			targetPos = aiMove.first.first;
			moves = board.PossibleMoves(startPos);

			std::cout << "******************" << std::endl;
			std::cout << "AI move: " << startPos << " --> " << targetPos << ", score: " << aiMove.second << std::endl;
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
			if ((blackTurn ? 1 : -1) * board.At(row, col) > 0) {
				moves = board.PossibleMoves(startPos);
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
		board.Move(startPos, targetPos, moves, abs(targetPos.row - startPos.row) > 1 || abs(targetPos.col - startPos.col) > 1);
		board.PrintBoard();
		blackTurn = !blackTurn;
		
	}

	return 0;
}