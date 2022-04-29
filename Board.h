#pragma once

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <algorithm>
#define BOARD_SIZE 8
#define PAWN 1
#define KING 2

/**
 * Enumeration to represent the possible states of a square of the board
 * Black pieces are positive, reds are negative
 */
enum Piece {
	EMPTY       = 0,
	BLACK_PIECE = PAWN,
	BLACK_KING  = KING,
	RED_PIECE   = -PAWN,
	RED_KING    = -KING
};

struct Position {
	int row;
	int col;
};

/* Represent a configuration of a checkers board */
class Board
{
public:
	Board();

	/**
	 * Move piece to target coordinates
	 * @param piece Position of desired piece to move
	 * @param target Position of the target to move the piece to
	 * @return true if the move is valid
	 */
	bool Move(Position piece, Position target);

	/**
	 * Reset positions of pieces
	 */
	void ResetBoard();

	/**
	 * Finds all possible moves that can be made by the piece
	 * @param piece Position of desired piece to move
	 * @return vector of all possible positions the piece can move to
	 */
	std::vector<Position> PossibleMoves(Position piece);

	std::vector<Board> LookAhead();

	void PrintBoard();

private:
	std::vector<Position> Jumps(std::vector<Position> &so_far, Position start);

	/**
	 * Returns if the coords of the piece can move to the target
	 * @param piece Position of desired piece to move
	 * @param target Position of the target to move the piece to
	 * @return true if the piece can move to the target
	 */
	bool IsValidMove(Position &piece, Position &target);

	Piece m_board[BOARD_SIZE][BOARD_SIZE];
};

#endif