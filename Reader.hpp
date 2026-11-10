#pragma once
#include "Receiver.hpp"
#include "Invoker.hpp"
#include <string>
#include <stack>
#include <functional>
#include <memory>
#include <unordered_map>

using CommandCreator = std::function<std::unique_ptr<Command>(Receiver*)>;

class Reader {
private:
    Receiver* receiver;
    Invoker* invoker;
    std::string expression;
    std::stack<char> OperatorStack;
    std::unordered_map<std::string, CommandCreator> operations;

    void ProcessBinaryOperation(char op) {
        std::string opstr(1, op);
        auto it = operations.find(opstr);
        if (it != operations.end()) {
            auto command = it->second(receiver);
            invoker->AddInQueue(std::move(command));
        }
    }

    void ProcessNumber(size_t& point, std::string& val) {
        char curr = expression[point];
        val += curr;
        point++;
        
        if (point < expression.length()) {
            while (point < expression.length() && 
                   expression[point] != ' ' && 
                   expression[point] != ')') {
                val += expression[point];
                point++;
            }
        }
        
        invoker->AddInQueue(std::make_unique<PushCommand>(receiver, std::stod(val)));
        val.clear();
    }

    void ProcessClosingBracket() {
        while (!OperatorStack.empty() && OperatorStack.top() != '(') {
            char op = OperatorStack.top();
            OperatorStack.pop();
            ProcessBinaryOperation(op);
        }
        if (!OperatorStack.empty()) {
            OperatorStack.pop();
        }
    }

    void ProcessOperator(char curr) {
        std::string currstr(1, curr);
        if (operations.count(currstr)) {
            while (!OperatorStack.empty() && 
                   Priority(OperatorStack.top()) >= Priority(curr)) {
                char op = OperatorStack.top();
                OperatorStack.pop();
                ProcessBinaryOperation(op);
            }
            OperatorStack.push(curr);
        }
    }

    void ClearOperatorStack() {
        while (!OperatorStack.empty()) {
            char op = OperatorStack.top();
            OperatorStack.pop();
            if (op != '(') {
                ProcessBinaryOperation(op);
            }
        }
    }

public:
    Reader(Receiver* reciever, Invoker* invoker) : receiver(reciever), invoker(invoker) {
        operations["+"] = [](Receiver* r) { return std::make_unique<AddCommand>(r); };
        operations["-"] = [](Receiver* r) { return std::make_unique<SubCommand>(r); };
        operations["*"] = [](Receiver* r) { return std::make_unique<MulCommand>(r); };
        operations["/"] = [](Receiver* r) { return std::make_unique<DivCommand>(r); };
    }

    void GetLine() {
        std::getline(std::cin, expression);
    }

    int Priority(char op) {
        if (op == '+' || op == '-') 
            return 1;
        if (op == '*' || op == '/') 
            return 2;
        if (op == '^')
            return 3;
        if (op == 's')
            return 4;
        return 0;
    }

    void AddOperation(const std::string& name, const CommandCreator& creator) {
        operations[name] = creator;
    }

    void FillStack() {
        size_t point = 0;
        std::string val;
        char curr;
        while (point < expression.length()) {
            curr = expression[point++];
            std::string currstr(1, curr);
            if (operations.count(currstr)) {
                ProcessOperator(curr);
                continue;
            }

            switch (curr) {
                case ' ':
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                    ProcessNumber(point, val);
                    break;
                case '(':
                    OperatorStack.push(curr);
                    break;
                case ')':
                    ProcessClosingBracket();
                    break;
                default:
                    break;
            }
        }

        ClearOperatorStack();
    }
};