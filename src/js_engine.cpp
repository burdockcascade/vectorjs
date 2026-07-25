#include "js_engine.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace VectorJS {
    void JSEngine::check_and_throw_exception(JSValue value) const {
        if (!JS_IsException(value)) {
            return;
        }

        const JSValue exception_val = JS_GetException(ctx);
        std::string error_msg = "JavaScript Exception";

        const char* msg = JS_ToCString(ctx, exception_val);
        if (msg) {
            error_msg = msg;
            JS_FreeCString(ctx, msg);
        }

        // Attempt to extract stack trace
        JSValue stack_val = JS_GetPropertyStr(ctx, exception_val, "stack");
        if (!JS_IsUndefined(stack_val)) {
            const char* stack_str = JS_ToCString(ctx, stack_val);
            if (stack_str) {
                error_msg += "\nStack trace:\n";
                error_msg += stack_str;
                JS_FreeCString(ctx, stack_str);
            }
        }

        JS_FreeValue(ctx, stack_val);
        JS_FreeValue(ctx, exception_val);
        JS_FreeValue(ctx, value);

        throw std::runtime_error(error_msg);
    }

    std::string JSEngine::read_file_contents(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + filepath);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    JSEngine::JSEngine() {
        rt = JS_NewRuntime();
        if (!rt) throw std::runtime_error("Failed to create JSRuntime");

        ctx = JS_NewContext(rt);
        if (!ctx) {
            JS_FreeRuntime(rt);
            throw std::runtime_error("Failed to create JSContext");
        }
    }

    JSEngine::~JSEngine() {
        if (ctx) JS_FreeContext(ctx);
        if (rt) JS_FreeRuntime(rt);
    }

    JSEngine::JSEngine(JSEngine&& other) noexcept : rt(other.rt), ctx(other.ctx) {
        other.rt = nullptr;
        other.ctx = nullptr;
    }

    JSEngine& JSEngine::operator=(JSEngine&& other) noexcept {
        if (this != &other) {
            if (ctx) JS_FreeContext(ctx);
            if (rt) JS_FreeRuntime(rt);

            rt = other.rt;
            ctx = other.ctx;
            other.rt = nullptr;
            other.ctx = nullptr;
        }
        return *this;
    }

    void JSEngine::eval(std::string_view code, std::string_view filename) const {
        JSValue result = JS_Eval(ctx, code.data(), code.length(), filename.data(), JS_EVAL_TYPE_MODULE);
        check_and_throw_exception(result);
        JS_FreeValue(ctx, result);
    }

    JSValue JSEngine::eval_value(std::string_view code, std::string_view filename) const {
        JSValue result = JS_Eval(ctx, code.data(), code.length(), filename.data(), JS_EVAL_TYPE_MODULE);
        check_and_throw_exception(result);
        return result;
    }

    void JSEngine::eval_file(const std::string& filepath, int eval_flags) const {
        std::string code = read_file_contents(filepath);
        JSValue result = JS_Eval(ctx, code.c_str(), code.length(), filepath.c_str(), eval_flags);

        // Unified exception handling with stack trace support
        check_and_throw_exception(result);
        JS_FreeValue(ctx, result);
    }
}