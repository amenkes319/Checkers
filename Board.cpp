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

void Board::Move(Piece &start, Piece &target, const std::unordered_map<Position, Position> moves, bool isJump) {
	target.SetType(start.GetType());
	start.SetType(EMPTY);

	if (isJump) {
		Piece newPos = target; // piece jumped to
		do {
			Piece prevPos = At(moves.at(newPos.GetPosition()));
			int jumpedRow = Avg(newPos.GetPosition().row, prevPos.GetPosition().row);
			int jumpedCol = Avg(newPos.GetPosition().col, prevPos.GetPosition().col);
			Piece jumped = At(jumpedRow, jumpedCol);
			RemovePiece(jumped);
			newPos = prevPos;
				
		} while (newPos != start);
	}

	// King'ing
	if (target.GetPosition().row == 0 && target.GetType() == RED_PAWN) {
		target.SetType(RED_KING);
	} else if (target.GetPosition().row == BOARD_SIZE - 1 && target.GetType() == BLACK_PAWN) {
		target.SetType(BLACK_KING);
	}
}

/* Reset board */
void Board::ResetBoard() {
	m_blackCounter = 0;
	m_redCounter = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (i < 3 && j % 2 != i % 2) {

				// initialize m_board[i][j]
				m_board[i][j] = Piece(BLACK_PAWN, i, j);
				m_blackCounter++;
			}
			else if (i >= 5 && j % 2 != i % 2) {
				m_board[i][j] = Piece(RED_PAWN, i, j);
				m_redCounter++;
			}
			else {
				m_board[i][j] = Piece(EMPTY, i, j);
			}
		}
	}
}

/*  */
std::unordered_map<Position, Position> Board::PossibleMoves(const Piece &start) const {
	std::unordered_map<Position, Position> moves;

	if (start.GetType() != EMPTY) {
		Position targetPos;
		for (int i = -1; i <= 1; i += 2) {
			for (int j = -1; j <= 1; j += 2) {
				targetPos = { start.GetPosition().row + i, start.GetPosition().col + j };
				if ((start.GetType() == i || // pawn only moves up or down
					Abs(start.GetType()) == KING) && // king can do both
					IsValid(start.GetPosition().row + i) &&
					IsValid(start.GetPosition().col + j) &&
					At(targetPos).GetType() == EMPTY)
				{
					moves.insert({ targetPos, start.GetPosition()});
				}
			}
		}
	}

	std::unordered_map<Position, Position> jumps = Jumps(start);
	moves.insert(jumps.begin(), jumps.end());

	return moves;
}

/* Possible Jumps */
std::unordered_map<Position, Position> Board::Jumps(const Piece &start) const {
	std::unordered_map<Position, Position> jumps;
	Jumps(jumps, start);
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
			switch (m_board[i][j].GetType()) {
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
void Board::Jumps(std::unordered_map<Position, Position>& jumps, const Piece &start) const {
	std::unordered_map<Position, Position> moves = PossibleJumps(start);
	if (!moves.empty()) {
		for (const auto& pos : moves) {
			if (jumps.find(pos.first) == jumps.end()) { // pos hasn't been visited
				Board newBoard(*this);
				Piece newStart = newBoard.At(start.GetPosition());
				Piece newTarget = newBoard.At(pos.first);
				newBoard.Move(newStart, newTarget, moves, true);
				jumps.insert(pos);
				Jumps(jumps, newTarget);
			}
		}
	}
}

/* Single jumps */
std::unordered_map<Position, Position> Board::PossibleJumps(const Piece& start) const {
	std::unordered_map<Position, Position> jumps;

	Piece target;
	for (int i = -2; i <= 2; i += 4) {
		for (int j = -2; j <= 2; j += 4) {
			target = At(start.GetPosition().row + i, start.GetPosition().col + j);
			if ((Sign(start.GetType()) == Sign(i) || Abs(start.GetType()) == KING) &&
				IsValidPos(target.GetPosition().row, target.GetPosition().col) &&
				target.GetType()  == EMPTY &&
				Sign(At(target.GetPosition().row / 2, target.GetPosition().col / 2).GetType()) == -Sign(start.GetType()))
			{
				jumps.insert({ target.GetPosition(), start.GetPosition() });
			}
		}
	}

	return jumps;
}

/*  */
void Board::RemovePiece(Piece& target) {
	if (Sign(target.GetType()) == BLACK) {
		m_blackCounter--;
	}
	else if (Sign(target.GetType()) == RED) {
		m_redCounter--;
	}
	target.SetType(EMPTY);
}