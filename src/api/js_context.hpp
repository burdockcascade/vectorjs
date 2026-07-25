#pragma once
#include <quickjs.h>

namespace HostApi {
    inline JSValue create_update_context_object(JSContext* ctx);
    inline JSValue create_draw_render_object(JSContext* ctx);
}