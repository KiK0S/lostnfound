#pragma once

#include <chrono>
#include <functional>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdio.h>
#endif

namespace game_loop {

bool isRunning = true;

std::function<void()> update;
std::function<void()> render;

void Loop() {
    if (!isRunning) {
        return;
    }

    update();

    // Pre-render
		// ??? 

    // Render
	render();

    // Post render
		// ??
}


#ifdef __EMSCRIPTEN__

void startLoop() {
    emscripten_set_main_loop(Loop, 0, 1);
}

#else
void startLoop() {
    while (true) {
        Loop();
    }
}
#endif

}