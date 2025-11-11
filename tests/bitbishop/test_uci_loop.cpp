#include <gtest/gtest.h>
#include <sstream>
#include "../main/uci_loop.cpp"

/**
 * @brief Test fixture for UCI loop tests
 *
 * This fixture provides input and output string streams for testing the UCI loop
 * in isolation. It handles setup and cleanup of these streams for each test.
 */
class UciLoopTest : public ::testing::Test {
protected:
    std::stringstream input;
    std::stringstream output;

    void SetUp() override {
        // Clear streams before each test
        input.clear();
        output.clear();
    }

    void TearDown() override {
        // Clean up after each test
        input.str("");
        output.str("");
    }
};

/**
 * @brief Tests the UCI initialization command
 *
 * Verifies that the engine correctly identifies itself and confirms UCI protocol
 * support. This is the first command any UCI-compatible GUI will send to the engine.
 *
 * Expected responses:
 * - Engine name identification
 * - Author identification
 * - UCI protocol confirmation
 */
TEST_F(UciLoopTest, UciCommand) {
    input << "uci\n";
    uci_loop(input, output);

    std::string response = output.str();
    EXPECT_TRUE(response.find("id name ChessEngine") != std::string::npos);
    EXPECT_TRUE(response.find("id author Hardcode") != std::string::npos);
    EXPECT_TRUE(response.find("uciok") != std::string::npos);
}

/**
 * @brief Tests the isready command
 *
 * Verifies that the engine can confirm its readiness to receive commands.
 * This is a critical command that ensures the engine is properly initialized
 * and ready to process further commands.
 */
TEST_F(UciLoopTest, IsReadyCommand) {
    input << "isready\n";
    uci_loop(input, output);

    std::string response = output.str();
    EXPECT_EQ(response, "readyok\n");
}

/**
 * @brief Tests the go command
 *
 * Verifies that the engine can process a move calculation request.
 * This is one of the most important commands as it triggers the engine's
 * main functionality of finding the best move.
 */
TEST_F(UciLoopTest, GoCommand) {
    input << "go\n";
    uci_loop(input, output);

    std::string response = output.str();
    EXPECT_EQ(response, "bestmove e2e4\n");
}

/**
 * @brief Tests the quit command
 *
 * Verifies that the engine can properly handle the quit command.
 * The quit command should cause the engine to exit without producing
 * any output, ensuring clean termination.
 */
TEST_F(UciLoopTest, QuitCommand) {
    input << "quit\n";
    uci_loop(input, output);

    // Quit command should not produce any output
    EXPECT_TRUE(output.str().empty());
}

/**
 * @brief Tests multiple commands in sequence
 *
 * Verifies that the engine can handle a sequence of different commands
 * correctly. This is important as a real UCI GUI will send multiple
 * commands in sequence during normal operation.
 */
TEST_F(UciLoopTest, MultipleCommands) {
    input << "uci\nisready\ngo\nquit\n";
    uci_loop(input, output);

    std::string response = output.str();
    EXPECT_TRUE(response.find("id name ChessEngine") != std::string::npos);
    EXPECT_TRUE(response.find("readyok") != std::string::npos);
    EXPECT_TRUE(response.find("bestmove e2e4") != std::string::npos);
}

/**
 * @brief Tests empty input handling
 *
 * Verifies that the engine properly handles empty input lines.
 * This is important for robustness as GUIs might send empty lines
 * or whitespace-only input.
 */
TEST_F(UciLoopTest, EmptyInput) {
    input << "\n";
    uci_loop(input, output);

    // Empty input should not produce any output
    EXPECT_TRUE(output.str().empty());
}

/**
 * @brief Tests unknown command handling
 *
 * Verifies that the engine gracefully handles unknown commands
 * without crashing or producing unexpected output. This is important
 * for maintaining stability when receiving unexpected input.
 */
TEST_F(UciLoopTest, UnknownCommand) {
    input << "unknown_command\n";
    uci_loop(input, output);

    // Unknown commands should not produce any output
    EXPECT_TRUE(output.str().empty());
}
