#pragma once

#include "components/entity_system.hpp"
#include <vector>

namespace collision {

struct CollisionObject;

std::vector<CollisionObject*> colliders;

struct CollisionObject: public components::Component {
    CollisionObject(): components::Component() {
        colliders.push_back(this);
    }
    
};

}