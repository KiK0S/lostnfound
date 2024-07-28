#pragma once
#include "components/gpu_program.hpp"
#include "components/transform_object.hpp"
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

transform::NoRotationTransform camera_transform;
ViewMatrix view_matrix_provider;

entity::Entity camera;

void init() {
	std::cout << "init camera\n";
	camera.add(&camera_transform)
				.add(&view_matrix_provider)
				.bind();
} 

init::CallbackOnStart camera_init(&init);


}