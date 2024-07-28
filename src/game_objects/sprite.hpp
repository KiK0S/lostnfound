#pragma once
#include "components/animated_object.hpp"
#include "components/drawable_object.hpp"
#include "components/gpu_program.hpp"
#include "components/controllable_object.hpp"
#include "game_objects/camera.hpp"
#include "systems/render_system.hpp"
#include "systems/easy_drawable_system.hpp"
#include "systems/geometry_system.hpp"
#include "utils/file_system.hpp"
#include <functional>
#include <string>

namespace sprite {

	struct Sprite : public render::DrawableObject {

		Sprite(std::string name, glm::vec2 top_left, glm::vec2 bottom_right, int layer):
		 transform(top_left, bottom_right),
		 layer(layer),
		 texture(name) {}
		~Sprite() {}
		virtual geometry::GeometryObject* get_geometry() {
			return &geometry::quad;
		}
		virtual transform::TransformObject* get_transform() {
			return &transform;
		}
		virtual layers::LayeredObject* get_layer() {
			return &layer;
		}
		virtual texture::TexturedObject* get_texture() {
			return &texture;
		}
		virtual shaders::ShaderUniformsObject* get_uniform() {
			return &model_matrix;
		}
		virtual shaders::Program* get_program() {
			return &shaders::raycast_program;
		}

		virtual void bind(entity::Entity* entity) {
			get_geometry()->bind(entity);
			get_transform()->bind(entity);
			get_layer()->bind(entity);
			get_texture()->bind(entity);
			get_uniform()->bind(entity);
		}

		bool hide() {
			glm::vec2 pos = get_transform()->get_pos();
			transform::TransformObject* camera_transform = camera::camera.get<transform::TransformObject>();
			glm::vec2 camera_pos = camera_transform->get_pos();
			pos -= camera_pos;
			if (pos.x <= -1.2 || pos.x >= 1.2 || pos.y >= 1.2 || pos.y <= -1.2)
				return true;
			return false;
		}
		transform::NoRotationTransform transform;
		layers::ConstLayer layer;
		texture::OneTextureObject texture;
		render::ModelMatrix model_matrix;
	};

	struct AnimatedSprite: public Sprite {
		AnimatedSprite(std::string name, glm::vec2 top_left, glm::vec2 bottom_right, int layer, std::map<std::string, std::vector<float>> data): Sprite(name, top_left, bottom_right, layer), animated_texture(name, data) {
		}

		virtual texture::TexturedObject* get_texture() {
			return &animated_texture;
		}
		texture::AnimatedTexture animated_texture;
	};


	struct SpriteCustomProgram: public Sprite {
		SpriteCustomProgram(std::string name, glm::vec2 top_left, glm::vec2 bottom_right, int layer, shaders::Program* program): Sprite(name, top_left, bottom_right, layer), program(program) {
		}
		virtual shaders::Program* get_program() {
			return program;
		}
		shaders::Program* program;
	};
}