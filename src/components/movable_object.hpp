#pragma once

#include "components/entity_system.hpp"

namespace move {

struct MovableObject: public components::Component {
    MovableObject(): components::Component() {}
    virtual ~MovableObject() {}
    virtual glm::vec2 move(glm::vec2 d) = 0;
};

}