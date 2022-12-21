#pragma once

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#define BOARD_SIZE 8
#define PAWN  1
#define KING  2
#define BLACK 1
#define RED  -1

#define IsValid(index) ( index >= 0 && index < BOARD_SIZE )
#define IsValidPos(row, col) ( IsValid(row) && IsValid(col) )
#define Abs(n) ( n < 0 ? -n : n )
#define Sign(n) ( (0 < n) - (n < 0) )
#define Avg(x, y) ( (x + y) / 2 )

/**
 * Enumeration to represent the possible states of a square of the board
 * Black pieces are positive, reds are negative
 */
enum Piece {
	EMPTY      =  0,
	BLACK_PAWN =  PAWN,
	BLACK_KING =  KING,
	RED_PAWN   = -PAWN,
	RED_KING   = -KING
};

/* Enumeration to represent the coordinates of each piece on the board. */
struct Position {
	int row;
	int col;
	
	bool operator==(const Position &p) const {
		return this->row == p.row && this->col == p.col;
	}

	bool operator!=(const Position& p) const {
		return !(*this == p);
	}

	friend std::ostream &operator<<(std::ostream& os, Position const& p) {
		return os << p.row << ", " << p.col;
	}
};

/* Represent a configuration of a checkers board */
class Board {
public:
	Board();
	Board(const Board &board);

	bool operator==(const Board& b) const {
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (m_board[i][j] != b.m_board[i][j]) return false;
			}
		}
		return true;
	}

	/**
	 * Move piece to target coordinates.
	 * @param start Position of desired piece to move
	 * @param target Position of the target to move the piece to
	 * @param moves Map of all possible moves from start
	 * @param isJump True if the move is a jump
	 */
	void Move(const Position &start, const Position &target, const std::unordered_map<Position, Position> moves, bool isJump = false);
	
	/**
	 * Reset positions of pieces.
	 */
	void ResetBoard();

	/**
	 * Finds all possible moves that can be made by the piece.
	 * @param piece Position of desired piece to move
	 * @return Hash map of all possible positions the piece can move to
	 */
	std::unordered_map<Position, Position> PossibleMoves(const Position& start) const;

	/**
	 * Gets all possible jumps from the starting position.
	 * @param start Position of the piece
	 * @return Hash map of all positions the piece can jump to
	 */
	std::unordered_map<Position, Position> Jumps(const Position& start);

	/**
	 * Gets the piece at the given position.
	 * @param pos Position of the wanted piece
	 * @return Piece enum of which piece is at the given position
	 */
	Piece At(Position pos) const;

	/**
	 * Gets the piece at the given coordinates.
	 * @param row Row of the wanted piece
	 * @param col Column of the wanted piece
	 * @return Piece enum of which piece is at the given coordinates
	 */
	Piece At(int row, int col) const;

	void PrintBoard();

	/**
	 * Gets the number of black pieces on the board.
	 * @return number of black pieces
	 */
	int GetBlackPieces() { return m_blackCounter; }

	/**
	 * Gets the number of red pieces on the board.
	 * @return number of red pieces
	 */
	int GetRedPieces() { return m_redCounter; }

private:
	/**
	 * Get all the single-jumps at the given position and board configuration.
	 * @param start Starting position of the piece to test
	 * @param board The configuration of the board
	 * @return 
	 */
	std::unordered_map<Position, Position> PossibleJumps(const Position &start, const Board &board);

	/**
	 * Recursive helper function to go down the tree of possible multi-jumps.
	 * @param jumps Map of jumps discovered so far. Key = start position, value = end position
	 *				Note: start and end position are always a single jump apart
	 * @param start Starting position that changes with each recursive call
	 * @param board The configuration of the board
	 */
	void Jumps(std::unordered_map<Position, Position> &jumps, const Position &start, const Board &board);

	/**
	 * Replace the targeted piece with an empty piece.
	 * @param target Position of piece to remove
	 */
	void RemovePiece(const Position& target);

	Piece m_board[BOARD_SIZE][BOARD_SIZE];
	int m_blackCounter;
	int m_redCounter;
};

namespace std {
	template<> struct hash<Position>
	{
		std::size_t operator()(const Position& p) const noexcept
		{
			return p.row * 10 + p.col;
		}
	};

	template<> struct hash<Board>
	{
		// Zobrist hashing
		std::size_t operator()(const Board& b) const noexcept
		{
			srand(time(NULL));
			constexpr int BOARD_POSITIONS = 64;
			constexpr int PIECE_TYPES = 5;
			//init zobrist
			int table[BOARD_POSITIONS][PIECE_TYPES];
			for (int i = 0; i < BOARD_POSITIONS; i++) {
				for (int j = 0; j < PIECE_TYPES; j++) {
					table[i][j] = rand();
				}
			}

			int h = 0;
			int j;
			for (int i = 0; i < BOARD_POSITIONS; i++) {
				j = b.At(i / BOARD_SIZE, i % BOARD_SIZE);
				h = h ^ table[i][j];
			}
			
			return h;
		}
	};
}

#endif