#pragma once
#include <vector>

namespace arena {

const int SIZE = 1024 * 1024 * 1;

struct Buffer{
	Buffer(int size): size(size), begin(0) {
		data = new char[size];
	}
	~Buffer() {
		delete[] data;
	}

	template <typename T, typename ...Args>
	T* create(Args ...args) {
		assert(begin + sizeof(T) < size);
		T* ptr = new(data + begin) T(args...);
		begin += sizeof(T);
		return ptr;
	}

	char* data;
	int begin;
	int size;
	std::vector<int> sizes;
};

Buffer default_buffer(SIZE);

Buffer* current_buffer = &default_buffer;

template <typename T, typename ...Args>
T* create(Args ...args) {
	return current_buffer->create<T>(args...);
}

void run_with_arena(int size, std::function<void()> callback) {
	Buffer new_buf(size);
	Buffer* old = current_buffer;
	current_buffer = &new_buf;
	callback();
	current_buffer = old;
}

}