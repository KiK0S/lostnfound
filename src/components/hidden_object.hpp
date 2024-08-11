#pragma once
#include "components/entity_system.hpp"

namespace render {
struct HiddenObject : public components::Component {
	HiddenObject(): components::Component() {}
	void hide() {
		visible = false;
	}
	void show() {
		visible = true;
	}
	bool is_visible() {
		return visible;
	}
	void switch_state() {
		if (is_visible()) {
			hide();
		} else {
			show();
		}
	}

	bool visible = false;
};
}
