#pragma once

#include <vector>
#include "components/dynamic_object.hpp"
#include "components/controllable_object.hpp"
#include "components/minimap_object.hpp"
#include "utils/random.hpp"
#include "game_objects/player.hpp"
#include "systems/geometry_system.hpp"
#include "glm/glm/vec2.hpp"
#include "glm/gtx/norm.hpp"

namespace course {

uint8_t color[4] = {191, 4, 151, 255};


struct Course {
	Course(int n) {
		positions.emplace_back(glm::vec2(0, 0));
		for (int i = 1; i < n; i++) {
			positions.emplace_back(glm::vec2(rnd::get_double(-10, 10), rnd::get_double(-10, 10)));
		}
		geometries.push_back(std::make_unique<geometry::Triangle>());
		add_minimap_entity(geometries[0].get(), positions[0]);
		for (int i = 1; i < n; i++) {
			add_minimap_line(positions[i - 1], positions[i], i);
			geometries.push_back(std::make_unique<geometry::Circle>());
			add_minimap_entity(geometries.back().get(), positions[i]);
		}	
	}
	~Course() {}

	void add_minimap_entity(geometry::GeometryObject* g, glm::vec2 pos) {
		auto mini_e = std::make_unique<entity::Entity>();
		auto mini_drawable = std::make_unique<render::SolidDrawable>(g, &shaders::static_object_program);
		mini_drawable->transform.scale(glm::vec2(0.1f, 0.1f));
		mini_drawable->transform.translate(glm::vec2{pos.x * 0.1f, pos.y * 0.1f});
		mini_e->add(mini_drawable.get());
		mini_e->bind();
		auto minimaped = std::make_unique<minimap::MiniMapEntity>(std::move(mini_e));
		auto e = std::make_unique<entity::Entity>();
		e->add(minimaped.get());
		e->bind();
		es.push_back(std::move(e));
		ms.push_back(std::move(minimaped));
		ds.push_back(std::move(mini_drawable));
	}
	void add_minimap_line(glm::vec2 from, glm::vec2 to, int idx) {
		auto mini_e = std::make_unique<entity::Entity>();
		auto patch = std::make_unique<geometry::BezierCurve>(std::string("leg-") + std::to_string(idx), std::array<glm::vec2, 4>{from, from, to, to});
		auto mini_drawable = std::make_unique<render::SolidDrawable>(patch.get(), &shaders::bezier_program);
		mini_drawable->transform.scale(glm::vec2(0.1f, 0.1f));
		mini_drawable->uniforms.add(patch.get());
		mini_e->add(mini_drawable.get());
		mini_e->bind();
		auto minimaped = std::make_unique<minimap::MiniMapEntity>(std::move(mini_e));
		auto e = std::make_unique<entity::Entity>();
		e->add(minimaped.get());
		e->bind();
		es.push_back(std::move(e));
		ms.push_back(std::move(minimaped));
		geometries.push_back(std::move(patch));
		ds.push_back(std::move(mini_drawable));
	}
	std::vector<glm::vec2> positions;
	std::vector<std::unique_ptr<entity::Entity>> es;
	std::vector<std::unique_ptr<geometry::GeometryObject>> geometries;
	std::vector<std::unique_ptr<minimap::MiniMapObject>> ms;
	std::vector<std::unique_ptr<render::DrawableObject>> ds;
	float control_radius = 0.05;
	uint32_t current_control = 0;

};
Course course(5);

struct CourseUpdate: public dynamic::DynamicObject {
	CourseUpdate(): dynamic::DynamicObject() {}
	void update() {
		if (course.current_control == course.positions.size())
			return;
		glm::vec2 diff = player::player.get<transform::TransformObject>()->get_pos() - course.positions[course.current_control];
		float d = glm::length2(diff);
		if (d > course.control_radius)
			return;

//  todo:
//		drawText(control_number);
		course.current_control++;
		std::cout << "found control\n";
	}
	void bind(entity::Entity*) {}
};

struct CourseHint: public input::ControllableObject {
	CourseHint(): input::ControllableObject() {}
	void handle_user_action(SDL_Event e) {
		if (e.key.keysym.scancode == SDL_SCANCODE_E) {
			glm::vec2 diff = player::player.get<transform::TransformObject>()->get_pos() - course.positions[course.current_control];
			float d = glm::length2(diff);
			std::cout << d << '\n';
		}
	}
};

CourseUpdate course_update;
CourseHint course_hint;

}