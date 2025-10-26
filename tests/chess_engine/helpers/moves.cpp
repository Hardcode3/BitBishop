#include <gtest/gtest.h>

#include <algorithm>
#include <chess_engine/move.hpp>
#include <vector>

// Helper function to check if a move exists in the vector
bool contains_move(const std::vector<Move>& moves, const Move& target) {
  return std::find_if(moves.begin(), moves.end(), [&](const Move& m) {
           return m.from == target.from && m.to == target.to && m.promotion == target.promotion &&
                  m.is_capture == target.is_capture && m.is_en_passant == target.is_en_passant &&
                  m.is_castling == target.is_castling;
         }) != moves.end();
}

// Helper function to count moves with specific properties
int count_moves_from(const std::vector<Move>& moves, Square from) {
  return std::count_if(moves.begin(), moves.end(), [&](const Move& m) { return m.from == from; });
}

int count_moves_to(const std::vector<Move>& moves, Square to) {
  return std::count_if(moves.begin(), moves.end(), [&](const Move& m) { return m.to == to; });
}

int count_captures(const std::vector<Move>& moves) {
  return std::count_if(moves.begin(), moves.end(), [](const Move& m) { return m.is_capture; });
}

int count_promotions(const std::vector<Move>& moves) {
  return std::count_if(moves.begin(), moves.end(), [](const Move& m) { return m.promotion.has_value(); });
}

int count_en_passant(const std::vector<Move>& moves) {
  return std::count_if(moves.begin(), moves.end(), [](const Move& m) { return m.is_en_passant; });
}

// Helper function to count moves with specific promotion piece
int count_promotions_to(const std::vector<Move>& moves, const Piece& piece) {
  return std::count_if(moves.begin(), moves.end(),
                       [&](const Move& m) { return m.promotion.has_value() && m.promotion.value() == piece; });
}
