#pragma once
#include <string>

namespace file {

std::string asset(std::string name) {
	#ifdef __EMSCRIPTEN__
		return "/assets/" + name;
	#else
		return "../assets/" + name;
	#endif
}
}