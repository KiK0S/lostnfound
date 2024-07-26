#pragma once
#include "game_objects/player.hpp"
#include "game_objects/sprite.hpp"

namespace player {

struct RayCastPosition: public shaders::ShaderUniformsObject {
	RayCastPosition(): ShaderUniformsObject({&shaders::raycast_program}) {}
	~RayCastPosition(){}
	void reg_uniforms(GLuint program) {
		entity::Entity* e = get_entity();
		transform::TransformObject* t = e->get<transform::TransformObject>();
		glm::vec2 raycast_start = t->get_pos();
		auto playerLocation = glGetUniformLocation(program, "uStartPosition");
		glUniform2fv(playerLocation, 1, glm::value_ptr(raycast_start));
	}
};

RayCastPosition raycast_provider;

entity::Entity player;


sprite::AnimatedSprite player_sprite("monster", {-0.1f, -0.1f}, {0.1f, 0.1f}, 2, {
		{"idle", {0.3, 0.1, 0.1, 0.1}},
		{"runright", {0.05, 0.05, 0.05, 0.05, 0.05, 0.05}},
		{"runleft", {0.05, 0.05, 0.05, 0.05, 0.05, 0.05}}
	});

void init() {
	std::cout << "init player\n";
	player.add(&player_sprite)
				.add(&raycast_provider)
				.add(&player_control)
				.add(&player_scene)
				.bind();
}
init::CallbackOnStart player_init(&init);

entity::Entity* get_player() {
	return &player;
}


}