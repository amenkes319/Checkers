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

// TODO: Add proper jump
void Board::Move(const Position& piece, const Position& target, bool isJump) {
	if (IsValidMove(piece, target)) {
		m_board[target.row][target.col] = m_board[piece.row][piece.col];
		m_board[piece.row][piece.col] = EMPTY;

		if (isJump) {
			// 3,2   7,2
			// the row and column difference is the number of pieces that were jumped
			// the row and column sign is the direction of the jump
			// the loop iterates through each piece that was jumped and removes 
			int rowDiff = Abs(piece.row - target.row);
			int colDiff = Abs(piece.col - target.col);
			int rowSign = Sign(piece.row - target.row);
			int colSign = Sign(piece.col - target.col);
			for (int i = 1; i < rowDiff; i++) {
				m_board[piece.row + (i * rowSign)][piece.col + (i * colSign)] = EMPTY;
			}
		}

		// King'ing
		if (target.row == 0 && m_board[target.row][target.col] == RED_PAWN) {
			m_board[target.row][target.col] = RED_KING;
		} else if (target.row == BOARD_SIZE - 1 && m_board[target.row][target.col] == BLACK_PAWN) {
			m_board[target.row][target.col] = BLACK_KING;
		}
	}
}

/*  */
void Board::ResetBoard() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (i < 3 && j % 2 != i % 2) {
				m_board[i][j] = BLACK_PAWN;
			}
			else if (i >= 5 && j % 2 != i % 2) {
				m_board[i][j] = RED_PAWN;
			}
			else {
				m_board[i][j] = EMPTY;
			}
		}
	}
	//m_board[2][1] = BLACK_KING;
	RemovePiece({ 7, 2 });
}

/*  */
std::unordered_set<Position> Board::PossibleMoves(const Position &start) {
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

	std::unordered_set<Position> jumps = Jumps(start);
	moves.insert(jumps.begin(), jumps.end());

	return moves;
}

/* Possible Jumps */
std::unordered_set<Position> Board::Jumps(const Position& start) {
	std::unordered_set<Position> jumps;
	Jumps(jumps, start, *this);
	return jumps;
}

// TODO: Implement backtracking to simulate possible game moves and look ahead
std::unordered_set<Board> Board::LookAhead() {
	std::unordered_set<Board> boards;

	return boards;
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
	for (int i = 0; i < BOARD_SIZE; i++) {
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

			std::cout << p;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

/* Private helper method for Jumps() */
void Board::Jumps(std::unordered_set<Position>& jumps, const Position& start, Board& board) {
	std::unordered_set<Position> moves = PossibleJumps(start, board);
	if (!moves.empty()) {
		for (const auto& pos : moves) {
			if (jumps.find(pos) == jumps.end()) { // pos hasn't been visited
				Board newBoard(board);
				newBoard.Move(start, pos, true);
				jumps.insert(pos);
				Jumps(jumps, pos, newBoard);
			}
		}
	}
}

/* Single jumps */
std::unordered_set<Position> Board::PossibleJumps(const Position& start, Board& board) {
	std::unordered_set<Position> jumps;

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
				jumps.insert(target);
			}
		}
	}

	return jumps;
}

/*  */
void Board::RemovePiece(const Position& target) {
	m_board[target.row][target.col] = EMPTY;
}

/*  */
bool Board::IsValidMove(const Position& piece, const Position& target) {
	return true;
	std::unordered_set<Position> moves = PossibleMoves(piece);
	return moves.find(target) != moves.end();
}
