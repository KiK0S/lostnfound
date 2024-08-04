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
struct Curve {
	std::vector<std::unique_ptr<entity::Entity>> entities;
	Curve(std::vector<glm::vec2> points) {
		if (points.size() == 0) return;
		glm::vec2 prev = points[0];
	
		for (int i = 1; i + 2 < points.size(); i += 2) {
			glm::vec2 next = i + 4 < points.size() ? (points[i + 1] + points[i + 2]) / 2.0f : points[i + 2];
			add_curve_segment(std::array{
													prev, 
													points[i],
													points[i + 1],
													next}, i);			
			prev = next;
		}
	}

	void add_curve_segment(std::array<glm::vec2, 4> points, int idx) {
		// std::cout << "add_curve_segment\n";
		auto e = std::make_unique<entity::Entity>();
		auto mini_e = arena::create<entity::Entity>();
		auto patch = arena::create<geometry::BezierCurve>(std::string("river-") + std::to_string(idx), points);
		auto drawable = arena::create<render::SolidDrawable>(patch, &shaders::bezier_program);
		auto mini_drawable = arena::create<render::SolidDrawable>(patch, &shaders::bezier_program);
		auto scened = arena::create<scene::SceneObject>("main");
		mini_drawable->transform.scale(glm::vec2(0.1f, 0.1f));
		mini_drawable->color = &color::blue;
		drawable->color = &color::blue;

		drawable->uniforms.add(patch);
		mini_drawable->uniforms.add(patch);
		mini_e->add(mini_drawable);
		mini_e->bind();
		auto minimaped = arena::create<minimap::MiniMapEntityPtr>(mini_e);
		e->add(drawable);
		e->add(minimaped);
		e->add(scened);
		e->bind();
		entities.push_back(std::move(e));
	}
};


void create_river(std::string name, std::vector<glm::vec2> points) {
	arena::create<Curve>(points);
}

Curve river({
	{-0.75f, -0.75f},
	{-0.25f,  1.0f},
	{0.25f, -1.0f},
	{0.75f,  0.0f},
	{1.0f, 0.0f},
	{1.25f, -0.5f},
	{1.7f, 2.0f},
	{1.8f, 1.0f},
	{1.6f, 0.8f},
	{1.5f, 0.9f}
});

}