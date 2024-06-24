cmake_minimum_required(VERSION 3.14)

set (CMAKE_CXX_STANDARD 17)

project(LostNFound VERSION 0.0.0)

include(FetchContent)

FetchContent_Declare(
    stb_image
    URL "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h"
    DOWNLOAD_NO_EXTRACT True
)

FetchContent_MakeAvailable(stb_image)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR})



set(EXE_SOURCES
	main.cpp
)

add_executable(LostNFound ${EXE_SOURCES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

if (EMSCRIPTEN)
		set(CMAKE_CXX_FLAGS  "-s USE_SDL=2")
		SET(CMAKE_EXE_LINKER_FLAGS "-s LLD_REPORT_UNDEFINED -sNO_DISABLE_EXCEPTION_CATCHING -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 --preload-file ${PROJECT_SOURCE_DIR}/assets@/assets")
else()
		set(CMAKE_CXX_FLAGS  "-fsanitize=undefined -fsanitize=bounds -fsanitize=address")
		find_package(GLEW REQUIRED)
		include_directories(${GLEW_INCLUDE_DIRS})
		target_link_libraries(LostNFound PRIVATE ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} GL)
endif()

target_link_libraries(LostNFound PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(LostNFound PRIVATE stb_image)

configure_file(${PROJECT_SOURCE_DIR}/index.html ${PROJECT_BINARY_DIR}/index.html COPYONLY)
