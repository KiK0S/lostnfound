
#pragma once
#include <vector>
#include <GL/gl.h>
#include "components/component.hpp"

namespace layers {

struct LayeredObject;

std::vector<LayeredObject*> layereds;

struct LayeredObject: public components::Component {
	LayeredObject(): components::Component() {
		layereds.push_back(this);
	}
	virtual ~LayeredObject() {}

	virtual int get_layer() const = 0;
};

struct ConstLayer: public LayeredObject {
	ConstLayer(int layer): LayeredObject(), layer(layer) {}
	int get_layer() const {
		return layer;
	};
	int layer;
};


}