#pragma once
#include "components/entity_system.hpp"
#include "components/scene_object.hpp"
#include "components/minimap_object.hpp"
#include <vector>
#include <memory>
#include "geometry/curve.hpp"

#include "systems/easy_drawable_system.hpp"
namespace river {
struct Curve {
	std::vector<std::unique_ptr<geometry::BezierCurve>> patches;
	std::vector<std::unique_ptr<render::DrawableObject>> drawables;
	std::vector<std::unique_ptr<minimap::MiniMapObject>> minimaps;
	std::vector<std::unique_ptr<entity::Entity>> entities;
	std::vector<std::unique_ptr<scene::SceneObject>> scenes;
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
		std::cout << "add_curve_segment\n";
		auto e = std::make_unique<entity::Entity>();
		auto mini_e = std::make_unique<entity::Entity>();
		auto patch = std::make_unique<geometry::BezierCurve>(std::string("river-") + std::to_string(idx), points);
		auto drawable = std::make_unique<render::SolidDrawable>(patch.get(), &shaders::bezier_program);
		auto mini_drawable = std::make_unique<render::SolidDrawable>(patch.get(), &shaders::bezier_program);
		auto scened = std::make_unique<scene::SceneObject>("main");
		mini_drawable->transform.scale(glm::vec2(0.1f, 0.1f));
		drawable->uniforms.add(patch.get());
		mini_drawable->uniforms.add(patch.get());
		mini_e->add(mini_drawable.get());
		mini_e->bind();
		auto minimaped = std::make_unique<minimap::MiniMapEntity>(std::move(mini_e));
		e->add(drawable.get());
		e->add(minimaped.get());
		e->add(scened.get());
		e->bind();
		entities.push_back(std::move(e));
		minimaps.push_back(std::move(minimaped));
		drawables.push_back(std::move(drawable));
		drawables.push_back(std::move(mini_drawable));
		scenes.push_back(std::move(scened));
		patches.push_back(std::move(patch));
	}
};

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