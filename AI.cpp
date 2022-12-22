#include "AI.h"

AI::AI() {
	for (int i = 0; i < 7; i++) {
		time[i] = 0;
	}
}

void AI::P() {
	for (int i = 0; i < 7; i++) {
		std::cout << i << ": " << time[i] << std::endl;
	}
}

std::pair<std::pair<Position, Position>, int> AI::BestMove(const Board &board, int depth, int alpha, int beta, bool maximize) {
	
	time[depth]++;
	
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
//#pragma omp parallel for
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			Piece piece = board.At(i, j);
			if (maximize) {
				if (piece.GetColor() != BLACK) {
					continue;
				}
			}
			else {
				if (piece.GetColor() != RED) {
					continue;
				}
			}

			// std::cout << "Depth: " << depth << std::endl;

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
					score = BestMove(copy, depth - 1, alpha, beta, !maximize).second;

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
			if (piece.GetType() == EMPTY) continue;

			// Calculate the number of pawns/kings
			switch (piece.GetType()) {
			case BLACK_PAWN:
				score += PAWN_VALUE;
				break;

			case BLACK_KING:
				score += KING_VALUE;
				break;

			case RED_PAWN:
				score -= PAWN_VALUE;
				break;

			case RED_KING:
				score -= KING_VALUE;
				break;
			}

			// Calculate the number of trapped kings for each player
			if (piece.GetType() == BLACK_KING) {
				if (IsKingTrapped(board, piece)) {
					score += KING_TRAPPED_VALUE;
				}
			}
			else if (piece.GetType() == RED_KING) {
				if (IsKingTrapped(board, piece)) {
					score -= KING_TRAPPED_VALUE;
				}
			}

			// Calculate the number of pieces with clear paths to become a king
			if (ClearPathToKing(board, piece)) {
				score += (piece.GetColor() == BLACK ? 1 : -1) * CLEAR_KING_PATH_VALUE;
			}

			// Calculate the piece mobility (number of moves) for each player
			auto moves = board.PossibleMoves(piece);
			score += (piece.GetColor() == BLACK ? 1 : -1) * moves.size() * PIECE_MOBILITY_VALUE;
			
			// Calculate the piece protection for each player
			score += (piece.GetColor() == BLACK ? 1 : -1) * GetPieceProtection(board, piece) * PIECE_PROTECTION_VALUE;
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

bool AI::IsKingTrapped(const Board& board, const Piece piece) {
	// get possible moves for piece
	auto moves = board.PossibleMoves(piece);
	if (!moves.empty()) {
		return false;
	}

	// If the king has no legal moves, check if any friendly pieces can
	// capture the enemy pieces surrounding the king.
	int enemyType = (piece.GetColor() == BLACK) ? RED : BLACK;
	for (int i = -1; i <= 1; i += 2) {
		for (int j = -1; j <= 1; j += 2) {
			if (!IsValidPos(piece.GetRow() + i, piece.GetCol() + j)) continue;
			Piece enemyPiece = board.At(piece.GetRow() + i, piece.GetCol() + j);
			if (enemyPiece.GetType() == enemyType) {
				// Check if any friendly pieces can capture the enemy piece
				for (int k = -1; k <= 1; k += 2) {
					for (int l = -1; l <= 1; l += 2) {
						if (!IsValidPos(enemyPiece.GetRow() + k, enemyPiece.GetCol() + l)) continue;
						Piece friendlyPiece = board.At(enemyPiece.GetRow() + k, enemyPiece.GetCol() + l);
						if (friendlyPiece.GetColor() == piece.GetColor()) {
							// Check if the friendly piece can capture the enemy piece
							auto friendlyMoves = board.PossibleMoves(friendlyPiece);
							for (auto& move : friendlyMoves) {
								bool jump = abs(move.second.row - move.first.row) > 1 || abs(move.second.col - move.first.col) > 1;
								if (!jump) continue;
								
								Piece jumped = board.At(Avg(move.second.row, move.first.row),
														Avg(move.second.col, move.first.col));
								if (jumped.GetPosition() == enemyPiece.GetPosition()) {
									return false;
								}
							}
						}
					}
				}
			}
		}
	}
	// If the king has no legal moves and no friendly pieces can capture the
	// enemy pieces surrounding it, the king is trapped.
	return true;
}

bool AI::ClearPathToKing(const Board& board, Piece piece) {
	// Check if piece is already at the king's row
	if ((piece.GetColor() == BLACK && piece.GetRow() == BOARD_SIZE - 1) ||
		(piece.GetColor() == RED && piece.GetRow() == 0)) {
		return true;
	}

	// Check if piece has moves
	auto moves = board.PossibleMoves(piece);
	if (moves.empty()) {
		return false;
	}
	
	// Check if any of the moves lead to the king's row
	for (auto& move : moves) {
		if ((piece.GetColor() == BLACK && move.first.row == BOARD_SIZE - 1) ||
			(piece.GetColor() == RED && move.first.row == 0)) {
			return true;
		}
	}

	// Recursively check if any intermediate positions have a path to king
	for (auto& move : moves) {
		Board copy(board);
		Piece target = copy.At(move.first);
		bool jump = abs(move.second.row - move.first.row) > 1 || abs(move.second.col - move.first.col) > 1;

		copy.Move(piece, target, moves, jump);
		if (ClearPathToKing(copy, target)) {
			return true;
		}
	}
	return false;
}

int AI::GetPieceProtection(const Board& board, const Piece piece) {
	int protection = 0;
	for (int i = -1; i <= 1; i += 2) {
		for (int j = -1; j <= 1; j += 2) {
			if (!IsValidPos(piece.GetRow() + i, piece.GetCol() + j)) continue;
			
			Piece adjacent = board.At(piece.GetRow() + i, piece.GetCol() + j);
			if (adjacent.GetColor() == piece.GetColor()) {
				protection++;
			}
		}
	}
	return protection;
}