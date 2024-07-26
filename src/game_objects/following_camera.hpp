#pragma once
#include "components/gpu_program.hpp"
#include "components/transform_object.hpp"
#include "game_objects/player.hpp"
#include "glm/gtx/norm.hpp"

namespace camera {

struct ViewMatrix : public shaders::ShaderUniformsObject {
	ViewMatrix() : shaders::ShaderUniformsObject({&shaders::static_object_program, &shaders::raycast_program, &shaders::bezier_program, &shaders::visibility_program}) {}
	
	void reg_uniforms(GLuint program) {

		entity::Entity* e = get_entity();
		transform::TransformObject* tr = e->get<transform::TransformObject>();

		auto viewLocation = glGetUniformLocation(program, "uViewMatrix");
		glm::mat4 viewMatrix = tr->get_model_matrix();
		viewMatrix[0][3] *= -1;
		viewMatrix[1][3] *= -1;
		glUniformMatrix4fv(viewLocation, 1, GL_TRUE, glm::value_ptr(viewMatrix));
	}
};


struct PlayerFollowing : public dynamic::DynamicObject {
	PlayerFollowing(): dynamic::DynamicObject() {}
	~PlayerFollowing() {}
	void update() {
		entity::Entity* e = get_entity();
		transform::TransformObject* camera = e->get<transform::TransformObject>();
		transform::TransformObject* player = player::get_player()->get<transform::TransformObject>();

		glm::vec2 diff = player->get_pos() - camera->get_pos();
		float d = glm::length2(diff);
		if (d < 0.1)
			return;
		float v = 0.05;
		if (d > 0.4) {
			v *= 5;
		}
		camera->translate(diff * v);
	}
};

transform::NoRotationTransform camera_transform;
PlayerFollowing following_rule;
ViewMatrix view_matrix_provider;

entity::Entity camera;

void init() {
	std::cout << "init camera\n";
	camera.add(&camera_transform)
				.add(&following_rule)
				.add(&view_matrix_provider)
				.bind();
} 

init::CallbackOnStart camera_init(&init);


}