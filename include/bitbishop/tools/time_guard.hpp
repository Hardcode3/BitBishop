#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace Tools {

/**
 * @class TimeGuard
 * @brief An RAII-based asynchronous countdown timer used to signal a timeout via an atomic flag.
 *
 * This class spawns a background thread that waits for a specified duration. If the duration
 * elapses without the object being destroyed / cancelled, it sets the provided @p stop_flag to true.
 *
 * @note This is designed for search algorithms where a search must
 * be terminated after a certain amount of time.
 */
class TimeGuard {
 private:
  std::atomic<bool>& m_stop_flag;  ///< External stop flag set by the TimeGuard upon timeout.

  std::chrono::steady_clock::duration m_timeout;  ///< Timeout setting external stop flag to true after delay's end.
  bool m_cancelled = false;                       ///< Boolean flag set to true by the cancel() method.
  std::thread m_worker;                           ///< Thread running the timer.
  std::mutex m_mutex;
  std::condition_variable m_cv;

 public:
  /**
   * @brief Constructs the timer and immediately starts the countdown in a background thread.
   *
   * @param stop_flag Reference to an atomic boolean. This flag is set to true by this class upon timeout,
   *                  but is NOT monitored for external cancellation.
   * @param timeout   The maximum duration to wait before setting the external stop flag.
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
   * Uses a @c std::condition_variable::wait_for to sleep efficiently.
   * It wakes up if:
   * 1. The @p m_timeout expires (sets @p m_stop_flag to @c true).
   * 2. The @p m_cancelled flag is set via a destructor call.
   *
   * NB: @p m_stop_flag cannot be set to @c true by external logic to stop the timer.
   */
  void start();

  /**
   * @brief Internal helper to signal the worker thread to stop and clean up resources.
   *
   * It notifies the condition variable and joins the worker thread.
   * Thread-safe as it can only by called privately once by the destructor.
   * (It uses thread.join, which is not thread safe when called multiple times and would lead
   * to undefined behavior).
   */
  void cancel();
};

}  // namespace Tools
