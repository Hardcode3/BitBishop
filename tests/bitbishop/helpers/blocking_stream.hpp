#include <condition_variable>
#include <mutex>
#include <sstream>

/**
 * @brief Thread-safe blocking stream buffer.
 *
 * Provides a producer-consumer mechanism for feeding input to a std::istream.
 * Data is pushed externally and consumed lazily via underflow(), blocking
 * until data is available or the stream is closed.
 */
class BlockingStreamBuf : public std::streambuf {
 public:
  /**
   * @brief Append data to the buffer and notify waiting readers.
   * @param data Input data to append.
   */
  void push(const std::string& data) {
    std::lock_guard<std::mutex> lock(m_);
    buffer_ += data;
    cv_.notify_one();
  }

  /**
   * @brief Mark the buffer as closed and wake all waiting readers.
   */
  void close() {
    std::lock_guard<std::mutex> lock(m_);
    closed_ = true;
    cv_.notify_all();
  }

 protected:
  /**
   * @brief Provide next character to the stream or blocks until one exists.
   *
   * Waits until data is available or the stream is closed. Returns EOF when
   * no more data is available and the stream is closed.
   *
   * Example:
   * - Call #1 → 'u'  pos=1
   * - Call #2 → 'c'  pos=2
   * - Call #3 → 'i'  pos=3
   * - Call #4 → '\n' pos=4
   * - Call #5 → waits or EOF
   */
  int_type underflow() override {
    std::unique_lock<std::mutex> lock(m_);
    cv_.wait(lock, [&] { return pos_ < buffer_.size() || closed_; });

    if (pos_ >= buffer_.size()) {
      return traits_type::eof();
    }

    char* base = &buffer_[pos_];

    // Set the values of the pointers defining the get area
    this->setg(base, base, base + 1);

    ++pos_;

    // Returns the pointer to the current character (get pointer) in the get area.
    return traits_type::to_int_type(*this->gptr());
  }

 private:
  std::string buffer_;  ///< Internal storage
  size_t pos_ = 0;      ///< Current read position

  std::mutex m_;                ///< Synchronization mutex
  std::condition_variable cv_;  ///< Notification mechanism
  bool closed_ = false;         ///< Indicates no more data will arrive
};

/**
 * @brief Blocking input stream backed by BlockingStreamBuf.
 *
 * Acts like std::cin for the engine, but allows controlled, thread-safe
 * injection of input from tests.
 *
 * @see UciEngineTest
 */
class BlockingIStream : public std::istream {
 public:
  /**
   * @brief Construct stream using internal blocking buffer.
   */
  BlockingIStream() : std::istream(&buf_) {}

  /**
   * @brief Write data into the stream.
   * @param s Input string (typically UCI command + '\n').
   */
  void write(const std::string& s) { buf_.push(s); }

  /**
   * @brief Close the stream (signals EOF to readers).
   */
  void close() { buf_.close(); }

 private:
  BlockingStreamBuf buf_;  ///< Underlying buffer
};
