#pragma once
#include "qjs_wrapper.hpp"

class Application {
public:
    // Lifecycle
    explicit Application();

    // Deleted Copy/Assignment
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    // Execution
    void Run(std::string scriptPath) const;

private:
    qjs::Engine engine;

};
