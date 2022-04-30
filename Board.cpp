#include "Board.h"

#define IsValid(index) ( index >= 0 && index < BOARD_SIZE )
#define IsValidPos(row, col) ( IsValid(row) && IsValid(col) )
#define Abs(n) ( n < 0 ? -n : n )
#define Sign(n) ( (0 < n) - (n < 0) )
#define Avg(x, y) ( (x + y) / 2 )

Board::Board() {
	ResetBoard();
}

Board::Board(Board& board) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			m_board[i][j] = board.m_board[i][j];
		}
	}
}

void Board::ResetBoard() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (i < 3 && j % 2 != i % 2) {
				m_board[i][j] = BLACK_KING;
			}
			else if (i >= 5 && j % 2 != i % 2) {
				m_board[i][j] = RED_KING;
			}
			else {
				m_board[i][j] = EMPTY;
			}
		}
	}

	RemovePiece({ 7, 2 });
}

// TODO: Add king'ing
void Board::Move(const Position &piece, const Position &target, bool isJump) {
	m_board[target.row][target.col] = m_board[piece.row][piece.col];
	m_board[piece.row][piece.col] = EMPTY;

	if (isJump) {
		Position jumped = { Avg(piece.row, target.row), Avg(piece.col, target.col) };
		RemovePiece(jumped);
	}
}

void Board::RemovePiece(const Position& target) {
	m_board[target.row][target.col] = EMPTY;
}

bool Board::IsValidMove(const Position &piece, const Position &target) {
	std::unordered_set<Position> moves = PossibleMoves(piece);
	return moves.find(target) != moves.end();
}

/* Single jumps */
std::unordered_set<Position> Board::PossibleJumps(const Position &start, Board &board) {
	std::unordered_set<Position> jumps;

	Position target;
	Piece piece;
	for (int i = -2; i <= 2; i += 4) {
		for (int j = -2; j <= 2; j += 4) {
			piece = board.At(start);
			target = { start.row + i, start.col + j };
			if ((Sign(piece) == i || Abs(piece) == KING) &&
				IsValidPos(start.row + i, start.col + j) &&
				board.At(target) == EMPTY &&
				Sign(board.At(start.row + i/2, start.col + j/2)) == -Sign(piece))
			{
				jumps.insert(target);
			}
		}
	}

	return jumps;
}

void Board::Jumps(std::unordered_set<Position>& jumps, const Position &start, Board &board) {
	std::unordered_set<Position> moves = PossibleJumps(start, board);
	if (!moves.empty()) {
		for (const auto& pos : moves) {
			if (jumps.find(pos) == jumps.end()) { // pos hasn't been visited
				Board newBoard(board);
				newBoard.Move(start, pos, true);
				newBoard.PrintBoard();
				jumps.insert(pos);
				Jumps(jumps, pos, newBoard);
			}
		}
	}
}

std::unordered_set<Position> Board::Jumps(const Position& start) {
	std::unordered_set<Position> jumps;
	Jumps(jumps, start, *this);
	return jumps;
}

// TODO: Find possible jumps
std::unordered_set<Position> Board::PossibleMoves(Position start) {
	std::unordered_set<Position> moves;

	if (m_board[start.row][start.col] != EMPTY) {
		Position target;
		for (int i = -1; i <= 1; i += 2) {
			for (int j = -1; j <= 1; j += 2) {
				target = { start.row + i, start.col + j };
				if ((m_board[start.row][start.col] == i || // pawn only moves up or down
					Abs(m_board[start.row][start.col]) == KING) && // king can do both
					IsValid(start.row + i) && IsValid(start.col + j) &&
					At(target) == EMPTY)
				{
					moves.insert(target);
				}
			}
		}
	}

	return moves;
}

// TODO: Implement backtracking to simulate possible game moves and look ahead
std::unordered_set<Board> Board::LookAhead() {
	std::unordered_set<Board> boards;

	return boards;
}

Piece Board::At(Position pos) {
	return m_board[pos.row][pos.col];
}

Piece Board::At(int row, int col) {
	return m_board[row][col];
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