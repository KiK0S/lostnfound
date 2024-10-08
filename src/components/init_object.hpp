#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include "components/component.hpp"

namespace init {

struct UnInitializedObject;

std::vector<UnInitializedObject*> initializable;

struct cmp {
	bool operator()(UnInitializedObject* a, UnInitializedObject* b) const;
};

struct UnInitializedObject: public components::Component {
	UnInitializedObject(int priority = 0): components::Component(), priority(priority) {
		initializable.push_back(this);
	}
	virtual ~UnInitializedObject() {}
	void bind(entity::Entity*) {}
	virtual void init() = 0;
	virtual int get_priority() {
		return priority;
	}
	int priority = 0;
};


void init() {
	std::sort(initializable.begin(), initializable.end(), cmp());
	for (auto x : initializable) {
		x->init();
	}
}

struct CallbackOnStart: public UnInitializedObject {
	CallbackOnStart(std::function<void()>&& callback, int priority = 0): UnInitializedObject(priority), callback(std::move(callback)) {	}
	virtual ~CallbackOnStart() {}
	void init() {
		callback();
	}
	std::function<void()> callback;
};



bool cmp::operator()(UnInitializedObject* a, UnInitializedObject* b) const {
	return a->get_priority() < b->get_priority();
}

}