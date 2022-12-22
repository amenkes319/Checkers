#pragma once

#ifndef PIECE_H
#define PIECE_H

#include <iostream>

#define PAWN  1
#define KING  2
#define BLACK 1
#define RED  -1

/**
 * Enumeration to represent the possible states of a square of the board
 * Black pieces are positive, reds are negative
 */
enum Type {
	EMPTY = 0,
	BLACK_PAWN = PAWN,
	BLACK_KING = KING,
	RED_PAWN =  -PAWN,
	RED_KING =  -KING
};

/* Enumeration to represent the coordinates of each piece on the board. */
struct Position {
	int row;
	int col;

	bool operator==(const Position& p) const {
		return this->row == p.row && this->col == p.col;
	}

	bool operator!=(const Position& p) const {
		return !(*this == p);
	}

	friend std::ostream& operator<<(std::ostream& os, Position const& p) {
		return os << p.row << ", " << p.col;
	}
};

class Piece
{
public:
	Piece();
	Piece(Type type, int row, int col);
	Piece(Type type, Position position);

	Type GetType() const { return m_type; }
	Position GetPosition() const { return m_position; }
	int GetRow() const { return m_position.row; }
	int GetCol() const { return m_position.col; }
	int GetColor() const { return (m_type == EMPTY) ? 0 : m_type / abs(m_type); }

	void SetType(Type type) { m_type = type; }
	void SetPosition(Position position) { m_position = position; }
	void SetPosition(int row, int col) { m_position.row = row; m_position.col = col; }

	bool operator==(const Piece& p) const {
		return this->m_type == p.m_type && this->m_position == p.m_position;
	}
	
	bool operator!=(const Piece& p) const {
		return !(*this == p);
	}
	
private:
	Type m_type;
	Position m_position;
};

#endif