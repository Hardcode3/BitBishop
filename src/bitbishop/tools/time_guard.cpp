#include <bitbishop/tools/time_guard.hpp>

namespace Tools {

TimeGuard::TimeGuard(std::atomic<bool>& stop_flag, std::chrono::steady_clock::duration timeout)
    : stop_flag(stop_flag), timeout(timeout) {
  worker = std::thread(&TimeGuard::start, this);
}

TimeGuard::~TimeGuard() { cancel(); }

void TimeGuard::start() {
  std::unique_lock<std::mutex> lock(mutex);

  const bool was_interrupted = cv.wait_for(lock, timeout, [this] { return cancelled || this->stop_flag.load(); });

  if (!was_interrupted) {
    this->stop_flag.store(true);
  }
}

void TimeGuard::cancel() {
  {
    std::lock_guard<std::mutex> lock(mutex);
    cancelled = true;
  }
  cv.notify_one();

  if (worker.joinable()) {
    worker.join();
  }
}

}  // namespace Tools
