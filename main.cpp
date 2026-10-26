#include "Receiver.hpp"
#include "Invoker.hpp"
#include "Reader.hpp"
#include "DllBinder.hpp"
#include <iostream>

int main() {
    try {
        Receiver receiver;
        Invoker invoker;
        Reader reader(&receiver, &invoker);
        LoadPluginsFromDirectory("./plugins", reader);

        std::cout << "Введите выражение (после каждого знака или числа - пробел, кроме скобок, \nперед \"(\" - пробел, и после \")\" - тоже, например: \"1 + 2 * (30 / 10)\"):\n";
        reader.GetLine();
        reader.FillStack();
        invoker.executeCommands();

        std::cout << receiver.GetTopValue() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}