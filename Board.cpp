#include "Board.h"

Board::Board() {
	ResetBoard();
}

Board::Board(const Board& board) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			m_board[i][j] = board.m_board[i][j];
		}
	}
	m_blackCounter = board.m_blackCounter;
	m_redCounter = board.m_redCounter;
}

void Board::Move(const Position& start, const Position& target, const std::unordered_map<Position, Position> moves, bool isJump) {
	m_board[target.row][target.col] = m_board[start.row][start.col];
	m_board[start.row][start.col] = EMPTY;
		
	if (isJump) {
		Position newPos = target; // position jumped to
		Position prevPos; // position it jumped from
		do {
			prevPos = moves.at(newPos);
			int jumpedRow = Avg(newPos.row, prevPos.row);
			int jumpedCol = Avg(newPos.col, prevPos.col);
			Position jumped = { jumpedRow, jumpedCol };
			RemovePiece(jumped);
			newPos = prevPos;
				
		} while (newPos != start);
	}

	// King'ing
	if (target.row == 0 && m_board[target.row][target.col] == RED_PAWN) {
		m_board[target.row][target.col] = RED_KING;
	} else if (target.row == BOARD_SIZE - 1 && m_board[target.row][target.col] == BLACK_PAWN) {
		m_board[target.row][target.col] = BLACK_KING;
	}
}

/* Reset board */
void Board::ResetBoard() {
	m_blackCounter = 0;
	m_redCounter = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (i < 3 && j % 2 != i % 2) {
				m_board[i][j] = BLACK_PAWN;
				m_blackCounter++;
			}
			else if (i >= 5 && j % 2 != i % 2) {
				m_board[i][j] = RED_PAWN;
				m_redCounter++;
			}
			else {
				m_board[i][j] = EMPTY;
			}
		}
	}
}

/*  */
std::unordered_map<Position, Position> Board::PossibleMoves(const Position &start) {
	std::unordered_map<Position, Position> moves;
	Piece startPiece = At(start.row, start.col);

	if (startPiece != EMPTY) {
		Position target;
		for (int i = -1; i <= 1; i += 2) {
			for (int j = -1; j <= 1; j += 2) {
				target = { start.row + i, start.col + j };
				if ((startPiece == i || // pawn only moves up or down
					Abs(startPiece) == KING) && // king can do both
					IsValid(start.row + i) && IsValid(start.col + j) &&
					At(target) == EMPTY)
				{
					moves.insert({ target, start });
				}
			}
		}
	}

	std::unordered_map<Position, Position> jumps = Jumps(start);
	moves.insert(jumps.begin(), jumps.end());

	return moves;
}

/* Possible Jumps */
std::unordered_map<Position, Position> Board::Jumps(const Position& start) {
	std::unordered_map<Position, Position> jumps;
	Jumps(jumps, start, *this);
	return jumps;
}

/* Get the piece at the given position */
Piece Board::At(Position pos) const {
	return m_board[pos.row][pos.col];
}

/* Get the piece at the given coordinates */
Piece Board::At(int row, int col) const {
	return m_board[row][col];
}

void Board::PrintBoard() {
	char p;
	std::cout << "   0 1 2 3 4 5 6 7" << std::endl;
	std::cout << "  ________________" << std::endl;
	for (int i = 0; i < BOARD_SIZE; i++) {
		std::cout << i << " |";
		for (int j = 0; j < BOARD_SIZE; j++) {
			p = '-';
			switch (m_board[i][j]) {
			case 1:
				p = 'b';
				break;
			case 2:
				p = 'B';
				break;
			case -1:
				p = 'r';
				break;
			case -2:
				p = 'R';
				break;
			}

			std::cout << p << " ";
		}
		std::cout << std::endl;
	}
}

/* Private helper method for Jumps() */
void Board::Jumps(std::unordered_map<Position, Position>& jumps, const Position& start, const Board& board) {
	std::unordered_map<Position, Position> moves = PossibleJumps(start, board);
	if (!moves.empty()) {
		for (const auto& pos : moves) {
			if (jumps.find(pos.first) == jumps.end()) { // pos hasn't been visited
				Board newBoard(board);
				newBoard.Move(start, pos.first, moves, true);
				jumps.insert(pos);
				Jumps(jumps, pos.first, newBoard);
			}
		}
	}
}

/* Single jumps */
std::unordered_map<Position, Position> Board::PossibleJumps(const Position& start, const Board& board) {
	std::unordered_map<Position, Position> jumps;

	Position target;
	Piece piece;
	for (int i = -2; i <= 2; i += 4) {
		for (int j = -2; j <= 2; j += 4) {
			piece = board.At(start);
			target = { start.row + i, start.col + j };
			if ((Sign(piece) == Sign(i) || Abs(piece) == KING) &&
				IsValidPos(start.row + i, start.col + j) &&
				board.At(target) == EMPTY &&
				Sign(board.At(start.row + i / 2, start.col + j / 2)) == -Sign(piece))
			{
				jumps.insert({ target, start });
			}
		}
	}

	return jumps;
}

/*  */
void Board::RemovePiece(const Position& target) {
	if (Sign(At(target)) == BLACK) {
		m_blackCounter--;
	}
	else {
		m_redCounter--;
	}
	m_board[target.row][target.col] = EMPTY;
}