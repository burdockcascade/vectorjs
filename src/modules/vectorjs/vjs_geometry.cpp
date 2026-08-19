#include <optional>
#include <memory>
#include <raylib.h>
#include <qjspp.hpp>

#include "vjs_application.hpp"
#include "vjs_geometry.hpp"

namespace App::Modules {

    void register_rectangle(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto rectangle = engine.make_class<JSRectangle>("Rectangle");

        rectangle.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSRectangle> {
            if (args.size() < 4) return nullptr;
            return std::make_unique<JSRectangle>(
                static_cast<float>(args[0].to_double()),
                static_cast<float>(args[1].to_double()),
                static_cast<float>(args[2].to_double()),
                static_cast<float>(args[3].to_double())
            );
        });

        rectangle.property("x",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->x); },
            [](JSRectangle* self, const qjspp::Value& val) { self->x = static_cast<float>(val.to_double()); }
        );

        rectangle.property("y",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->y); },
            [](JSRectangle* self, const qjspp::Value& val) { self->y = static_cast<float>(val.to_double()); }
        );

        rectangle.property("width",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->width); },
            [](JSRectangle* self, const qjspp::Value& val) { self->width = static_cast<float>(val.to_double()); }
        );

        rectangle.property("height",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->height); },
            [](JSRectangle* self, const qjspp::Value& val) { self->height = static_cast<float>(val.to_double()); }
        );

        rectangle.instance_method("contains", [&engine](JSRectangle* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);
            auto* point = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!point) return engine.make_bool(false);
            return engine.make_bool(CheckCollisionPointRec(*point, *self));
        });

        rectangle.instance_method("overlaps", [&engine](JSRectangle* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);
            auto* rec2 = qjspp::get_native_opaque<JSRectangle>(args[0]);
            if (!rec2) return engine.make_bool(false);
            return engine.make_bool(CheckCollisionRecs(*self, *rec2));
        });

        rectangle.instance_method("getCollisionRect", [&engine](JSRectangle* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            auto* rec2 = qjspp::get_native_opaque<JSRectangle>(args[0]);
            if (!rec2) return {};
            auto result = GetCollisionRec(*self, *rec2);
            return engine.make_native_object(std::make_unique<JSRectangle>(result));
        });

        builder.export_class("Rectangle", rectangle.build());
    }

    void register_circle(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto circle = engine.make_class<JSCircle>("Circle");

        // Constructor: new Circle(x, y, radius)
        circle.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSCircle> {
            if (args.size() < 3) return nullptr;
            return std::make_unique<JSCircle>(JSCircle{
                Vector2{ static_cast<float>(args[0].to_double()), static_cast<float>(args[1].to_double()) },
                static_cast<float>(args[2].to_double())
            });
        });

        // Properties: x, y, radius
        circle.property("x",
            [](JSContext* ctx, JSCircle* self) { return qjspp::Value::make_double(ctx, self->center.x); },
            [](JSCircle* self, const qjspp::Value& val) { self->center.x = static_cast<float>(val.to_double()); }
        );

        circle.property("y",
            [](JSContext* ctx, JSCircle* self) { return qjspp::Value::make_double(ctx, self->center.y); },
            [](JSCircle* self, const qjspp::Value& val) { self->center.y = static_cast<float>(val.to_double()); }
        );

        circle.property("radius",
            [](JSContext* ctx, JSCircle* self) { return qjspp::Value::make_double(ctx, self->radius); },
            [](JSCircle* self, const qjspp::Value& val) { self->radius = static_cast<float>(val.to_double()); }
        );

        // Collision: Check if a Vector2 point is inside this circle
        circle.instance_method("contains", [&engine](JSCircle* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);
            auto* point = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!point) return engine.make_bool(false);
            return engine.make_bool(CheckCollisionPointCircle(*point, self->center, self->radius));
        });

        // Collision: Check overlap with another Circle or a Rectangle
        circle.instance_method("overlaps", [&engine](JSCircle* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);

            if (auto* otherCircle = qjspp::get_native_opaque<JSCircle>(args[0])) {
                return engine.make_bool(CheckCollisionCircles(self->center, self->radius, otherCircle->center, otherCircle->radius));
            }

            if (auto* rec = qjspp::get_native_opaque<JSRectangle>(args[0])) {
                return engine.make_bool(CheckCollisionCircleRec(self->center, self->radius, *rec));
            }

            return engine.make_bool(false);
        });

        builder.export_class("Circle", circle.build());
    }

    void register_triangle(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto triangle = engine.make_class<JSTriangle>("Triangle");

        // Constructor: new Triangle(v1, v2, v3) expecting Vector2 objects
        triangle.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSTriangle> {
            if (args.size() < 3) return nullptr;
            auto* p1 = qjspp::get_native_opaque<JSVector2>(args[0]);
            auto* p2 = qjspp::get_native_opaque<JSVector2>(args[1]);
            auto* p3 = qjspp::get_native_opaque<JSVector2>(args[2]);
            if (!p1 || !p2 || !p3) return nullptr;
            return std::make_unique<JSTriangle>(JSTriangle{ *p1, *p2, *p3 });
        });

        // Properties: v1, v2, v3
        triangle.property("v1",
            [&engine](JSContext* ctx, JSTriangle* self) {
                return engine.make_native_object(std::make_unique<JSVector2>(self->v1));
            },
            [](JSTriangle* self, const qjspp::Value& val) {
                if (auto* v = qjspp::get_native_opaque<JSVector2>(val)) self->v1 = *v;
            }
        );

        triangle.property("v2",
            [&engine](JSContext* ctx, JSTriangle* self) {
                return engine.make_native_object(std::make_unique<JSVector2>(self->v2));
            },
            [](JSTriangle* self, const qjspp::Value& val) {
                if (auto* v = qjspp::get_native_opaque<JSVector2>(val)) self->v2 = *v;
            }
        );

        triangle.property("v3",
            [&engine](JSContext* ctx, JSTriangle* self) {
                return engine.make_native_object(std::make_unique<JSVector2>(self->v3));
            },
            [](JSTriangle* self, const qjspp::Value& val) {
                if (auto* v = qjspp::get_native_opaque<JSVector2>(val)) self->v3 = *v;
            }
        );

        // Collision: Check if a Vector2 point is inside this triangle
        triangle.instance_method("contains", [&engine](JSTriangle* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);
            auto* point = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!point) return engine.make_bool(false);
            return engine.make_bool(CheckCollisionPointTriangle(*point, self->v1, self->v2, self->v3));
        });

        builder.export_class("Triangle", triangle.build());
    }

    void register_point(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto point = engine.make_class<JSPoint>("Point");

        // Constructor: new Point(x, y)
        point.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSPoint> {
            if (args.size() < 2) return nullptr;
            return std::make_unique<JSPoint>(JSPoint{
                Vector2{ static_cast<float>(args[0].to_double()), static_cast<float>(args[1].to_double()) }
            });
        });

        // Properties: x, y
        point.property("x",
            [](JSContext* ctx, JSPoint* self) { return qjspp::Value::make_double(ctx, self->position.x); },
            [](JSPoint* self, const qjspp::Value& val) { self->position.x = static_cast<float>(val.to_double()); }
        );

        point.property("y",
            [](JSContext* ctx, JSPoint* self) { return qjspp::Value::make_double(ctx, self->position.y); },
            [](JSPoint* self, const qjspp::Value& val) { self->position.y = static_cast<float>(val.to_double()); }
        );

        // Collision: Check if this point lies inside Rectangle, Circle, or Triangle
        point.instance_method("intersects", [&engine](JSPoint* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);

            if (auto* rec = qjspp::get_native_opaque<JSRectangle>(args[0])) {
                return engine.make_bool(CheckCollisionPointRec(self->position, *rec));
            }

            if (auto* circle = qjspp::get_native_opaque<JSCircle>(args[0])) {
                return engine.make_bool(CheckCollisionPointCircle(self->position, circle->center, circle->radius));
            }

            if (auto* tri = qjspp::get_native_opaque<JSTriangle>(args[0])) {
                return engine.make_bool(CheckCollisionPointTriangle(self->position, tri->v1, tri->v2, tri->v3));
            }

            return engine.make_bool(false);
        });

        builder.export_class("Point", point.build());
    }

    void register_line(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto line = engine.make_class<JSLine>("Line");

        // Constructor: new Line(p1, p2) expecting Vector2 objects
        line.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSLine> {
            if (args.size() < 2) return nullptr;
            auto* p1 = qjspp::get_native_opaque<JSVector2>(args[0]);
            auto* p2 = qjspp::get_native_opaque<JSVector2>(args[1]);
            if (!p1 || !p2) return nullptr;
            return std::make_unique<JSLine>(JSLine{ *p1, *p2 });
        });

        // Properties: start, end
        line.property("start",
            [&engine](JSContext* ctx, JSLine* self) {
                return engine.make_native_object(std::make_unique<JSVector2>(self->start));
            },
            [](JSLine* self, const qjspp::Value& val) {
                if (auto* v = qjspp::get_native_opaque<JSVector2>(val)) self->start = *v;
            }
        );

        line.property("end",
            [&engine](JSContext* ctx, JSLine* self) {
                return engine.make_native_object(std::make_unique<JSVector2>(self->end));
            },
            [](JSLine* self, const qjspp::Value& val) {
                if (auto* v = qjspp::get_native_opaque<JSVector2>(val)) self->end = *v;
            }
        );

        // Collision: Check intersection with another Line segment
        line.instance_method("intersects", [&engine](JSLine* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);

            auto* otherLine = qjspp::get_native_opaque<JSLine>(args[0]);
            if (!otherLine) return engine.make_bool(false);

            Vector2 collisionPoint = { 0 };
            bool hasCollision = CheckCollisionLines(
                self->start, self->end,
                otherLine->start, otherLine->end,
                &collisionPoint
            );

            return engine.make_bool(hasCollision);
        });

        // Get collision intersection point with another Line (returns Vector2 or null)
        line.instance_method("getIntersection", [&engine](JSLine* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};

            auto* otherLine = qjspp::get_native_opaque<JSLine>(args[0]);
            if (!otherLine) return {};

            Vector2 collisionPoint = { 0 };
            if (CheckCollisionLines(self->start, self->end, otherLine->start, otherLine->end, &collisionPoint)) {
                return engine.make_native_object(std::make_unique<JSVector2>(collisionPoint));
            }

            return {};
        });

        builder.export_class("Line", line.build());
    }

    void register_polygon(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto polygon = engine.make_class<JSPolygon>("Polygon");

        polygon.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSPolygon> {

            if (args.empty()) return nullptr;

            std::vector<Vector2> pts;
            if (args[0].is_array()) {
                for (const auto& elem : args[0].to_vector()) {
                    if (auto* v = qjspp::get_native_opaque<JSVector2>(elem)) {
                        pts.push_back(*v);
                    }
                }
            }

            if (pts.size() < 3) return nullptr;
            return std::make_unique<JSPolygon>(JSPolygon{ std::move(pts) });
        });

        // Property: points count
        polygon.property("vertexCount",
            [](JSContext* ctx, JSPolygon* self) {
                return qjspp::Value::make_int(ctx, static_cast<int32_t>(self->points.size()));
            },
            nullptr
        );

        // Method: getPoint(index) -> Vector2
        polygon.instance_method("getPoint", [&engine](JSPolygon* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            int index = args[0].to_int();
            if (index < 0 || index >= static_cast<int>(self->points.size())) return {};
            return engine.make_native_object(std::make_unique<JSVector2>(self->points[index]));
        });

        // Collision: Point vs Polygon check using Raylib's native function
        polygon.instance_method("contains", [&engine](JSPolygon* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);

            auto* point = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!point) return engine.make_bool(false);

            // Raylib's CheckCollisionPointPoly takes (Vector2 point, const Vector2 *points, int pointCount)
            bool collision = CheckCollisionPointPoly(*point, self->points.data(), static_cast<int>(self->points.size()));
            return engine.make_bool(collision);
        });

        builder.export_class("Polygon", polygon.build());
    }

}