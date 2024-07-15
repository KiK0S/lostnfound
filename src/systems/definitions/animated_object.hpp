#pragma once
#include <vector>
#include <string>

namespace animation {

struct AnimatedObject;
std::vector<AnimatedObject*> animateds;

struct AnimatedObject {
	AnimatedObject() {
		animateds.push_back(this);
	}
	virtual ~AnimatedObject() {}
	virtual void update(float dt) = 0;
	virtual void set_state(std::string state) = 0; 
	virtual std::string get_state() = 0; 
};

}