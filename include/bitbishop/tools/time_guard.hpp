#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace Tools {

/**
 * @class TimeGuard
 * @brief An RAII-based asynchronous countdown timer used to signal a timeout via an atomic flag.
 *
 * This class spawns a background thread that waits for a specified duration. If the duration
 * elapses without the object being destroyed or cancelled, it sets the provided @p stop_flag to true.
 *
 * @note This is designed for search algorithms where a search must
 * be terminated after a certain amount of time.
 */
class TimeGuard {
 public:
  /**
   * @brief Constructs the timer and immediately starts the countdown in a background thread.
   *
   * @param stop_flag Reference to an atomic boolean. This flag is monitored for external
   *                  cancellation and is set to @c true by this class upon timeout.
   * @param timeout   The maximum duration to wait before setting the flag.
   */
  TimeGuard(std::atomic<bool>& stop_flag, std::chrono::steady_clock::duration timeout);

  /**
   * @brief Destructor. Safely cancels the timer and joins the background thread.
   *
   * This ensures that no "zombie" threads are left running. If the timer has not yet
   * expired, it is signaled to wake up and exit immediately.
   */
  ~TimeGuard();

  // Deleted copy and move operations to prevent accidental thread ownership issues.
  TimeGuard(const TimeGuard&) = delete;
  TimeGuard& operator=(const TimeGuard&) = delete;
  TimeGuard(TimeGuard&&) = delete;
  TimeGuard& operator=(TimeGuard&&) = delete;

 private:
  /**
   * @brief The main loop for the worker thread.
   *
   * Uses a @c std::condition_variable::wait_for to sleep efficiently. It wakes up if:
   * 1. The @p timeout expires (sets @p stop_flag to @c true).
   * 2. The @p cancelled flag is set via the destructor/cancel().
   * 3. The @p stop_flag is set to @c true by external logic.
   */
  void start();

  /**
   * @brief Internal helper to signal the worker thread to stop and clean up resources.
   *
   * Thread-safe. It notifies the condition variable and joins the worker thread.
   */
  void cancel();

  std::atomic<bool>& stop_flag;
  std::chrono::steady_clock::duration timeout;
  std::mutex mutex;
  std::condition_variable cv;
  bool cancelled = false;
  std::thread worker;
};

}  // namespace Tools
