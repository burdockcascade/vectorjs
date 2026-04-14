#pragma once
#include "qjs_wrapper.hpp"

class Runner {
public:
    // Lifecycle
    explicit Runner(std::string path);

    // Deleted Copy/Assignment
    Runner(const Runner&) = delete;
    Runner& operator=(const Runner&) = delete;

    // Execution
    void Run() const;

private:
    std::string scriptPath;
    qjs::Engine engine;

};
