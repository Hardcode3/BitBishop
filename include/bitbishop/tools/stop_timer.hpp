#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace Tools {

/**
 * @brief Sets a stop flag once a timeout expires unless cancelled first.
 *
 * The timer starts immediately on construction. Destroying the instance
 * cancels the pending timeout and joins the worker thread automatically.
 */
class StopTimer {
 public:
  StopTimer(std::atomic<bool>& stop_flag, std::chrono::steady_clock::duration timeout) : stop_flag(stop_flag) {
    worker = std::thread([this, timeout] {
      std::unique_lock<std::mutex> lock(mutex);
      const bool interrupted = cv.wait_for(lock, timeout, [this] { return cancelled || this->stop_flag.load(); });
      if (!interrupted) {
        this->stop_flag.store(true);
      }
    });
  }

  ~StopTimer() { cancel(); }

  StopTimer(const StopTimer&) = delete;
  StopTimer& operator=(const StopTimer&) = delete;
  StopTimer(StopTimer&&) = delete;
  StopTimer& operator=(StopTimer&&) = delete;

 private:
  void cancel() {
    {
      std::lock_guard<std::mutex> lock(mutex);
      cancelled = true;
    }
    cv.notify_one();
    if (worker.joinable()) {
      worker.join();
    }
  }

  std::atomic<bool>& stop_flag;
  std::mutex mutex;
  std::condition_variable cv;
  bool cancelled = false;
  std::thread worker;
};

}  // namespace Tools
