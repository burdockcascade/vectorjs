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
    void eval_script(std::string scriptPath) const;

    void eval_bytecode(const std::string &bytecodePath);

    void compile_file_to_file(const std::string &compileInput, const std::string &output_file) const;

private:
    qjs::Engine engine;

};
