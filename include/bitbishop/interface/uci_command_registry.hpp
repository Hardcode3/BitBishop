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
  bool dispatch(const std::vector<std::string>& line) const;
};

}  // namespace Uci
