#include "Piece.h"

Piece::Piece() {
	m_type = EMPTY;
	m_position.row = -1;
	m_position.col = -1;
}

Piece::Piece(Type type, int row, int col) {
	m_type = type;
	m_position.row = row;
	m_position.col = col;
}

Piece::Piece(Type type, Position position) {
	m_type = type;
	m_position = position;
}