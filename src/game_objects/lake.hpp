
#pragma once
#include "systems/easy_drawable_system.hpp"

namespace lake {

geometry::Polygon lake_polygon("lake", {
		glm::vec2(-3.0f, 0.0f),
		glm::vec2(-2.5f, -0.1f),
		glm::vec2(-2.4f, 0.1f),
		glm::vec2(-1.0f, -0.3f),
		glm::vec2(-0.75f, -0.75f),
		glm::vec2(-1.0f, -2.0f),
		glm::vec2(-2.5f, -1.3f)
	});



std::vector<std::unique_ptr<scene::SceneObject>> spawned_scenes;
std::vector<std::unique_ptr<sprite::Sprite>> spawned_sprites;
scene::SceneObject lake_scene("main");
render::SolidDrawable lake_drawable(&lake_polygon);
render::SolidDrawable lake_mini_drawable(&lake_polygon, &shaders::static_object_program);

spawn::SpawnerRuleContainer duck_spawning({
															spawn::SpawningRule{
																0.75,
																[](glm::vec2 pos) {
																	auto sprite_ptr = std::make_unique<sprite::Sprite>("duck", glm::vec2{pos.x - 0.1, pos.y - 0.1}, glm::vec2{pos.x + 0.1, pos.y + 0.1}, 2);
																	auto scene_ptr = std::make_unique<scene::SceneObject>("main");
																	auto duck = std::make_unique<entity::Entity>();
																	duck->add(sprite_ptr.get());
																	duck->add(scene_ptr.get());
																	duck->bind();
																	spawned_scenes.emplace_back(std::move(scene_ptr));
																	spawned_sprites.emplace_back(std::move(sprite_ptr));
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