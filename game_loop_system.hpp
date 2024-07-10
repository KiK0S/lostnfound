#pragma once

#include <chrono>
#include <functional>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdio.h>
#endif

namespace game_loop {

struct Dynamic;

std::vector<Dynamic*> dynamics;

struct Dynamic {
    Dynamic() {
        dynamics.push_back(this);
    }
    virtual ~Dynamic() {}
    virtual void update() = 0;
};


bool isRunning = true;

std::function<void()> render;

void Loop() {
    #ifdef __EMSCRIPTEN__
    try {
    #endif

    if (!isRunning) {
        return;
    }

    for (auto dynamic : dynamics)
        dynamic->update();

    // Pre-render
		// ??? 

    // Render
	render();
    
    #ifdef __EMSCRIPTEN__
    } catch(...) {
        emscripten_cancel_main_loop();
        // emscripten_force_exit(1);
    }
    #endif

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