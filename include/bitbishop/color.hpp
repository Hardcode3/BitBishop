#pragma once
#include <format>

/**
 * @brief Depicts possible piece colors in the chess game.
 *
 * This enumeration represents the two player colors in chess.
 * It can be used with std::format for automatic string conversion.
 *
 * @see std::formatter<Color> for formatting support
 */
enum class Color : std::uint8_t {
  BLACK,  ///< Black pieces/player
  WHITE   ///< White pieces/player
};

/**
 * @brief Custom formatter for Color enum to enable std::format support.
 *
 * This specialization allows Color values to be directly used in std::format
 * and std::format_to calls, automatically converting them to their string
 * representations ("black" or "white").
 *
 * @example
 * @code
 * Color player = Color::WHITE;
 * std::string msg = std::format("Current player: {}", player);
 * // Output: "Current player: white"
 * @endcode
 */
template <>
struct std::formatter<Color> {
  /**
   * @brief Parses the format specification (currently no custom format options).
   *
   * @param ctx The format parse context
   * @return Iterator to the end of the parsed format specification
   */
  constexpr auto static parse(format_parse_context& ctx) { return ctx.begin(); }

  /**
   * @brief Formats a Color value as a string.
   *
   * Converts Color::BLACK to "black" and Color::WHITE to "white".
   * Unknown values are formatted as "unknown".
   *
   * @param color The Color value to format
   * @param ctx The format context
   * @return Iterator to the end of the formatted output
   */
  auto static format(Color color, format_context& ctx) {
    string_view name = "unknown";
    switch (color) {
        // clang-format off
    case Color::BLACK: name = "black"; break;
    case Color::WHITE: name = "white"; break;
        // clang-format on
    }
    return std::format_to(ctx.out(), "{}", name);
  }
};
