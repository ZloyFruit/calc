#pragma once
#include "Command.hpp"
#include <memory>
#include <queue>

class Invoker {
    std::unique_ptr<Command> _on_start;
    std::unique_ptr<Command> _on_finish;
    std::queue<std::unique_ptr<Command>> Commands;
public:
    void SetOnStart(std::unique_ptr<Command> &&command) {
        _on_start = std::move(command);
    }

    void SetOnFinish(std::unique_ptr<Command> &&command) {
        _on_finish = std::move(command);
    }

    void AddInQueue(std::unique_ptr<Command> &&command) {
        Commands.push(move(command));
    }

    void executeCommands() {
        if (_on_start) {
            _on_start->execute();
        }

        while (!Commands.empty()) {
            auto command = std::move(Commands.front());
            Commands.pop();
            command->execute();
        }

        if (_on_finish) {
            _on_finish->execute();
        }
    }
};