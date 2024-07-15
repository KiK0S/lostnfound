#pragma once
#include <vector>

namespace dynamic {

struct DynamicObject;

std::vector<DynamicObject*> dynamics;

struct DynamicObject {
    DynamicObject() {
        dynamics.push_back(this);
    }
    virtual ~DynamicObject() {}
    virtual void update() = 0;
};


}