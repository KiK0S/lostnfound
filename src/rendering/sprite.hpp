#pragma once
#include "systems/definitions/animated_object.hpp"
#include "systems/definitions/drawable_object.hpp"
#include "systems/definitions/controllable_object.hpp"
#include "systems/render_system.hpp"
#include "rendering/programs.hpp"
#include "utils/file_system.hpp"
#include "camera/camera.hpp"
#include <functional>
#include <string>

namespace sprite {
	struct Sprite : public render::DrawableObject, input::ControllableObject {

		Sprite(std::string name, float l, float t, float r, float b, std::function<void(SDL_Event, Sprite*)>&& f, int layer): name(name), l(l), t(t), r(r), b(b), user_callback(std::move(f)), layer(layer), render::DrawableObject(), input::ControllableObject() {}
		Sprite(std::string name, float l, float t, float r, float b): Sprite(name, l, t, r, b, [](SDL_Event, Sprite*) {}, 1) {}
		Sprite(std::string name, float l, float t, float r, float b, int layer): Sprite(name, l, t, r, b, [](SDL_Event, Sprite*) {}, layer) {}
		~Sprite() {}
		virtual std::vector<float> get_pos() {
			return {
				-1.0f, -1.0f, // Vertex 1: top left
				1.0f, -1.0f, // Vertex 2: top right
				-1.0f, 1.0f, // Vertex 3: bottom left

				1.0f, -1.0f, // Vertex 4: top right
				1.0f, 1.0f,  // Vertex 6: bottom right
				-1.0f, 1.0f, // Vertex 5: bottom left
			};
		}
		virtual shaders::Program* get_program() {
			return &gpu_programs::raycast_program;
		}
		virtual std::vector<float> get_uv() {
			return {
				0.0f,  1.0f, // Vertex 1: top left
				1.0f,  1.0f, // Vertex 2: top right
				0.0f, 0.0f, // Vertex 3: bottom left

				1.0f,  1.0f, // Vertex 4: top right
				1.0f, 0.0f,  // Vertex 6: bottom right
				0.0f, 0.0f // Vertex 5: bottom left
			};
		}

		// [-1, 1] -> [l, r]
		// +1
		// [0, 2]
		// / (2)
		// [0, 1]
		// * (r - l)
		// [0, r - l]
		// + l
		virtual std::vector<float> get_model_matrix() {
			return {
				(r - l) / 2.0f, 0.0f, 0.0f, l + (r - l) / 2.0f,
				0.0f, (b - t) / 2.0f, 0.0f, t + (b - t) / 2.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}
		virtual GLuint get_texture() {
			return render::get_texture(file::asset(get_name() + ".png"));
		}
		virtual std::string get_name() const {
			return name;
		}
		void handle_user_action(SDL_Event e) {
			user_callback(e, this);
		}
		virtual int get_layer() const {
			return layer;
		}
		virtual bool show() {
			if (l - camera::camera.pos.x >= 1.2 || camera::camera.pos.x - r >= 1.2 || t - camera::camera.pos.y >= 1.2 || camera::camera.pos.y - b >= 1.2)
				return false;
			return true; 
		}
		float l, t, r, b;
		std::string name;
		int layer;
		std::function<void(SDL_Event, Sprite*)> user_callback;
	};

	struct AnimatedSprite : public Sprite, animation::AnimatedObject {

		AnimatedSprite(std::string name, float l, float t, float r, float b, std::function<void(SDL_Event, Sprite*)>&& f): Sprite(name, l, t, r, b, std::move(f), 1), animation::AnimatedObject() {}
		AnimatedSprite(std::string name, float l, float t, float r, float b): AnimatedSprite(name, l, t, r, b, [](SDL_Event, Sprite*) {}) {}
		~AnimatedSprite() {}
		GLuint get_texture() {
			while (durations[get_state()][current_index] <= time) {
				time -= durations[get_state()][current_index];
				current_index += 1;
				if (current_index == durations[get_state()].size()) {
					current_index = 0;
				}
			}
			return render::get_texture(file::asset(get_name() + "_" + get_state() + "_" + std::to_string(current_index) + ".png"));
		}
		void update(float dt) {
			time += dt;
		}
		void set_state(std::string state) {
			if (this->state == state)
				return;
			this->state = state;
			time = 0.0f;
			current_index = 0;
		}
		std::string get_state() { return state; }
		std::string state = "idle";
		std::map<std::string, std::vector<float>> durations;
		size_t current_index = 0;
		float time = 0;
	};
}