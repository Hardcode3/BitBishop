#include <bitbishop/interface/search_controller.hpp>

void Uci::SearchController::run(std::stop_token stop_token) {
  using namespace Search;

  BestMove best;
  if (limits.infinite) {
    for (int current_depth = 1; !stop_token.stop_requested(); ++current_depth) {
      best = negamax(*pos, current_depth, ALPHA_INIT, BETA_INIT, 0, stop_token);
      if (stop_token.stop_requested()) {
        break;
      }
    }
  } else {
    best = negamax(*pos, *limits.depth, ALPHA_INIT, BETA_INIT, 0, stop_token);
  }
  if (best.move) {
    (*out) << "bestmove " << (*best.move).to_uci() << "\n";
  } else {
    (*out) << "bestmove "
           << "0000\n";
  }
  (*out) << std::flush;
}

void Uci::SearchController::start(Position &pos, SearchLimits limits, std::ostream &out_stream) {
  if (worker.joinable()) {
    worker.request_stop();
    worker.join();
  }

  if (!limits.depth) {
    limits.infinite = true;
  }

  this->pos = &pos;
  out = &out_stream;
  this->limits = limits;

  worker = std::jthread([this](std::stop_token stop_token) { run(stop_token); });
}

void Uci::SearchController::stop() { worker.request_stop(); }
