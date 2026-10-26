#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <filesystem>
#include <windows.h>
#include "Receiver.hpp"
#include "Command.hpp"
#include "Reader.hpp"

class DllCommandWrapper : public Command {
    Command* real_command;
    void (*destroy_func)(Command*);
public:
    DllCommandWrapper(Command* cmd, void (*destroy)(Command*)) : real_command(cmd), destroy_func(destroy) {}

    ~DllCommandWrapper() override {
        if (real_command && destroy_func)
            destroy_func(real_command);
    }

    void execute() const override {
        if (real_command) {
            real_command->execute();
        }
    }
};

inline void LoadPlugin(const std::string& dllPath, Reader& reader) {
    HMODULE hDll = LoadLibraryA(dllPath.c_str());
    if (!hDll) {
        std::cout << "Error with loading Dll from " << dllPath << std::endl;
        return;
    }

    using CreateFunc = Command* (*)(Receiver*);
    using DestroyFunc = void (*)(Command*);
    using NameFunc = const char* (*)();

    CreateFunc create_command = (CreateFunc)GetProcAddress(hDll, "create_command");
    DestroyFunc destroy_command = (DestroyFunc)GetProcAddress(hDll, "destroy_command");
    NameFunc get_function_name = (NameFunc)GetProcAddress(hDll, "get_function_name");
    
    if (!create_command || !destroy_command || !get_function_name) {
         std::cout << "Error with find \"create_command\" or \"destroy_command\" or \"get_function_name\" from " << dllPath << std::endl;
        return;
    }

    std::string operationName = get_function_name();

    reader.AddOperation(operationName, [create_command, destroy_command](Receiver* r) -> std::unique_ptr<Command> {
        Command* new_command = create_command(r);
        return std::make_unique<DllCommandWrapper>(new_command, destroy_command);
    });

    std::cout << "Operation loaded successfully: " << operationName << std::endl;
}

inline void LoadPluginsFromDirectory(const std::string& directoryPath, Reader& reader) {
    if (!std::filesystem::exists(directoryPath) || !std::filesystem::is_directory(directoryPath)) {
        std::cout << "Plugins directory '" << directoryPath << "' not found." << std::endl;
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".dll") {
            LoadPlugin(entry.path().string(), reader);
        }
    }
}