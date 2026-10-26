#include <cmath>
#include <numbers>
#include "Command.hpp"
#include "Receiver.hpp"

class SinCommand : public Command {
    Receiver* receiver;
public:
    SinCommand(Receiver* r) : receiver(r) {}

    void execute() const override {

        double argument = receiver->PopValue();
        receiver->AddValue(std::sin(argument * std::numbers::pi / 180.));
    }
};

extern "C" __declspec(dllexport) Command* create_command(Receiver* rec) {
    return new SinCommand(rec);
}

extern "C" __declspec(dllexport) void destroy_command(Command* cmd) {
    delete cmd;
}


extern "C" __declspec(dllexport) const char* get_function_name() {
    return "s";
}