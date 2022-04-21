#include "Piece.h"

Piece::Piece(PieceColor color) {
	m_color = color;
	m_bKinged = false;
}

bool Piece::MakeKing() {
	if (m_bKinged) return false;

	m_bKinged = true;

	return true;
}