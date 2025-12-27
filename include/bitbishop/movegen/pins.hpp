#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/lookups/between_squares.hpp>
#include <bitbishop/lookups/bishop_rays.hpp>
#include <bitbishop/lookups/rook_rays.hpp>
#include <bitbishop/square.hpp>

/**
 * @brief Enumeration of sliding ray types used for pin detection.
 *
 * Distinguishes between rook-like and bishop-like rays,
 * determining which enemy sliders can produce a pin.
 */
enum class RayType {
  ROOK,   ///< Orthogonal rays (rook / queen)
  BISHOP  ///< Diagonal rays (bishop / queen)
};

/**
 * @brief Enumeration of ray directions from the king square.
 *
 * Used to determine ray orientation and correct blocker ordering
 * (LSB vs MSB) when scanning occupied squares.
 */
enum class RayDir {
  N,
  S,
  E,
  W,  ///< Orthogonal directions
  NE,
  NW,
  SE,
  SW  ///< Diagonal directions
};

/**
 * @brief Describes a single ray used for pin detection.
 *
 * A PinRay represents one directional ray starting from the king square,
 * along with the information required to:
 *  - determine blocker ordering
 *  - validate enemy sliding pieces that can create pins
 */
struct PinRay {
  Bitboard ray;  ///< Precomputed ray bitboard from the king square
  RayType type;  ///< Type of sliding piece relevant for this ray
  RayDir dir;    ///< Direction of the ray

  /**
   * @brief Returns the first blocker along this ray.
   *
   * The function selects either the least-significant or most-significant
   * bit depending on the ray direction.
   *
   * @param bb Bitboard of occupied squares along the ray
   * @return Square of the closest blocker to the king
   */
  [[nodiscard]] Square first_blocker(Bitboard bb) const {
    switch (dir) {
      case RayDir::N:
      case RayDir::NE:
      case RayDir::E:
      case RayDir::NW:
        return bb.lsb().value();
      default:
        return bb.msb().value();
    }
  }

  /**
   * @brief Checks whether a piece matches the required enemy slider for this ray.
   *
   * For rook rays, accepts enemy rooks or queens.
   * For bishop rays, accepts enemy bishops or queens.
   *
   * @param piece Bitboard with a single enemy piece
   * @param board Current board state
   * @param them Enemy color
   * @return true if the piece can create a pin along this ray
   */
  [[nodiscard]] bool matches_slider(Bitboard piece, const Board& board, Color them) const {
    return (type == RayType::BISHOP) ? (piece & (board.bishops(them) | board.queens(them)))
                                     : (piece & (board.rooks(them) | board.queens(them)));
  }
};

/**
 * @brief Result structure for pin computation.
 *
 * Contains:
 *  - a bitboard of all pinned friendly pieces
 *  - a per-square pin ray mask restricting legal movement
 */
struct PinResult {
  Bitboard pinned = Bitboard::Zeros();
  std::array<Bitboard, Const::BOARD_SIZE> pin_ray{};
};

/**
 * @brief Scans a single ray from the king to detect a possible pin.
 *
 * The function detects the classical pin pattern:
 *   king → friendly piece → enemy sliding piece
 *
 * If such a configuration is found:
 *  - the friendly piece is marked as pinned
 *  - its legal movement is restricted to the pin ray
 *
 * @param king_sq Square of the friendly king
 * @param ray_info Ray descriptor (direction, type, bitboard)
 * @param board Current board position
 * @param us Friendly color
 * @param result Accumulated pin result (modified in place)
 */
void scan_pin_ray(Square king_sq, const PinRay& ray_info, const Board& board, Color us, PinResult& result);

/**
 * @brief Computes all pinned pieces for the given side.
 *
 * Pins are detected by scanning all rook and bishop rays
 * originating from the king square.
 *
 * A piece is considered pinned if moving it would expose
 * the king to a sliding attack.
 *
 * @param king_sq Square of the friendly king
 * @param board Current board position
 * @param us Friendly color
 * @return PinResult containing pinned pieces and their pin rays
 */
PinResult compute_pins(Square king_sq, const Board& board, Color us);
