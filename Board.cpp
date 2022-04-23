#include "Board.h"

Board::Board() {
	ResetBoard();
}

bool Board::Move(Coords piece, Coords target) {
	if (IsValidMove(piece, target)) {
		m_board[target.row][target.col] = m_board[piece.row][piece.col];
		m_board[piece.row][piece.col] = EMPTY;
		return true;
	}
	return false;
}

void Board::ResetBoard() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (i < 3 && j % 2 != i % 2) {
				m_board[i][j] = BLACK_PIECE;
			} else if (i >= 5 && j % 2 != i % 2) {
				m_board[i][j] = RED_PIECE;
			} else {
				m_board[i][j] = EMPTY;
			}
		}
	}
}

bool Board::IsValidMove(Coords &piece, Coords &target) {
	std::vector<Coords> moves = PossibleMoves(piece);
	return true;
	//return std::find(moves.begin(), moves.end(), target) != moves.end();
}

/**
 * Helper function to determine if an index is within the bounds of the board
 * @param index Index to test
 * @return true if index is between [0, BOARD_SIZE)
 */
static bool IsValid(int index) {
	return index >= 0 && index < BOARD_SIZE;
}

// TODO: Find possible jumps
std::vector<Coords> Board::PossibleMoves(Coords piece) {
	std::vector<Coords> moves;
	Coords c;
	for (int i = -1; i <= 1; i += 2) {
		for (int j = -1; j <= 1; j += 2) {
			if (IsValid(piece.row + i) && IsValid(piece.col + j)) {
				c = { piece.row + i, piece.col + j };
				moves.push_back(c);
			}
		}
	}

	return moves;
}

// TODO: Implement backtracking to simulate possible game moves and look ahead
std::vector<Board> Board::LookAhead() {
	std::vector<Board> boards;

	return boards;
}

void Board::PrintBoard() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			char p = '-';
			switch (m_board[i][j]) {
			case 1:
			case 2:
				p = 'X';
				break;
			case -1:
			case -2:
				p = 'O';
				break;
			}

			std::cout << p;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}