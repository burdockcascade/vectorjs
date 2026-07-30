#pragma once
#include <quickjs.h>

namespace HostApi {
    JSValue create_update_context_object(JSContext* ctx);
    JSValue create_draw_render_object(JSContext* ctx);
}