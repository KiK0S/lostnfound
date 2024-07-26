#pragma once
#include "components/component.hpp"
#include "components/init_object.hpp"

namespace dynamic {

struct DynamicObject;
struct cmp {
	bool operator()(DynamicObject* a, DynamicObject* b) const;
};

std::vector<DynamicObject*> dynamics;

struct DynamicObject : public components::Component {
    DynamicObject(int priority = 0): priority(priority), components::Component() {
        dynamics.push_back(this);
    }
    virtual ~DynamicObject() {}
    virtual void update() = 0;
    int get_priority() {
        return priority;
    }
    int priority = 0;
};


void init() {
    std::sort(dynamics.begin(), dynamics.end(), cmp());
}
init::CallbackOnStart init_dynamics(&init);

bool cmp::operator()(DynamicObject* a, DynamicObject* b) const {
	return a->get_priority() < b->get_priority();
}

}