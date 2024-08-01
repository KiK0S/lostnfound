#pragma once
#include "components/component.hpp"


namespace render {
	struct TextObject;
	std::vector<TextObject*> texts;


	struct TextObject: public components::Component {
		TextObject(std::string text): text(text), components::Component() {
			texts.push_back(this);
		}
		virtual std::string get_text() {
			return text;
		}
		std::string text;
	};


	struct TextGeometry : public geometry::GeometryObject {
		TextGeometry(std::string name): name(name), geometry::GeometryObject() {}
		std::vector<glm::vec2> get_pos() {
			return pos;
		}
		int get_size() {
			return pos.size();
		}
		std::vector<glm::vec2> get_uv() {
			return uv;
		}
		std::string get_name() const {
			return name;
		}
		std::string name;
		std::vector<glm::vec2> pos;
		std::vector<glm::vec2> uv;
	};

 
}