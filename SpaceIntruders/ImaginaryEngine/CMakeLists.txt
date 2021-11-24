# CMakeList.txt: проект CMake для SpaceIntruders; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.
add_library(ImaginaryEngine STATIC src/engine.cpp src/engine.hpp)

if(NOT IOS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2)
	set(SDL2MAIN_LIBRARY SDL2main)
	set(SDL2_LIBRARY SDL2-static)
endif()

#if(NOT APPLE AND NOT ANDROID AND (UNIX OR MINGW))
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glew)
set(GLEW_LIBRARY libglew_static)
#endif()

target_link_libraries(ImaginaryEngine PUBLIC
		${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY} ${GLEW_LIBRARY})

target_include_directories(ImaginaryEngine 
		PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/src 
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/include
			${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include)

set(SYSTEM_LIBS -mwindows ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
