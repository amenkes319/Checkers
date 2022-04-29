#include "Board.h"

#define IsValid(index) ( index >= 0 && index < BOARD_SIZE )
#define Abs(n) ( n < 0 ? -n : n )

Board::Board() {
	ResetBoard();
}

// TODO: Add king'ing
bool Board::Move(Position piece, Position target) {
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
				m_board[i][j] = BLACK_KING;
			} else if (i >= 5 && j % 2 != i % 2) {
				m_board[i][j] = RED_KING;
			} else {
				m_board[i][j] = EMPTY;
			}
		}
	}
}

bool Board::IsValidMove(Position &piece, Position &target) {
	std::vector<Position> moves = PossibleMoves(piece);
	return true;
	// std::find(moves.begin(), moves.end(), target) != moves.end();
}

std::vector<Position> Board::Jumps(std::vector<Position> &so_far, Position start) {
	std::vector<Position> jumps;
	return jumps;
}

// TODO: Find possible jumps
std::vector<Position> Board::PossibleMoves(Position piece) {
	std::vector<Position> moves;
	Position p;
	for (int i = -1; i <= 1; i += 2) {
		for (int j = -1; j <= 1; j += 2) {
			if ((m_board[piece.row][piece.col] == i || // pawn only moves up or down
				Abs(m_board[piece.row][piece.col]) == KING) && // king can go both
				IsValid(piece.row + i) && IsValid(piece.col + j) &&
				m_board[piece.row + i][piece.col + j] == EMPTY)
			{
				p = { piece.row + i, piece.col + j };
				moves.push_back(p);
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
				p = 'x';
				break;
			case 2:
				p = 'X';
				break;
			case -1:
				p = 'o';
				break;
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