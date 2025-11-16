/**
 * @file move_test_helpers.hpp
 * @brief Utility functions for inspecting and validating generated move lists in tests.
 *
 * These helpers simplify writing GoogleTest assertions for move generation
 * by providing reusable predicates, counters, and filters.
 */

#include <gtest/gtest.h>

#include <algorithm>
#include <bitbishop/color.hpp>
#include <bitbishop/move.hpp>
#include <optional>
#include <vector>

/**
 * @brief Counts moves that satisfy a predicate.
 * @param moves Vector of moves to analyze.
 * @param pred Predicate applied to each move.
 * @return Number of moves matching the predicate.
 */
template <typename Pred>
int count_if(const std::vector<Move>& moves, Pred pred) {
  return std::count_if(moves.begin(), moves.end(), pred);
}

/**
 * @brief Filters moves that satisfy a predicate.
 * @param moves Vector of moves to filter.
 * @param pred Predicate applied to each move.
 * @return Vector of moves matching the predicate.
 */
template <typename Pred>
std::vector<Move> filter_moves(const std::vector<Move>& moves, Pred pred) {
  std::vector<Move> result;
  std::copy_if(moves.begin(), moves.end(), std::back_inserter(result), pred);
  return result;
}

/**
 * @brief Checks if a specific move exists in the move list.
 * @param moves Vector of moves to search.
 * @param target Move to search for.
 * @return true if an exact match is found, false otherwise.
 */
bool contains_move(const std::vector<Move>& moves, const Move& target) {
  return std::any_of(moves.begin(), moves.end(), [&](const Move& m) {
    return m.from == target.from && m.to == target.to && m.promotion == target.promotion &&
           m.is_capture == target.is_capture && m.is_en_passant == target.is_en_passant &&
           m.is_castling == target.is_castling;
  });
}

/**
 * @brief Checks if the list contains a move matching given flag constraints.
 * @param moves Vector of moves to search.
 * @param is_capture Expected capture flag (ignored if nullopt).
 * @param is_castling Expected castling flag (ignored if nullopt).
 * @param is_en_passant Expected en passant flag (ignored if nullopt).
 * @param has_promotion Expected promotion presence (ignored if nullopt).
 * @return true if any move matches all specified criteria.
 */
bool contains_move_with_flags(const std::vector<Move>& moves, std::optional<bool> is_capture = std::nullopt,
                              std::optional<bool> is_castling = std::nullopt,
                              std::optional<bool> is_en_passant = std::nullopt,
                              std::optional<bool> has_promotion = std::nullopt) {
  return std::any_of(moves.begin(), moves.end(), [&](const Move& m) {
    if (is_capture && m.is_capture != *is_capture) return false;
    if (is_castling && m.is_castling != *is_castling) return false;
    if (is_en_passant && m.is_en_passant != *is_en_passant) return false;
    if (has_promotion && m.promotion.has_value() != *has_promotion) return false;
    return true;
  });
}

/**
 * @brief Validates flag combinations for all moves.
 *
 * Ensures that:
 * - Castling moves are not captures, en passant, or promotions.
 * - En passant moves are captures but not castling.
 *
 * @param moves Vector of moves to validate.
 * @return true if all moves have valid flag combinations.
 */
bool has_valid_move_flags(const std::vector<Move>& moves) {
  return std::all_of(moves.begin(), moves.end(), [](const Move& m) {
    if (m.is_castling) return !m.is_capture && !m.is_en_passant && !m.promotion.has_value();
    if (m.is_en_passant) return m.is_capture && !m.is_castling;
    return true;
  });
}

/**
 * @brief Counts moves originating from a given square.
 */
int count_moves_from(const std::vector<Move>& moves, Square from) {
  return count_if(moves, [&](const Move& m) { return m.from == from; });
}

/**
 * @brief Counts moves targeting a given square.
 */
int count_moves_to(const std::vector<Move>& moves, Square to) {
  return count_if(moves, [&](const Move& m) { return m.to == to; });
}

/**
 * @brief Counts all capture moves (including en passant).
 */
int count_captures(const std::vector<Move>& moves) {
  return count_if(moves, [](const Move& m) { return m.is_capture; });
}

/**
 * @brief Counts normal captures (excluding en passant).
 */
int count_regular_captures(const std::vector<Move>& moves) {
  return count_if(moves, [](const Move& m) { return m.is_capture && !m.is_en_passant; });
}

/**
 * @brief Counts en passant captures.
 */
int count_en_passant(const std::vector<Move>& moves) {
  return count_if(moves, [](const Move& m) { return m.is_en_passant; });
}

/**
 * @brief Counts all castling moves.
 */
int count_castling(const std::vector<Move>& moves) {
  return count_if(moves, [](const Move& m) { return m.is_castling; });
}

/**
 * @brief Counts promotions of any type.
 */
int count_promotions(const std::vector<Move>& moves) {
  return count_if(moves, [](const Move& m) { return m.promotion.has_value(); });
}

/**
 * @brief Counts promotions to a specific piece type.
 * @param piece Piece type and color to match.
 */
int count_promotions_to(const std::vector<Move>& moves, const Piece& piece) {
  return count_if(moves, [&](const Move& m) { return m.promotion.has_value() && m.promotion.value() == piece; });
}

/**
 * @brief Counts promotions that also capture.
 */
int count_promotion_captures(const std::vector<Move>& moves) {
  return count_if(moves, [](const Move& m) { return m.promotion.has_value() && m.is_capture; });
}

/**
 * @brief Counts promotions that are not captures.
 */
int count_quiet_promotions(const std::vector<Move>& moves) {
  return count_if(moves, [](const Move& m) { return m.promotion.has_value() && !m.is_capture; });
}

/**
 * @brief Counts quiet moves (non-capture, non-promotion, non-castling).
 */
int count_quiet_moves(const std::vector<Move>& moves) {
  return count_if(moves, [](const Move& m) {
    return !m.is_capture && !m.is_castling && !m.is_en_passant && !m.promotion.has_value();
  });
}

/**
 * @brief Counts kingside castling moves for the given side.
 */
int count_kingside_castling(const std::vector<Move>& moves, Color side) {
  Square target = (side == Color::WHITE) ? Squares::G1 : Squares::G8;
  return count_if(moves, [&](const Move& m) { return m.is_castling && m.to == target; });
}

/**
 * @brief Counts queenside castling moves for the given side.
 */
int count_queenside_castling(const std::vector<Move>& moves, Color side) {
  Square target = (side == Color::WHITE) ? Squares::C1 : Squares::C8;
  return count_if(moves, [&](const Move& m) { return m.is_castling && m.to == target; });
}

/**
 * @brief Finds all moves between two given squares.
 * @param from Source square.
 * @param to Destination square.
 * @return Vector of moves matching the from-to combination.
 */
std::vector<Move> find_moves_between(const std::vector<Move>& moves, Square from, Square to) {
  return filter_moves(moves, [&](const Move& m) { return m.from == from && m.to == to; });
}
