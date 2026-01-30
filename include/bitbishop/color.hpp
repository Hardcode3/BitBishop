#pragma once

#include <array>
#include <bitbishop/config.hpp>
#include <format>

/**
 * @brief Depicts possible piece colors in the chess game.
 *
 * This enumeration represents the two player colors in chess.
 * It is intentionally compact and suitable for use as an array index.
 *
 * @see std::formatter<Color> for formatting support with std::format
 * @see ColorUtil for helper utilities operating on Color values
 */
enum class Color : std::uint8_t {
  BLACK,  ///< Black pieces / player
  WHITE   ///< White pieces / player
};

/**
 * @brief Utility functions operating on the Color enum.
 *
 * This namespace groups constexpr helpers related to color manipulation,
 * such as iteration, indexing, and color inversion. Keeping these helpers
 * separate from the enum avoids polluting the global namespace while
 * preserving a clear and expressive API.
 */
namespace ColorUtil {

/**
 * @brief Converts a Color value to a zero-based array index.
 *
 * This is primarily intended for indexing color-dependent lookup tables
 * and arrays.
 *
 * @param color The color to convert
 * @return 0 for Color::BLACK, 1 for Color::WHITE
 */
CX_FN std::size_t to_index(Color color) { return static_cast<std::size_t>(color); }

/**
 * @brief Returns the opposite color.
 *
 * @param color The input color
 * @return Color::WHITE if color is Color::BLACK, otherwise Color::BLACK
 */
CX_FN Color opposite(Color color) { return color == Color::WHITE ? Color::BLACK : Color::WHITE; }

/**
 * @brief Valid Color values.
 */
CX_INLINE std::array<Color, 2> ALL = {Color::BLACK, Color::WHITE};

/**
 * @brief Mumber of colors.
 */
CX_INLINE static std::size_t SIZE = 2;

}  // namespace ColorUtil

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
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  CX_VALUE auto parse(format_parse_context& ctx) { return ctx.begin(); }

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
