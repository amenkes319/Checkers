#include "AI.h"

AI::AI() {
}

std::pair<std::pair<Position, Position>, int> AI::LookAhead(const Board &board, int depth, int alpha, int beta, bool maximize) {
	// If the search has reached the maximum depth or the board is in a quiescent state,
	// return the heuristic value of the current board position
	if (depth == 0 || IsQuiescent(board)) {
		return {
			{
				{ -1, -1 }, // pos1
				{ -1, -1 }  // pos2
			},
			Heuristic(board)
		};
	}

	std::pair<Position, Position> bestMove;
	int bestScore;

	// Initialize the best score based on whether we are maximizing or minimizing
	if (maximize) {
		bestScore = std::numeric_limits<int>::min(); // black
	}
	else {
		bestScore = std::numeric_limits<int>::max(); // red
	}

	// Create a transposition table to store the scores of previously evaluated board positions
	std::unordered_map<Board, int> transpositionTable;

	// Iterate through all possible moves
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			Piece piece = board.At(i, j);
			if (maximize) {
				if (Sign(piece.GetType()) != Sign(BLACK_PAWN)) {
					continue;
				}
			}
			else {
				if (Sign(piece.GetType()) != Sign(RED_PAWN)) {
					continue;
				}
			}

			//std::cout << "Depth: " << depth << std::endl;

			// Find all possible moves for the current pos
			auto moves = board.PossibleMoves(piece);

			// Iterate through all possible moves
			for (auto& move : moves) {
				// Make the move on a copy of the current board
				Board copy(board);
				Piece start = copy.At(move.first);
				Piece target = copy.At(move.second);
				copy.Move(start, target, moves);

				int score;
				// Check if the board position has been evaluated before
				if (transpositionTable.count(copy) > 0) {
					// If the board position has been evaluated before, use the previously stored score
					// instead of searching through the subtree again
					score = transpositionTable[copy];
				}
				else {
					// Otherwise, search through the subtree to find the score
					score = LookAhead(copy, depth - 1, alpha, beta, !maximize).second;

					// Store the score in the transposition table for future use
					transpositionTable[copy] = score;
				}

				// make sure move.second is true start in case of multi jump
				while (moves.find(move.second) != moves.end()) {
					move.second = moves[move.second];
				}

				// Update the best move and score based on the current score
				if (maximize && score > bestScore) {
					bestMove = { move.first, move.second };
					bestScore = score;

					// Update alpha for alpha-beta pruning
					alpha = std::max(alpha, bestScore);
					if (beta <= alpha) break; // Beta cut-off
				}
				else if (!maximize && score < bestScore) {
					bestMove = { move.first, move.second };
					bestScore = score;

					// Update beta for alpha-beta pruning
					beta = std::min(beta, bestScore);
					if (beta <= alpha) break; // Alpha cut-off
				}
			}
		}
	}
	// Return the best move and score
	return { bestMove, bestScore };
}

int AI::Heuristic(const Board &board) {
	int score = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			Piece piece = board.At(i, j);
			switch (piece.GetType()) {
			case BLACK_PAWN:
				score += 5;
				break;

			case BLACK_KING:
				score += 10;
				break;

			case RED_PAWN:
				score -= 5;
				break;

			case RED_KING:
				score -= 10;
				break;
			}
		}
	}
	return score;
}

bool AI::IsQuiescent(const Board& board) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			Piece piece = board.At(i, j);
			if (piece.GetType() != EMPTY && !board.PossibleMoves(piece).empty()) {
				return false;
			}
		}
	}
	return true;
}

bool AI::IsKingTrapped(const Board& board, Position pos) {
	return IsKingTrapped(board, pos.row, pos.col);
}

bool AI::IsKingTrapped(const Board& board, int r, int c) {
	// get possible moves for piece
	Piece piece = board.At(r, c);
	auto moves = board.PossibleMoves(piece);
	if (!moves.empty()) {
		return false;
	}

	// If the king has no legal moves, check if any friendly pieces can
    // capture the enemy pieces surrounding the king.
	Piece enemy = -1 * board.At(r, c);
		
}


// TODO: Combine piece and position