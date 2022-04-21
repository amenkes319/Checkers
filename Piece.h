#pragma once

#ifndef PIECE_H
#define PIECE_H

#include <cstdlib>

/**
 * Represents two possible piece colors
 */
enum class PieceColor {
	RED,
	BLACK
};

class Piece
{
public:
	Piece(PieceColor color);

    /**
	 * Makes a piece a king if it is not already.
	 * @return false if piece is already a king.
	 */
	bool MakeKing();

private:
	PieceColor m_color;
	bool m_bKinged;
};

#endif