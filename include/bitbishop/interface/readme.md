# About the `interface/` directory

## Purpose

`interface/` is the **boundary between BitBishop and the outside world**.

In the current codebase this directory is **primarily the UCI layer**.

It is responsible for:

- **Parsing external commands** and inputs
- **Translating protocol concepts into engine calls**
- **Managing search sessions**, clocks, and stop requests
- **Emiting protocol-compliant responses**

## Place in the architecture

```mermaid
flowchart TD

    OutsideWorld("`
        Outside world
        CLI, GUI, Tests...
    `")
    Other("`...`")
    Engine("`
        **Search Engine**
        Evaluation and search
        -
        inlcude/bitbishop/engine/*.hpp
    `")
    Interface("`
        **Interface**
        UCI protocol and search orchestration
        -
        inlcude/bitbishop/interface/*.hpp
    `")

    OutsideWorld --> Interface

    Interface --> Engine

    Engine --> Other
```

### Thread Lifecycle (runtime)

Interface defines threading concepts in order to work with the UCI protocol.

There are currently three threads:

- The **main thread (control thread)**: parsing commands, polling search reports, and writing protocol output.
- The **listener thread** (`UciCommandChannel`) reads incoming command lines from the input stream.
- The **worker thread** (`SearchWorker`) handles best move search and currently works alone.

```mermaid
sequenceDiagram
    autonumber
    participant IO as GUI/CLI<br/>stdin/stdout
    participant In as Input Stream<br/>std::cin (default)
    participant Main as Main Thread<br/>UciEngine::loop()
    participant Cmd as Command Thread<br/>UciCommandChannel::reader_loop()
    participant Ch as Channel State<br/>pending_lines + lines_cv
    participant Session as SearchSession<br/>(main thread)
    participant Worker as Search Thread<br/>SearchWorker::run()

    Main->>Cmd: command_channel.start()
    loop Reader lifecycle
        Cmd->>In: std::getline(input_stream, line)
        Note right of Cmd: Blocking read (default runtime: std::cin).<br/>Sleeps until newline or EOF.
        alt Line read
            Cmd->>Ch: push line + notify_one()
        else EOF reached
            Cmd->>Ch: eof_reached = true + notify_all()
        end
    end

    loop Main loop tick (while is_running)
        Main->>Session: poll()
        Main->>Ch: wait_and_pop_line(timeout = 5ms)
        Note right of Main: Blocks on condition_variable::wait_for<br/>when queue is empty.
        alt Command line available
            Main->>Main: split(raw_line)
            Main->>Main: command_registry.dispatch(tokens)
            alt go / bench
                Main->>Session: start_go(...) or start_bench(...)
                Session->>Worker: start()
                loop Search execution
                    Worker-->>Session: push_report(Iteration/Finish)
                    Main->>Session: poll()
                    Session->>Session: emit_reports()
                    Session-->>IO: reporter output (bestmove / bench)
                end
            else stop
                Main->>Session: request_stop()
                Session->>Worker: stop_flag = true
            else quit
                Main->>Session: request_stop()
                Session->>Worker: stop_flag = true
                Main->>Main: is_running = false
                Note right of Main: Loop guard will fail on next check.<br/>Final cleanup runs after loop.
            end
        else No command within timeout
            Main->>Main: wake up and continue loop
        end
        alt Input reached EOF
            Main->>Session: request_stop()
            Main->>Session: poll() until idle
            Main->>Session: stop_and_join()
            Main->>Cmd: command_channel.stop()
            Main-->>IO: loop exits
        end
    end
```

#### Blocking points (intentional idle time)

BitBishop does not run every thread at 100% all the time. Some waits are expected and intentional.

| Thread | Blocking call | Blocks when | Wakes up when |
| --- | --- | --- | --- |
| Command thread (`UciCommandChannel`) | `std::getline(input_stream, line)` | No full line is available (default runtime stream is `std::cin`) | Newline arrives or EOF is reached |
| Main thread (`UciEngine::loop`) | `wait_and_pop_line(..., 5ms)` (`condition_variable::wait_for`) | Pending line queue is empty and EOF not reached | A line is pushed (`notify_one`), EOF is signaled (`notify_all`), or timeout elapses |
| Worker thread (`SearchWorker`) | No intentional sleep in `run()` | It is actively searching (CPU-bound) | Search limit reached or `stop_flag` set |

#### `stop` vs `quit` semantics

| Command | Effect on search | Effect on process |
| --- | --- | --- |
| `stop` | Requests search interruption (`stop_flag = true`) and keeps the UCI loop alive | Engine keeps running and can accept new commands |
| `quit` | Requests search interruption (`stop_flag = true`) | Sets `is_running = false`, then exits loop and performs final cleanup (`stop_and_join()`, `command_channel.stop()`) |

> Note: the runtime default input is `std::cin`, but `UciEngine` accepts any `std::istream`. With pre-buffered streams (for example `std::istringstream` in tests), `std::getline` may return immediately.

> Shutdown detail: `UciCommandChannel::stop()` cannot forcibly interrupt a blocking `std::getline`, so the reader thread is detached when EOF is not yet reached.

#### Thread state timeline (runtime)

This complementary view focuses only on thread states, not message payloads.

```mermaid
stateDiagram-v2
    direction LR

    state "Command thread (UciCommandChannel)" as CmdThread {
        [*] --> BlockedOnInput: start()
        BlockedOnInput --> RunningPush: line available
        RunningPush --> BlockedOnInput: push + notify_one
        BlockedOnInput --> EofReached: EOF
        EofReached --> [*]
    }

    state "Main thread (UciEngine::loop)" as MainThread {
        [*] --> RunningControl: while (is_running) start tick
        RunningControl --> BlockedOnCV: wait_and_pop_line(5ms)
        BlockedOnCV --> RunningControl: line/EOF/timeout wakeup
        RunningControl --> RunningControl: dispatch + poll + output
        RunningControl --> [*]: !is_running (quit OR EOF path)
    }

    state "Worker thread (SearchWorker)" as WorkerThread {
        [*] --> Idle: no active search
        Idle --> RunningSearch: start_go/start_bench
        RunningSearch --> RunningSearch: search iterations
        RunningSearch --> Idle: limit reached or stop_flag
    }
```

### Class Relationships (Structure)

```mermaid
classDiagram
    direction LR

    class UciEngine {
      -Board board
      -Position position
      -UciCommandChannel command_channel
      -SearchSession search_session
      -UciCommandRegistry command_registry
      -bool is_running
      +loop()
      -dispatch(line)
      -handle_go(line)
      -handle_bench(line)
      -handle_stop()
      -handle_quit()
    }

    class UciCommandChannel {
      -input_stream
      -reader_thread
      -state
      +start()
      +stop()
      +wait_and_pop_line(line, timeout)
      +eof()
    }

    class UciCommandRegistry {
      -handlers
      +register_handler(command, handler)
      +dispatch(line)
    }

    class SearchSession {
      -out_stream
      -worker
      -reporter
      +start_go(board, limits)
      +start_bench(board, limits)
      +request_stop()
      +poll()
      +stop_and_join()
    }

    class SearchWorker {
      -worker
      -stop_flag
      -finished
      -reports
      +start()
      +request_stop()
      +drain_reports()
      +stop()
    }

    class SearchReporter {
      <<interface>>
      +on_iteration(best, depth, stats)
      +on_finish(best, stats)
    }

    class UciReporter {
      +on_finish(best, stats)
    }

    class BenchReporter {
      +now()
      +on_finish(best, stats)
    }

    class SearchLimits
    class SearchReport

    UciEngine *-- UciCommandChannel
    UciEngine *-- UciCommandRegistry
    UciEngine *-- SearchSession

    SearchSession *-- SearchWorker : active search
    SearchSession *-- SearchReporter : active reporter

    SearchReporter <|.. UciReporter
    SearchReporter <|.. BenchReporter

    SearchWorker ..> SearchLimits
    SearchWorker ..> SearchReport
    SearchSession ..> UciReporter : for go
    SearchSession ..> BenchReporter : for bench
```
