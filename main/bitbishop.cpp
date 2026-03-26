#include <bitbishop/interface/uci_engine.hpp>
#include <iostream>

/**
 * @brief Launch the UCI engine loop.
 *
 * Constructs a UCI engine bound to `std::cin`/`std::cout` and runs its
 * main command-processing loop until termination.
 *
 * @return Exit code (0 on normal termination).
 */
int main() {
  Uci::UciEngine engine(std::cin, std::cout);
  engine.loop();
  return 0;
}
