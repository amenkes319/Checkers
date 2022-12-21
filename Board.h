#pragma once

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include "Piece.h"

#define BOARD_SIZE 8

#define IsValid(index) ( index >= 0 && index < BOARD_SIZE )
#define IsValidPos(row, col) ( IsValid(row) && IsValid(col) )
#define Abs(n) ( n < 0 ? -n : n )
#define Sign(n) ( (0 < n) - (n < 0) )
#define Avg(x, y) ( (x + y) / 2 )

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
	 * @param start Desired piece to move
	 * @param target Target to move the piece to
	 * @param moves Map of all possible moves from start
	 * @param isJump True if the move is a jump
	 */
	void Move(Piece &start, Piece &target, const std::unordered_map<Position, Position> moves, bool isJump = false);
	
	/**
	 * Reset positions of pieces.
	 */
	void ResetBoard();

	/**
	 * Finds all possible moves that can be made by the piece.
	 * @param piece Position of desired piece to move
	 * @return Hash map of all possible positions the piece can move to
	 */
	std::unordered_map<Position, Position> PossibleMoves(const Piece& start) const;

	/**
	 * Gets all possible jumps from the starting position.
	 * @param start Position of the piece
	 * @return Hash map of all positions the piece can jump to
	 */
	std::unordered_map<Position, Position> Jumps(const Piece& start) const;

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
	 * @return All single jumps from start position
	 */
	std::unordered_map<Position, Position> PossibleJumps(const Piece &start) const;

	/**
	 * Recursive helper function to go down the tree of possible multi-jumps.
	 * @param jumps Map of jumps discovered so far. Key = start position, value = end position
	 *				Note: start and end position are always a single jump apart
	 * @param start Starting piece that changes with each recursive call
	 * @param board The configuration of the board
	 */
	void Jumps(std::unordered_map<Position, Position> &jumps, const Piece &start) const;

	/**
	 * Replace the targeted piece with an empty piece.
	 * @param target Piece to remove
	 */
	void RemovePiece(Piece& target);

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
				j = b.At(i / BOARD_SIZE, i % BOARD_SIZE).GetType();
				h = h ^ table[i][j];
			}
			
			return h;
		}
	};
}

#endif