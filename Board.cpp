#include "Board.h"

Board::Board() {
	ResetBoard();
}

void Board::ResetBoard() {
	DeleteBoard();

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE / 2; j++) {
			if (i < 3) {
				board[i][j] = new Piece(PieceColor::BLACK);
			} else if (i < 5) {
				board[i][j] = nullptr;
			} else {
				board[i][j] = new Piece(PieceColor::RED);
			}
		}
	}
}

void Board::DeleteBoard() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE / 2; j++) {
			delete board[i][j];
		}
	}
}

Board::~Board() {
	DeleteBoard();
}