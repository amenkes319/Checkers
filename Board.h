#pragma once

#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#define BOARD_SIZE 8

class Board
{
public:
	Board();

	/**
	 * Delete current memory allocated to piece pointers (if any) and create
	 * new ones at the original positions
	 */
	void ResetBoard();

	/**
	 * Deletes the memory allocated to the board pieces
	 */
	void DeleteBoard();
	~Board();

private:
	Piece* board[BOARD_SIZE][BOARD_SIZE / 2];
};

#endif