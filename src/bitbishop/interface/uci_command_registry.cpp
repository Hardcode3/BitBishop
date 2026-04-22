#include <bitbishop/interface/uci_command_registry.hpp>

void Uci::UciCommandRegistry::register_handler(std::string command, Handler handler) {
  handlers.insert_or_assign(std::move(command), std::move(handler));
}

bool Uci::UciCommandRegistry::dispatch(std::vector<std::string>& line) const {
  if (line.empty()) {
    return false;
  }

  const auto it = handlers.find(line.front());
  if (it == handlers.end()) {
    return false;
  }

  it->second(line);
  return true;
}
