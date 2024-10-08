#pragma once
#include "components/entity_system.hpp"
#include "geometry_system.hpp"
#include "components/collision_object.hpp"
#include <iostream>


namespace level_loader {

entity::Entity* parse_entity(std::istream& in, shaders::Program* program = &shaders::raycast_program);
std::vector<entity::Entity*> parse(std::istream& in);


std::string parse_entity_description(std::istream& in) {
	std::string name;
	in >> name;
	std::string res = name;
	std::string token;
	while (in >> token) {
		res = res + ' ' + token;
		if (token == name) {
			break;
		}
	}
	return res;
}
geometry::GeometryObject* parse_geometry(std::istream& in, std::string name) {
	std::string type;
	in >> type;

	int n;
	in >> n;
	std::vector<glm::vec2> points(n);
	for (int i = 0; i < n; i++) {
		in >> points[i].x >> points[i].y;
	}
	if (type == "polygon") {
		return arena::create<geometry::Polygon>(name, points);
	} else {
		return arena::create<geometry::Curve>(name, points);
	}
}

texture::TexturedObject* parse_texture(std::istream& in) {
	std::string filepath;
	in >> filepath;
	return arena::create<texture::OneTextureObject>(filepath);
}

layers::LayeredObject* parse_layer(std::istream& in) {
	int layer_num;
	in >> layer_num;
	return arena::create<layers::ConstLayer>(layer_num);
}

collision::CollisionObject* parse_collision(std::istream& in) {
	return arena::create<collision::CollisionObject>();
}

color::ColoredObject* parse_color(std::istream& in) {
	int r;
	int g;
	int b;
	int a;
	in >> r >> g >> b >> a;
	return arena::create<color::OneColor>(glm::vec4{r / 255.0, g / 255.0, b / 255.0, a / 255.0});
}

spawn::SpawnerObject* parse_spawner(std::istream& in) {
	double density;
	in >> density;
	std::cerr << density << std::endl;
	auto desc = parse_entity_description(in);
	std::cerr << density << std::endl << desc << std::endl;
	return arena::create<spawn::SpawnerRuleContainer>(std::vector{
															spawn::SpawningRule{
																0.75,
																[=](glm::vec2 pos) {
																	std::cerr << desc << std::endl;
																	std::istringstream new_e_desc(desc);
																	auto new_e = parse_entity(new_e_desc);
																	new_e->get<transform::TransformObject>()->translate(pos);
																	new_e->get<transform::TransformObject>()->scale({0.1f, 0.1f});
																	new_e->add(&geometry::quad);

																	auto scene = arena::create<scene::SceneObject>("main");
																	new_e->add(scene);
																	new_e->bind();
																	return new_e;
																}
															}
														});
}

minimap::MiniMapObject* parse_minimap(std::istream& in) {
	auto e = parse_entity(in, &shaders::static_object_program);
	e->add(&texture::no_texture);
	e->add(&shaders::no_uniforms);
	e->get<transform::TransformObject>()->scale({0.1f, 0.1f});
	return arena::create<minimap::MiniMapEntityPtr>(e);
}

entity::Entity* parse_entity(std::istream& in, shaders::Program* program) {
	std::string name;

	if (!(in >> name)) return nullptr;
	entity::Entity* e = arena::create<entity::Entity>();
	std::string comp;
	
	while (in >> comp) {
		if (comp == name) break;
		if (comp == "geometry") e->add(parse_geometry(in, name));
		if (comp == "texture") e->add(parse_texture(in));
		if (comp == "color") e->add(parse_color(in));
		if (comp == "layer") e->add(parse_layer(in));
		if (comp == "minimap") e->add(parse_minimap(in));
		if (comp == "spawner") e->add(parse_spawner(in));
		if (comp == "collision") e->add(parse_collision(in));
	}
	auto model_matrix = arena::create<render::ModelMatrix>();
	e->add(model_matrix);
	auto translate = arena::create<transform::NoRotationTransform>();
	e->add(translate);
	e->add(arena::create<shaders::ProgramArgumentObject>(program));
	e->bind();
	return e;
}

std::vector<entity::Entity*> parse(std::istream& in) {
	std::vector<entity::Entity*> res;
	entity::Entity* e;
	while ((e = parse_entity(in)) != nullptr) {		
		res.push_back(e);
	}
	return res;
}

}

