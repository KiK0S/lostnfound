#pragma once
#include <memory>
#include <vector>

#include "game_objects/sprite.hpp"
#include "systems/render_system.hpp"
#include "components/blocking_object.hpp"
#include "game_objects/course.hpp"

#include "utils/random.hpp"
#include "game_objects/tilemap.hpp"

namespace map
{

uint8_t color[4] =	{56, 91, 94, 255};

std::vector<std::unique_ptr<render::DrawableObject>> sprites;
std::vector<std::unique_ptr<geometry::GeometryObject>> geometries;
std::vector<std::unique_ptr<scene::SceneObject>> scenes;
std::vector<std::unique_ptr<minimap::MiniMapEntity>> minimap_drawables;
std::vector<std::unique_ptr<visibility::BlockingEntity>> blocking_drawables;

std::unique_ptr<entity::Entity> map_object(const std::string& name, double x, double y, double width, double height) {
	auto full_sprite = std::make_unique<sprite::Sprite>(name, glm::vec2{y - height / 2, x - width / 2}, glm::vec2{y + height / 2, x + width / 2}, 3);
	
	auto minimap_sprite = std::make_unique<sprite::SpriteCustomProgram>(name, glm::vec2{(y) / 10.0 - 0.05, (x) / 10.0 - 0.05}, glm::vec2{(y) / 10.0 + 0.05, (x) / 10.0 + 0.05}, 3, &shaders::static_object_program);
	auto mini_entity = std::make_unique<entity::Entity>();
	mini_entity->add(minimap_sprite.get());
	mini_entity->bind();
	auto minimap_object = std::make_unique<minimap::MiniMapEntity>(std::move(mini_entity));
	auto circle = std::make_unique<geometry::Circle>();
	auto circle_drawable = std::make_unique<render::SolidDrawable>(circle.get());
	circle_drawable->transform.scale(glm::vec2{0.2f, 0.2f});
	circle_drawable->transform.translate(glm::vec2(y, x));
	
	auto blocking_entity = std::make_unique<entity::Entity>();
	blocking_entity->add(circle_drawable.get());
	blocking_entity->bind();
	auto blocking_object = std::make_unique<visibility::BlockingEntity>(std::move(blocking_entity));

	auto main_scene = std::make_unique<scene::SceneObject>("main");
	auto e = std::make_unique<entity::Entity>();
	e->add(full_sprite.get());
	e->add(minimap_object.get());
	e->add(blocking_object.get());
	e->add(main_scene.get());
	e->bind();
	sprites.push_back(std::move(full_sprite));
	sprites.push_back(std::move(minimap_sprite));
	sprites.push_back(std::move(circle_drawable));
	scenes.push_back(std::move(main_scene));
	minimap_drawables.push_back(std::move(minimap_object));
	blocking_drawables.push_back(std::move(blocking_object));
	geometries.push_back(std::move(circle));

	return e;
}

struct World {

	std::vector<std::unique_ptr<entity::Entity>> objects;
	tilemap::TileMap tilemap;
	int n, m;

	World(int n, int m): n(n), m(m), tilemap(n, m) {
		int treesCnt = 50;
		int rockCnt = 20;

		for (int i = 0; i < treesCnt; i++) {
			objects.emplace_back(map_object("tree", rnd::get_double(-10, 10), rnd::get_double(-10, 10), 0.4, 0.4));
		}
		for (int i = 0; i < rockCnt; i++) {
			objects.emplace_back(map_object("rock", rnd::get_double(-10, 10), rnd::get_double(-10, 10), 0.3, 0.3));
		}
	}
};

World world(100, 100);

} // namespace map
