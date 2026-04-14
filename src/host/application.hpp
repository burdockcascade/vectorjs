#pragma once
#include "qjs_wrapper.hpp"

class Application {
public:
    // Lifecycle
    explicit Application(std::string path);

    // Deleted Copy/Assignment
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    // Execution
    void Run() const;

private:
    std::string scriptPath;
    qjs::Engine engine;

};
