#include <gtest/gtest.h>

#include <algorithm>
#include <chess_engine/move.hpp>
#include <vector>

/**
 * @brief Checks if a specific move exists in a move list.
 *
 * Compares all move properties: source square, destination square, promotion piece,
 * and special move flags (capture, en passant, castling).
 *
 * @param moves Vector of moves to search through
 * @param target Move to search for
 * @return true if an exact match is found, false otherwise
 */
bool contains_move(const std::vector<Move>& moves, const Move& target) {
  return std::find_if(moves.begin(), moves.end(), [&](const Move& m) {
           return m.from == target.from && m.to == target.to && m.promotion == target.promotion &&
                  m.is_capture == target.is_capture && m.is_en_passant == target.is_en_passant &&
                  m.is_castling == target.is_castling;
         }) != moves.end();
}

/**
 * @brief Counts moves originating from a specific square.
 *
 * Useful for testing piece mobility or verifying that a piece has the expected
 * number of available moves from its current position.
 *
 * @param moves Vector of moves to analyze
 * @param from Source square to filter by
 * @return Number of moves starting from the specified square
 */
int count_moves_from(const std::vector<Move>& moves, Square from) {
  return std::count_if(moves.begin(), moves.end(), [&](const Move& m) { return m.from == from; });
}

/**
 * @brief Counts moves targeting a specific square.
 *
 * Useful for testing if a square is properly attacked or defended by multiple pieces.
 *
 * @param moves Vector of moves to analyze
 * @param to Destination square to filter by
 * @return Number of moves targeting the specified square
 */
int count_moves_to(const std::vector<Move>& moves, Square to) {
  return std::count_if(moves.begin(), moves.end(), [&](const Move& m) { return m.to == to; });
}

/**
 * @brief Counts capture moves in a move list.
 *
 * A capture move has the is_capture flag set to true, indicating the destination
 * square contains an enemy piece (or en passant capture).
 *
 * @param moves Vector of moves to analyze
 * @return Number of capture moves
 */
int count_captures(const std::vector<Move>& moves) {
  return std::count_if(moves.begin(), moves.end(), [](const Move& m) { return m.is_capture; });
}

/**
 * @brief Counts promotion moves in a move list.
 *
 * A promotion move has a non-empty promotion field, indicating the pawn
 * promotes to a specific piece (Queen, Rook, Bishop, or Knight).
 *
 * @param moves Vector of moves to analyze
 * @return Number of promotion moves (includes all promotion types)
 */
int count_promotions(const std::vector<Move>& moves) {
  return std::count_if(moves.begin(), moves.end(), [](const Move& m) { return m.promotion.has_value(); });
}

/**
 * @brief Counts en passant capture moves in a move list.
 *
 * En passant is a special pawn capture where the captured pawn is not on the
 * destination square but adjacent to it.
 *
 * @param moves Vector of moves to analyze
 * @return Number of en passant captures
 */
int count_en_passant(const std::vector<Move>& moves) {
  return std::count_if(moves.begin(), moves.end(), [](const Move& m) { return m.is_en_passant; });
}

/**
 * @brief Counts promotions to a specific piece type.
 *
 * Useful for verifying that all four promotion options (Q, R, B, N) are correctly
 * generated, or for testing specific promotion scenarios.
 *
 * Example: count_promotions_to(moves, Piece(Color::WHITE, PieceType::QUEEN))
 *
 * @param moves Vector of moves to analyze
 * @param piece Specific piece type and color to filter by
 * @return Number of promotions to the specified piece
 */
int count_promotions_to(const std::vector<Move>& moves, const Piece& piece) {
  return std::count_if(moves.begin(), moves.end(),
                       [&](const Move& m) { return m.promotion.has_value() && m.promotion.value() == piece; });
}
