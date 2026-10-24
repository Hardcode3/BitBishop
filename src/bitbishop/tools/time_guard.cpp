#include <bitbishop/tools/time_guard.hpp>

namespace bitbishop {

namespace Tools {

TimeGuard::TimeGuard(std::atomic<bool>& stop_flag, std::chrono::steady_clock::duration timeout)
    : m_stop_flag(stop_flag), m_timeout(timeout) {
  m_worker = std::thread(&TimeGuard::start, this);
}

TimeGuard::~TimeGuard() { cancel(); }

void TimeGuard::start() {
  std::unique_lock<std::mutex> lock(m_mutex);

  auto stop_predicate = [this] { return m_cancelled; };
  // NB: cv.wait_for() unlocks the mutext before going to sleep, letting cancel() use the lock.
  const bool was_interrupted = m_cv.wait_for(lock, m_timeout, stop_predicate);

  if (!was_interrupted) {
    this->m_stop_flag.store(true);
  }
}

void TimeGuard::cancel() {
  {
    std::scoped_lock lock(m_mutex);
    m_cancelled = true;
  }
  m_cv.notify_one();

  if (m_worker.joinable()) {
    m_worker.join();
  }
}

}  // namespace Tools

}  // namespace bitbishop