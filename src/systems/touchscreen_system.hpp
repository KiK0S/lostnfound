#pragma once
#include <SDL2/SDL.h>
#include "components/dynamic_object.hpp"
#include "components/entity_system.hpp"
#include "systems/easy_drawable_system.hpp"
#include "components/gpu_program.hpp"
#include "systems/geometry_system.hpp"
#include "systems/input_system.hpp"
#include "systems/minimap_system.hpp"
#include "components/touch_object.hpp"
#include "components/text_object.hpp"
#include "utils/arena.hpp"

#include <ctime>

namespace touchscreen {


std::unique_ptr<entity::Entity> joystick;
std::unique_ptr<entity::Entity> joystick_inner;
minimap::MiniMapUniforms joystick_uniform;



struct JoystickUpdate: public TouchObject {
	JoystickUpdate(): TouchObject() {}

	glm::vec2 joystick_info{0.0f, 0.0f};

	void update_joystick() {
		auto joystick_transform = joystick_inner->get<transform::TransformObject>();
		joystick_transform->translate(-joystick_transform->get_pos() + glm::vec2(-0.6f, 0.6f) + joystick_info * 0.1f);
	}

	void handle_touch(glm::vec2 point) {
		double dx = point.x + 0.6;
		double dy = point.y - 0.6;
		if (abs(dx) > 0.5 || abs(dy) > 0.5) {
			joystick_info = {0.0f, 0.0f};
			update_joystick();
			return;
		}
		dx /= 0.25;
		dy /= 0.25;
		joystick_info = {dx, dy};
		if (dx < -1) joystick_info /= -dx;
		if (dx > 1) joystick_info /= dx;
		if (dy < -1) joystick_info /= -dy;
		if (dy > 1) joystick_info /= dy;
		update_joystick();
		joystick_info.y *= -1;
	}
};
JoystickUpdate joystick_update;

void init_joystick() {
	joystick = std::make_unique<entity::Entity>();
	auto joystick_drawable = arena::create<render::SolidDrawable>(&geometry::circle, &shaders::static_object_program);
	joystick_drawable->get_transform()->scale(glm::vec2(0.25f, 0.25f));
	joystick_drawable->get_transform()->translate(glm::vec2(-0.6f, 0.6f));
	joystick_drawable->color = &color::lighter_grey;
	joystick_drawable->uniforms.add(&joystick_uniform);
	auto joystick_scene = arena::create<scene::SceneObject>("main");
	joystick->add(joystick_drawable);
	joystick->add(joystick_scene);
	joystick->bind();

	joystick_inner = std::make_unique<entity::Entity>();
	auto joystick_inner_drawable = arena::create<render::SolidDrawable>(&geometry::circle, &shaders::static_object_program);
	joystick_inner_drawable->get_transform()->scale(glm::vec2(0.2f, 0.2f));
	joystick_inner_drawable->get_transform()->translate(glm::vec2(-0.6f, 0.6f));
	joystick_inner_drawable->color = &color::light_grey;
	joystick_inner_drawable->uniforms.add(&joystick_uniform);
	auto joystick_inner_scene = arena::create<scene::SceneObject>("main");
	joystick_inner->add(joystick_inner_drawable);
	joystick_inner->add(joystick_inner_scene);
	joystick_inner->bind();
}


struct TouchKeyboardButton: public TouchObject {
	TouchKeyboardButton(SDL_Scancode key, glm::vec2 top_left, glm::vec2 bottom_right) : TouchObject(), key(key), top_left(top_left), bottom_right(bottom_right) {}
	void handle_touch(glm::vec2 pos) {
		if (pos.x < top_left.x || pos.x > bottom_right.x || pos.y < top_left.y || pos.y > bottom_right.y) {
			last_pushed = std::chrono::time_point<std::chrono::system_clock>::min();
			return;
		}
		std::chrono::time_point<std::chrono::system_clock> cur_time = std::chrono::system_clock::now();
		
		input::is_pressed[key] = true;
		if ((cur_time - last_pushed).count() > 10000) {
			SDL_Event event;
			event.key.keysym.scancode = key;
			input::input.fire_event(event);
		}
		last_pushed = cur_time;
	}
	std::chrono::time_point<std::chrono::system_clock> last_pushed = std::chrono::time_point<std::chrono::system_clock>::min();
	SDL_Scancode key;
	glm::vec2 top_left;
	glm::vec2 bottom_right;
};

entity::Entity map_button;

struct TouchSystem: public dynamic::DynamicObject {
	TouchSystem(): dynamic::DynamicObject(-1) {}

	bool inited = false;

	void init() {
		touchscreen::init_joystick();
		inited = true;
	}

	void update() {
		if (!inited /*&& SDL_GetNumTouchDevices() > 0*/) init();
		if (!inited) return;
		glm::vec2 point = input::get_touch() * 2.0f - glm::vec2(1.0f, 1.0f);
		if (point.x == -1 && point.y == -1) {
			joystick_update.joystick_info = {0.0f, 0.0f};
			joystick_update.update_joystick();
			return;
		}
		for (auto touch_object : touchables) {
			touch_object->handle_touch(point);
		}
	}
};

TouchSystem touchscreen_system;

TouchKeyboardButton map_button_handler(SDL_SCANCODE_SPACE, {0.7, 0.7}, {0.9, 0.8});
render::TextObject map_button_text("mymap");
transform::NoRotationTransform map_button_transform({{0.7, 0.7}, {0.9, 0.8}});
layers::ConstLayer map_button_layer(5);
render::ModelMatrix map_button_model_matrix;

render::DummyDrawable map_button_drawable(&shaders::static_object_program);

void init() {
	// map_button_transform.scale({0.1f, 0.1f});
	map_button.add(&map_button_text)
						.add(&map_button_handler)
						.add(&map_button_transform)
						.add(&map_button_model_matrix)
						.add(&map_button_layer)
						.add(&color::red)
						.add(&map_button_drawable)
						.bind();
}

init::CallbackOnStart map_button_init(&init);

}