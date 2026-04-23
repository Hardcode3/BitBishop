#include <gtest/gtest.h>

#include <bitbishop/helpers/async.hpp>
#include <bitbishop/helpers/blocking_stream.hpp>
#include <bitbishop/interface/uci_command_channel.hpp>
#include <chrono>
#include <functional>
#include <sstream>
#include <thread>

using namespace std::chrono_literals;

TEST(UciCommandChannelTest, WaitAndPopReturnsFalseWhenNotStarted) {
  std::istringstream input("uci\n");
  Uci::UciCommandChannel channel(input);
  // UciCommandChannel was not started so nothing is listening for commands

  std::string line;
  EXPECT_FALSE(channel.wait_and_pop_line(line, 5ms));
  EXPECT_FALSE(channel.eof());
}

TEST(UciCommandChannelTest, ReadsCommandsInOrderAndSignalsEof) {
  std::istringstream input("uci\nisready\n");
  Uci::UciCommandChannel channel(input);

  channel.start();

  std::string line;
  ASSERT_TRUE(channel.wait_and_pop_line(line, 100ms));
  EXPECT_EQ(line, "uci");

  ASSERT_TRUE(channel.wait_and_pop_line(line, 100ms));
  EXPECT_EQ(line, "isready");

  ASSERT_TRUE(wait_for([&] { return channel.eof(); }));
  EXPECT_FALSE(channel.wait_and_pop_line(line, 20ms));

  channel.stop();
}

TEST(UciCommandChannelTest, WaitAndPopTimesOutWhenNoInputYet) {
  BlockingIStream input;  // needs input.close() to signal eof
  Uci::UciCommandChannel channel(input);

  channel.start();

  std::string line;
  EXPECT_FALSE(channel.wait_and_pop_line(line, 20ms));
  EXPECT_FALSE(channel.eof());

  input.close();
  ASSERT_TRUE(wait_for([&] { return channel.eof(); }));
  channel.stop();
}

TEST(UciCommandChannelTest, ReceivesCommandProducedAfterStart) {
  BlockingIStream input;  // needs input.close() to signal eof
  Uci::UciCommandChannel channel(input);

  channel.start();

  std::thread producer([&input] {
    std::this_thread::sleep_for(10ms);
    input.write("go depth 4\n");
    input.close();
  });

  std::string line;
  ASSERT_TRUE(channel.wait_and_pop_line(line, 200ms));
  EXPECT_EQ(line, "go depth 4");

  ASSERT_TRUE(wait_for([&] { return channel.eof(); }));
  EXPECT_FALSE(channel.wait_and_pop_line(line, 20ms));

  producer.join();
  channel.stop();
}
