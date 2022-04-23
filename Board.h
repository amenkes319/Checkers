#pragma once

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <algorithm>
#define BOARD_SIZE 8

/**
 * Enumeration to represent the possible states of a square of the board
 * Black pieces are positive, reds are negative
 */
enum Piece {
	EMPTY       = 0,
	BLACK_PIECE = 1,
	BLACK_KING  = 2,
	RED_PIECE   = -1,
	RED_KING    = -2,
};

struct Coords {
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
	 * @param piece Coordinates of desired piece to move
	 * @param target Coordinates of the target to move the piece to
	 * @return true if the move is valid
	 */
	bool Move(Coords piece, Coords target);

	/**
	 * Reset positions of pieces
	 */
	void ResetBoard();

	/**
	 * Finds all possible moves that can be made by the piece
	 * @param piece Coordinates of desired piece to move
	 * @return vector of all possible coordinates the piece can move to
	 */
	std::vector<Coords> PossibleMoves(Coords piece);

	std::vector<Board> LookAhead();

	void PrintBoard();

private:
	/**
	 * Returns if the coords of the piece can move to the target
	 * @param piece Coordinates of desired piece to move
	 * @param target Coordinates of the target to move the piece to
	 * @return true if the piece can move to the target
	 */
	bool IsValidMove(Coords &piece, Coords &target);

	Piece m_board[BOARD_SIZE][BOARD_SIZE];
};

#endif