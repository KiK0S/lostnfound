#pragma once
#include "components/animated_object.hpp"
#include "components/drawable_object.hpp"
#include "components/controllable_object.hpp"
#include "systems/render_system.hpp"
#include "utils/file_system.hpp"
#include <functional>
#include <string>

namespace geometry {
	struct Quad : public geometry::GeometryObject {

		Quad(): geometry::GeometryObject() {
			pos = {
				glm::vec2{-1.0f, -1.0f}, // Vertex 1: top left
				glm::vec2{1.0f, -1.0f}, // Vertex 2: top right
				glm::vec2{-1.0f, 1.0f}, // Vertex 3: bottom left

				glm::vec2{1.0f, -1.0f}, // Vertex 4: top right
				glm::vec2{1.0f, 1.0f},  // Vertex 6: bottom right
				glm::vec2{-1.0f, 1.0f}, // Vertex 5: bottom left
			};
			uv = {
				glm::vec2{0.0f,  1.0f}, // Vertex 1: top left
				glm::vec2{1.0f,  1.0f}, // Vertex 2: top right
				glm::vec2{0.0f, 0.0f}, // Vertex 3: bottom left

				glm::vec2{1.0f,  1.0f}, // Vertex 4: top right
				glm::vec2{1.0f, 0.0f},  // Vertex 6: bottom right
				glm::vec2{0.0f, 0.0f} // Vertex 5: bottom left
			};
		}
		~Quad() {}
		virtual std::vector<glm::vec2> get_pos() {
			return pos;
		}
		virtual std::vector<glm::vec2> get_uv() {
			return uv; 
		}
		virtual int get_size() {
			return pos.size();
		}
		virtual std::string get_name() const {
			return "quad";
		}
		void bind(entity::Entity*) {}

		std::vector<glm::vec2> pos, uv;
	};

	Quad quad;

}