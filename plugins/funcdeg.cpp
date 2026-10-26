#include <cmath>
#include "Command.hpp"
#include "Receiver.hpp"

class PowCommand : public Command {
    Receiver* receiver;
public:
    PowCommand(Receiver* r) : receiver(r) {}

    void execute() const override {

        double exponent = receiver->PopValue();
        double base = receiver->PopValue();
        
        receiver->AddValue(std::pow(base, exponent));
    }
};

extern "C" __declspec(dllexport) Command* create_command(Receiver* rec) {
    return new PowCommand(rec);
}

extern "C" __declspec(dllexport) void destroy_command(Command* cmd) {
    delete cmd;
}


extern "C" __declspec(dllexport) const char* get_function_name() {
    return "^";
}