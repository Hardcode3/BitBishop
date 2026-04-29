#include <gtest/gtest.h>

#include <bitbishop/tools/time_guard.hpp>

using namespace Tools;

/**
 * Verifies the flag is set after the timer thread finishes its work.
 */
TEST(TimeGuardTest, SetsFlagOnTimeout) {
  std::atomic_bool should_stop{false};
  {
    TimeGuard guard(should_stop, std::chrono::milliseconds(10));
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }

  EXPECT_TRUE(should_stop);
}

/**
 * Verifies that the guard stops early if destroyed (RAII works).
 * This ensures the condition_variable is actually interrupting the sleep.
 */
TEST(TimeGuardTest, DestructorJoinsQuicklyBeforeTimeout) {
  std::atomic_bool should_stop{false};
  auto timeout = std::chrono::milliseconds(1'000);
  auto sleep_time = std::chrono::milliseconds(10);

  auto start = std::chrono::steady_clock::now();
  {
    TimeGuard guard(should_stop, timeout);
    std::this_thread::sleep_for(sleep_time);
  }
  auto end = std::chrono::steady_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  // Destruction should be quick
  EXPECT_LE(duration.count(), 2 * sleep_time.count());
  EXPECT_FALSE(should_stop);
}

/**
 * External code sets stop flag to true, resulting in time guard interruption.
 * Interruption should not alter external stop flag, already set to true.
 */
TEST(TimeGuardTest, GuardDoesNotModifyExternalFlagOnExternalBoolFlagInterruption) {
  std::atomic_bool should_stop{false};
  std::chrono::milliseconds timeout(20);
  {
    TimeGuard guard(should_stop, timeout);
    should_stop.store(true);
  }

  EXPECT_TRUE(should_stop);
}

/**
 * Time guard should not alter the stop flag when still alive.
 */
TEST(TimeGuardTest, GuardDoesNotSetExternalFlagWhenStillAlive) {
  std::atomic_bool should_stop{false};
  {
    std::chrono::milliseconds timeout(20);
    TimeGuard guard(should_stop, timeout);

    EXPECT_FALSE(should_stop);
  }
}
