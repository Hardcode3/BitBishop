#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Uci {

/**
 * @brief Registry mapping UCI command names to handlers.
 */
class UciCommandRegistry {
 public:
  using Handler = std::function<void(const std::vector<std::string>&)>;

 private:
  std::unordered_map<std::string, Handler> handlers;

 public:
  /**
   * @brief Registers or overrides one command handler.
   */
  void register_handler(std::string command, Handler handler);

  /**
   * @brief Dispatches the given command line if a handler exists.
   *
   * @return true when a handler was found and executed, false otherwise.
   */
  [[nodiscard]] bool dispatch(const std::vector<std::string>& line) const;

  /**
   * @brief Retrieves the number of handlers currently registered.
   *
   * @return Number of handlers registered in the UciCommandRegistry.
   */
  [[nodiscard]] std::size_t get_handlers_count() const noexcept { return handlers.size(); }
};

}  // namespace Uci
