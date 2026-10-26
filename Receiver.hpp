#pragma once
#include "Command.hpp"
#include <iostream>
#include <stack>
#include <memory>

class Receiver { 
private:
    std::stack<double> values;

public:
    double GetTopValue() const {
        if (values.empty())
            throw std::runtime_error("Stack is empty");
        return values.top();
    }

    void AddValue(double value) {
        values.push(value);
    }

    double PopValue() {
        if (values.empty())
            throw std::out_of_range("Error: Stack underflow (not enough operands for operation)");

        double val = values.top();
        values.pop();
        return val;
    }
};

class AddCommand : public Command {
private:
    Receiver* receiver;

public:
    AddCommand(Receiver* rec) : receiver(rec) {}

    void execute() const override {
        double b = receiver->PopValue();
        double a = receiver->PopValue();
        receiver->AddValue(a + b);
    }
};

class SubCommand : public Command {
private:
    Receiver* receiver;

public:
    SubCommand(Receiver* rec) : receiver(rec) {}

    void execute() const override {
        double b = receiver->PopValue();
        double a = receiver->PopValue();
        receiver->AddValue(a - b);
    }
};

class MulCommand : public Command {
private:
    Receiver* receiver;

public:
    MulCommand(Receiver* rec) : receiver(rec) {}

    void execute() const override {
        double b = receiver->PopValue();
        double a = receiver->PopValue();
        receiver->AddValue(a * b);
    }
};

class DivCommand : public Command {
private:
    Receiver* receiver;

public:
    DivCommand(Receiver* rec) : receiver(rec) {}

    void execute() const override {
        double b = receiver->PopValue();
        if (b == 0.) {
            throw std::runtime_error("Error: Division by zero");
        }
        else {
            double a = receiver->PopValue();
            receiver->AddValue(a / b);
        }
    }
};

class PushCommand : public Command {
private:
    Receiver* receiver;
    double value;

public:
    PushCommand(Receiver* rec, double val) : receiver(rec), value(val) {}

    void execute() const override {
        receiver->AddValue(value);
    }
};