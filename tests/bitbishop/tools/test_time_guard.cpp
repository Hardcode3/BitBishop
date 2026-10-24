#include <gtest/gtest.h>

#include <bitbishop/tools/time_guard.hpp>
#include <functional>

namespace bitbishop::testing {

using Tools::TimeGuard;

/**
 * @brief Repeatedly evaluates the predicate until it returns true or timeout is reached.
 * @note Helper function for the timeguard feature.
 * @param pred (Lambda) function returning a boolean being the predicate.
 * @param timeout Maximum polling duration.
 * @param interval Polling interval.
 *
 * @return true if the predicate became true at some point, false otherwise (predicate was never true until timeout).
 */
[[nodiscard]] bool inline poll_until(const std::function<bool()>& pred, std::chrono::steady_clock::duration timeout,
                                     std::chrono::steady_clock::duration interval = std::chrono::milliseconds(1)) {
  using namespace std::chrono;

  const auto deadline = steady_clock::now() + timeout;
  while (true) {
    if (pred()) {
      return true;
    }
    const auto now = steady_clock::now();
    if (now >= deadline) {
      return false;
    }
    std::this_thread::sleep_for(std::min(interval, deadline - now));
  }
}

TEST(TimeGuardTest, StopFlagIsSetWhenTimoutIsReached) {
  std::atomic_bool should_stop{false};
  TimeGuard guard(should_stop, std::chrono::milliseconds(10));

  const bool stop_flag_was_set = poll_until([&]() { return should_stop.load(); }, std::chrono::seconds(1));
  EXPECT_TRUE(stop_flag_was_set);
}

TEST(TimeGuardTest, StopFlagIsNotSetBeforeTimeout) {
  std::atomic_bool should_stop{false};
  TimeGuard guard(should_stop, std::chrono::hours(1));

  EXPECT_FALSE(should_stop.load());
}

TEST(TimeGuardTest, StopFlagIsNotSetWhenTimeGuardIsDestroyedEarly) {
  std::atomic_bool should_stop{false};
  {
    TimeGuard guard(should_stop, std::chrono::hours(1));
  }
  EXPECT_FALSE(should_stop.load());
}

}  // namespace bitbishop::testing