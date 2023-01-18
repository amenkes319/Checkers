#pragma once

#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Piece.h"

constexpr auto PAWN_VALUE = 20;
constexpr auto KING_VALUE = 28;
constexpr auto KING_TRAPPED_VALUE = -3;
constexpr auto CLEAR_KING_PATH_VALUE = 3;
constexpr auto PIECE_MOBILITY_VALUE = 1;
constexpr auto PIECE_PROTECTION_VALUE = 0.5;

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