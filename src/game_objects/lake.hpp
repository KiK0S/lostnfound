
#pragma once
#include "systems/easy_drawable_system.hpp"
#include "utils/arena.hpp"

namespace lake {

std::unique_ptr<entity::Entity> create_lake(std::string name, std::vector<glm::vec2> points) {
	auto polygon = arena::create<geometry::Polygon>(name, points);
	auto scene = arena::create<scene::SceneObject>("main");
	auto drawable = arena::create<render::SolidDrawable>(polygon);
	drawable->color = &color::blue;
	auto mini_drawable = arena::create<render::SolidDrawable>(polygon, &shaders::static_object_program);
	mini_drawable->transform.scale(glm::vec2{0.1f, 0.1f});
	mini_drawable->color = &color::blue;
	
	std::unique_ptr<entity::Entity> lake = std::make_unique<entity::Entity>();
	entity::Entity* mini_lake = arena::create<entity::Entity>();
	mini_lake->add(mini_drawable);
	mini_lake->bind();
	auto minimap = arena::create<minimap::MiniMapEntityPtr>(mini_lake);

	lake->add(drawable)
			.add(minimap)
			.add(scene)
			.bind();
	return lake;
}

geometry::Polygon lake_polygon("lake", {
		glm::vec2(-3.0f, 0.0f),
		glm::vec2(-2.5f, -0.1f),
		glm::vec2(-2.4f, 0.1f),
		glm::vec2(-1.0f, -0.3f),
		glm::vec2(-0.75f, -0.75f),
		glm::vec2(-1.0f, -2.0f),
		glm::vec2(-2.5f, -1.3f)
	});



scene::SceneObject lake_scene("main");
render::SolidDrawable lake_drawable(&lake_polygon);
render::SolidDrawable lake_mini_drawable(&lake_polygon, &shaders::static_object_program);

spawn::SpawnerRuleContainer duck_spawning({
															spawn::SpawningRule{
																0.75,
																[](glm::vec2 pos) {
																	auto sprite_ptr = arena::create<sprite::Sprite>("duck", glm::vec2{pos.x - 0.1, pos.y - 0.1}, glm::vec2{pos.x + 0.1, pos.y + 0.1}, 2);
																	auto scene_ptr = arena::create<scene::SceneObject>("main");
																	auto duck = std::make_unique<entity::Entity>();
																	duck->add(sprite_ptr);
																	duck->add(scene_ptr);
																	duck->bind();
																	return duck;
																}
															}
														});

entity::Entity lake;
std::unique_ptr<entity::Entity> mini_lake = std::make_unique<entity::Entity>();
minimap::MiniMapEntity lake_minimap(std::move(mini_lake));


void init() {
	std::cout << "init player\n";
	lake_mini_drawable.transform.scale(glm::vec2{0.1f, 0.1f});
	lake_mini_drawable.color = &color::blue;
	lake_drawable.color = &color::blue;

	lake_minimap.e->add(&lake_mini_drawable);
	lake_minimap.e->bind();
	lake.add(&lake_drawable)
			.add(&duck_spawning)
			.add(&lake_minimap)
			.add(&lake_scene)
			.bind();
}
init::CallbackOnStart lake_init(&init);

}