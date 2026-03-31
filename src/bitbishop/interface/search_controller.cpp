#include <bitbishop/interface/search_controller.hpp>

Uci::SearchController::SearchController(Board board, SearchLimits limits, std::ostream& ostream)
    : board(board), position(Position(this->board)), limits(limits), out(&ostream) {}

Uci::SearchController::~SearchController() { stop(); }

void Uci::SearchController::run() {
  using namespace Search;

  BestMove best;
  BestMove last_best;
  try {
    if (limits.infinite) {
      for (int current_depth = 1; !stop_flag.load(); ++current_depth) {
        best = negamax(position, current_depth, ALPHA_INIT, BETA_INIT, 0, &stop_flag);
        if (!stop_flag.load()) {
          last_best = best;
        }
      }
    } else if (limits.depth) {
      best = negamax(position, *limits.depth, ALPHA_INIT, BETA_INIT, 0, &stop_flag);
    }
  } catch (const std::exception& e) {
    (*out) << "info string exception " << e.what() << "\n";
  } catch (...) {
    (*out) << "info string unknown exception\n";
  }

  const BestMove& final = (last_best.move) ? last_best : best;
  const std::string best_move_str = (final.move) ? (*final.move).to_uci() : "0000";
  (*out) << "bestmove " << best_move_str << "\n" << std::flush;
}

void Uci::SearchController::start() {
  stop();
  stop_flag.store(false);

  if (!limits.depth) {
    limits.infinite = true;
  }

  worker = std::thread(&SearchController::run, this);
}

void Uci::SearchController::wait() {
  if (worker.joinable()) {
    worker.join();
  }
}

void Uci::SearchController::stop() {
  stop_flag.store(true);
  wait();
}
