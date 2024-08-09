#pragma once
#include "components/entity_system.hpp"
#include "components/scene_object.hpp"
#include "components/minimap_object.hpp"
#include <vector>
#include <memory>
#include "geometry/curve.hpp"
#include "utils/arena.hpp"

#include "systems/easy_drawable_system.hpp"
namespace river {



void create_river(std::string name, std::vector<glm::vec2> points) {
	auto geom = arena::create<geometry::Curve>(name, points);

	auto e = arena::create<entity::Entity>();
	auto mini_e = arena::create<entity::Entity>();
	auto drawable = arena::create<render::SolidDrawable>(geom, &shaders::bezier_raycast_program);
	auto mini_drawable = arena::create<render::SolidDrawable>(geom, &shaders::bezier_program);
	auto scened = arena::create<scene::SceneObject>("main");
	mini_drawable->transform.scale(glm::vec2(0.1f, 0.1f));
	mini_drawable->color = &color::blue;
	drawable->color = &color::blue;

	mini_e->add(mini_drawable);
	mini_e->bind();
	auto minimaped = arena::create<minimap::MiniMapEntityPtr>(mini_e);
	e->add(drawable);
	e->add(minimaped);
	e->add(scened);
	e->bind();
}

}