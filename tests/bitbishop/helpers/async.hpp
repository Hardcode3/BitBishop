#pragma once

#include <gtest/gtest.h>

#include <chrono>
#include <functional>
#include <thread>

/**
 * @brief Wait until a condition becomes true or timeout expires.
 *
 * Polls the predicate periodically until it returns true or the timeout
 * is reached. Useful for synchronizing with asynchronous engine behavior.
 *
 * @param pred Condition to evaluate.
 * @param timeout Maximum duration to wait.
 * @param interval Delay between successive checks.
 * @return true if condition became true within timeout, false otherwise.
 */
bool wait_for(std::function<bool()> pred, std::chrono::milliseconds timeout = std::chrono::milliseconds(500),
              std::chrono::milliseconds interval = std::chrono::milliseconds(10)) {
  const auto start = std::chrono::steady_clock::now();

  while (std::chrono::steady_clock::now() - start < timeout) {
    if (pred()) {
      return true;
    }
    std::this_thread::sleep_for(interval);
  }
  return false;
}

void assert_output_contains(const std::stringstream& output, const std::string& token,
                            std::chrono::milliseconds timeout = std::chrono::milliseconds(500)) {
  wait_for([&] { return output.str().find(token) != std::string::npos; }, timeout);
  ASSERT_TRUE(wait_for([&] { return output.str().find(token) != std::string::npos; }))
      << "Expected output to contain: " << token << "\nActual output:\n"
      << output.str();
}

void assert_output_not_contains(const std::stringstream& output, const std::string& token,
                                std::chrono::milliseconds timeout = std::chrono::milliseconds(500)) {
  ASSERT_TRUE(wait_for([&] { return output.str().find(token) == std::string::npos; }))
      << "Expected output not to contain: " << token << "\nActual output:\n"
      << output.str();
}
