#pragma once
#include "components/transform_object.hpp"
#include "components/collision_object.hpp"
#include "components/movable_object.hpp"

namespace move {

struct MovableWithCollisions: public MovableObject {
    MovableWithCollisions(): MovableObject() {}
    ~MovableWithCollisions() {}
    glm::vec2 move(glm::vec2 d) {
        if (glm::length(d) == 0.0f) return d;
        auto pos = get_entity()->get<transform::TransformObject>()->get_pos();
        for (auto collider : collision::colliders) {
            auto geom = collider->get_entity()->get<geometry::GeometryObject>();
            auto model_matrix = glm::transpose(collider->get_entity()->get<transform::TransformObject>()->get_model_matrix());
            auto points = geom->get_pos();
            for (int i = 0; i < points.size(); i += 3) {
                auto a = model_matrix * glm::vec4(points[i], 0.0f, 1.0f);
                auto b = model_matrix * glm::vec4(points[i + 1], 0.0f, 1.0f);
                auto c = model_matrix * glm::vec4(points[i + 2], 0.0f, 1.0f);
                if (intersect(pos, pos + d, a, b) || intersect(pos, pos + d, c, a) || intersect(pos, pos + d, b, c)) {
                    return glm::vec2{0.0f, 0.0f};
                }
            }
        }
        return d;
    }

    bool diff_side(glm::vec2 o, glm::vec2 a, glm::vec2 b) {
        bool a_pos = a.x * o.y - a.y * o.x > 0;
        bool b_pos = b.x * o.y - b.y * o.x > 0;
        return a_pos != b_pos;
    }
    bool intersect(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d) {
        return diff_side(b - a, c - a, d - a) && diff_side(d - c, a - c, b - c);      
    }
};

}