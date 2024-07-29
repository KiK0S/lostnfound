#pragma once
#include <SDL2/SDL.h>
#include "components/dynamic_object.hpp"
#include "components/entity_system.hpp"
#include "systems/easy_drawable_system.hpp"
#include "components/gpu_program.hpp"
#include "systems/geometry_system.hpp"
#include "systems/input_system.hpp"
#include "systems/minimap_system.hpp"

namespace touchscreen {


std::unique_ptr<entity::Entity> joystick;
std::unique_ptr<entity::Entity> joystick_inner;
std::unique_ptr<render::SolidDrawable> joystick_drawable;
std::unique_ptr<render::SolidDrawable> joystick_inner_drawable;
std::unique_ptr<scene::SceneObject> joystick_scene;
std::unique_ptr<scene::SceneObject> joystick_inner_scene;
minimap::MiniMapUniforms joystick_uniform;

void init() {
	joystick = std::make_unique<entity::Entity>();
	joystick_drawable = std::make_unique<render::SolidDrawable>(&geometry::circle, &shaders::static_object_program);
	joystick_drawable->get_transform()->scale(glm::vec2(0.25f, 0.25f));
	joystick_drawable->get_transform()->translate(glm::vec2(-0.6f, 0.6f));
	joystick_drawable->color = &color::lighter_grey;
	joystick_drawable->uniforms.add(&joystick_uniform);
	joystick_scene = std::make_unique<scene::SceneObject>("main");
	joystick->add(joystick_drawable.get());
	joystick->add(joystick_scene.get());
	joystick->bind();

	joystick_inner = std::make_unique<entity::Entity>();
	joystick_inner_drawable = std::make_unique<render::SolidDrawable>(&geometry::circle, &shaders::static_object_program);
	joystick_inner_drawable->get_transform()->scale(glm::vec2(0.2f, 0.2f));
	joystick_inner_drawable->get_transform()->translate(glm::vec2(-0.6f, 0.6f));
	joystick_inner_drawable->color = &color::light_grey;
	joystick_inner_drawable->uniforms.add(&joystick_uniform);
	joystick_inner_scene = std::make_unique<scene::SceneObject>("main");
	joystick_inner->add(joystick_inner_drawable.get());
	joystick_inner->add(joystick_inner_scene.get());
	joystick_inner->bind();
}

struct TouchSystem: public dynamic::DynamicObject {
	TouchSystem(): dynamic::DynamicObject() {}

	bool inited = false;

	void init() {
		touchscreen::init();
		inited = true;
	}

	glm::vec2 joystick_info{0.0f, 0.0f};

	void update_joystick() {
		auto joystick_transform = joystick_inner->get<transform::TransformObject>();
		joystick_transform->translate(-joystick_transform->get_pos() + glm::vec2(-0.6f, 0.6f) + joystick_info * 0.1f);
	}

	void update() {
		if (!inited && SDL_GetNumTouchDevices() > 0) init();
		if (!inited) return;
		glm::vec2 point = input::get_touch() * 2.0f - glm::vec2(1.0f, 1.0f);
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

TouchSystem touchscreen_system;


}