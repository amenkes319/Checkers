#pragma once

#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Piece.h"

class AI {
public:
	AI();

	/**
	 * Look Ahead to determine the best possible move
	 * @return pair of positions where the first position is the start and the second is the target
	 */
	std::pair<std::pair<Position, Position>, int> LookAhead(const Board& board, int depth, int alpha, int beta, bool maximize);

	
	
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
	bool IsKingTrapped(const Board& board, int r, int c);
	bool IsKingTrapped(const Board& board, Position pos);

	bool ClearPathToKing(const Board& board, int r, int c);
	bool ClearPathToKing(const Board& board, Position pos);

	int GetPieceProtection(const Board& board, int r, int c);
	int GetPieceProtection(const Board& board, Position pos);
};

#endif