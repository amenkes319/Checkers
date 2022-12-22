#pragma once

#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Piece.h"
#include <omp.h>

#define PAWN_VALUE 10
#define KING_VALUE 20
#define KING_TRAPPED_VALUE -3
#define CLEAR_KING_PATH_VALUE 5
#define PIECE_MOBILITY_VALUE 1
#define PIECE_PROTECTION_VALUE 0.5

class AI {
public:
	AI();

	/**
	 * Look Ahead to determine the best possible move
	 * @return pair of positions where the first position is the start and the second is the target
	 */
	std::pair<std::pair<Position, Position>, int> BestMove(const Board& board, int depth, int alpha, int beta, bool maximize);
	
	void P();

private:
	int Heuristic(const Board& board);

	/**
	 * Determine if the board is in a quiescent state
	 * @return true if the board is in a quiescent state, false otherwise
	 */
	bool IsQuiescent(const Board& board);

	/**
	 *
	 * @return true if the king at the given position is trapped by enemy pieces
	 */
	bool IsKingTrapped(const Board& board, const Piece piece);
	
	bool ClearPathToKing(const Board& board, const Piece piece);
	
	int GetPieceProtection(const Board& board, const Piece piece);

	int time[7];
};

#endif