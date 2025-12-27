#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test first_blocker returns LSB for northward ray.
 * @brief Confirms PinRay::first_blocker() returns least significant bit
 *        for north-facing rays.
 */
TEST(PinRayTest, FirstBlockerNorthDirection) {
  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};

  Bitboard blockers = Bitboard::Zeros();
  blockers.set(E3);
  blockers.set(E5);
  blockers.set(E7);

  Square blocker = ray.first_blocker(blockers);

  EXPECT_EQ(blocker, E3);
}

/**
 * @test first_blocker returns MSB for southward ray.
 * @brief Confirms PinRay::first_blocker() returns most significant bit
 *        for south-facing rays.
 */
TEST(PinRayTest, FirstBlockerSouthDirection) {
  PinRay ray{Lookups::ROOK_SOUTH_RAYS[E8.value()], RayType::ROOK, RayDir::S};

  Bitboard blockers = Bitboard::Zeros();
  blockers.set(E6);
  blockers.set(E4);
  blockers.set(E2);

  Square blocker = ray.first_blocker(blockers);

  EXPECT_EQ(blocker, E6);
}

/**
 * @test first_blocker returns LSB for eastward ray.
 * @brief Confirms PinRay::first_blocker() returns least significant bit
 *        for east-facing rays.
 */
TEST(PinRayTest, FirstBlockerEastDirection) {
  PinRay ray{Lookups::ROOK_EAST_RAYS[A4.value()], RayType::ROOK, RayDir::E};

  Bitboard blockers = Bitboard::Zeros();
  blockers.set(C4);
  blockers.set(E4);
  blockers.set(G4);

  Square blocker = ray.first_blocker(blockers);

  EXPECT_EQ(blocker, C4);
}

/**
 * @test first_blocker returns MSB for westward ray.
 * @brief Confirms PinRay::first_blocker() returns most significant bit
 *        for west-facing rays.
 */
TEST(PinRayTest, FirstBlockerWestDirection) {
  PinRay ray{Lookups::ROOK_WEST_RAYS[H4.value()], RayType::ROOK, RayDir::W};

  Bitboard blockers = Bitboard::Zeros();
  blockers.set(F4);
  blockers.set(D4);
  blockers.set(B4);

  Square blocker = ray.first_blocker(blockers);

  EXPECT_EQ(blocker, F4);
}

/**
 * @test first_blocker returns LSB for northeast ray.
 * @brief Confirms PinRay::first_blocker() returns least significant bit
 *        for northeast-facing rays.
 */
TEST(PinRayTest, FirstBlockerNortheastDirection) {
  PinRay ray{Lookups::BISHOP_NORTHEAST_RAYS[A1.value()], RayType::BISHOP, RayDir::NE};

  Bitboard blockers = Bitboard::Zeros();
  blockers.set(C3);
  blockers.set(E5);
  blockers.set(G7);

  Square blocker = ray.first_blocker(blockers);

  EXPECT_EQ(blocker, C3);
}

/**
 * @test first_blocker returns LSB for northwest ray.
 * @brief Confirms PinRay::first_blocker() returns least significant bit
 *        for northwest-facing rays.
 */
TEST(PinRayTest, FirstBlockerNorthwestDirection) {
  PinRay ray{Lookups::BISHOP_NORTHWEST_RAYS[H1.value()], RayType::BISHOP, RayDir::NW};

  Bitboard blockers = Bitboard::Zeros();
  blockers.set(F3);
  blockers.set(D5);
  blockers.set(B7);

  Square blocker = ray.first_blocker(blockers);

  EXPECT_EQ(blocker, F3);
}

/**
 * @test first_blocker returns MSB for southeast ray.
 * @brief Confirms PinRay::first_blocker() returns most significant bit
 *        for southeast-facing rays.
 */
TEST(PinRayTest, FirstBlockerSoutheastDirection) {
  PinRay ray{Lookups::BISHOP_SOUTHEAST_RAYS[A8.value()], RayType::BISHOP, RayDir::SE};

  Bitboard blockers = Bitboard::Zeros();
  blockers.set(C6);
  blockers.set(E4);
  blockers.set(G2);

  Square blocker = ray.first_blocker(blockers);

  EXPECT_EQ(blocker, C6);
}

/**
 * @test first_blocker returns MSB for southwest ray.
 * @brief Confirms PinRay::first_blocker() returns most significant bit
 *        for southwest-facing rays.
 */
TEST(PinRayTest, FirstBlockerSouthwestDirection) {
  PinRay ray{Lookups::BISHOP_SOUTHWEST_RAYS[H8.value()], RayType::BISHOP, RayDir::SW};

  Bitboard blockers = Bitboard::Zeros();
  blockers.set(F6);
  blockers.set(D4);
  blockers.set(B2);

  Square blocker = ray.first_blocker(blockers);

  EXPECT_EQ(blocker, F6);
}
