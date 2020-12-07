cmake_minimum_required(VERSION 3.12)

project(Exam Autumn 2020)

find_package(OpenGL REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(external/glfw-3.3.2)
add_subdirectory(external/glew-cmake-2.1.0)
add_subdirectory(external/glm-0.9.9.8)

add_executable(${PROJECT_NAME}
	main.cpp				header/dictionary.h
	functionality.cpp		header/functionality.h
	camera.cpp				header/camera.h
	class/skybox.cpp		class/header/skybox.h		class/shader/skybox.h
	class/terrain.cpp		class/header/terrain.h		class/shader/terrain.h
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/header
	${CMAKE_SOURCE_DIR}/external/stb/include
	${CMAKE_SOURCE_DIR}/external/tiny_obj_loader/include
)

target_compile_definitions(
	${PROJECT_NAME} PRIVATE
	GLEW_STATIC
	STB_IMAGE_IMPLEMENTATION
	TINYOBJLOADER_IMPLEMENTATION
)

target_link_libraries(${PROJECT_NAME}
	libglew_static
	glfw
	glm
	OpenGL::GL
)

add_custom_command(
	TARGET ${PROJECT_NAME} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/HeightMaps
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/HeightMaps
)

add_custom_command(
	TARGET ${PROJECT_NAME} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/NormalMapping
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/NormalMapping
)

add_custom_command(
	TARGET ${PROJECT_NAME} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/Objects
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Objects
)
