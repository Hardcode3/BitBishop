#include <gtest/gtest.h>

#include <bitbishop/interface/uci_command_registry.hpp>

using namespace Uci;

TEST(UciCommandRegistryTest, RegistersCorreclyHandlerFunction) {
  UciCommandRegistry cmd_registry;
  std::size_t counter = 0;
  const std::string test_command_name = "test_cmd";

  EXPECT_EQ(cmd_registry.get_handlers_count(), 0);

  cmd_registry.register_handler(test_command_name, [&counter](const std::vector<std::string>& line) {
    (void)line;
    ++counter;
  });

  EXPECT_EQ(cmd_registry.get_handlers_count(), 1);
}

TEST(UciCommandRegistryTest, RegistersMultipleTimesOverridesOldHandler) {
  UciCommandRegistry cmd_registry;
  std::size_t counter = 0;
  const std::string test_command_name = "test_cmd";
  constexpr const std::size_t REGISTER_COUNT = 5;

  EXPECT_EQ(cmd_registry.get_handlers_count(), 0);

  for (std::size_t i = 0; i < REGISTER_COUNT; ++i) {
    cmd_registry.register_handler(test_command_name, [&counter](const std::vector<std::string>& line) {
      (void)line;
      ++counter;
    });
    EXPECT_EQ(cmd_registry.get_handlers_count(), 1);
  }
}

TEST(UciCommandRegistryTest, DispatchesCorreclyHandlerFunction) {
  UciCommandRegistry cmd_registry;
  std::size_t counter = 0;
  const std::string test_command_name = "test_cmd";

  cmd_registry.register_handler(test_command_name, [&counter](const std::vector<std::string>& line) {
    (void)line;
    ++counter;
  });

  EXPECT_EQ(counter, 0);

  const std::vector<std::string> test_command{test_command_name, "some", "arguments"};
  const bool dispatch_result = cmd_registry.dispatch(test_command);

  EXPECT_TRUE(dispatch_result);
  EXPECT_EQ(counter, 1);
}

TEST(UciCommandRegistryTest, DispatchesDoesNothingWithNonExistingCommand) {
  UciCommandRegistry cmd_registry;
  std::size_t counter = 0;
  const std::string test_command_name = "test_cmd";

  cmd_registry.register_handler(test_command_name, [&counter](const std::vector<std::string>& line) {
    (void)line;
    ++counter;
  });

  EXPECT_EQ(counter, 0);

  const std::vector<std::string> test_command{"unknown", "some", "arguments"};
  const bool dispatch_result = cmd_registry.dispatch(test_command);

  EXPECT_FALSE(dispatch_result);
  EXPECT_EQ(counter, 0);
}
