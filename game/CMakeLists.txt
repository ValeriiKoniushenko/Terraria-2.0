add_executable(
	Terraria
	include/Terraria.h

	source/main.cpp
	source/Terraria.cpp
)

target_include_directories(
	Terraria PUBLIC
	include
)

target_link_libraries(
	Terraria PUBLIC
	Utils
	Logger
	glfw
	Glad
	Core-Wrappers
	Window
	Delegate
	World
	Input-Devices
	Misc
	Shapes
	freetype
	FreeType-Wrapper
	Animations
	Camera
	Texture-Manager
	Map
)

 add_custom_command(TARGET Terraria PRE_BUILD
 	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
 	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
 	COMMAND ${CMAKE_COMMAND} -E copy_directory
 	${CMAKE_CURRENT_SOURCE_DIR}/assets
 	${CMAKE_CURRENT_BINARY_DIR}/assets)

 add_custom_target(
 	Copy-Assets
 	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
 	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
 	COMMAND ${CMAKE_COMMAND} -E copy_directory
 	${CMAKE_CURRENT_SOURCE_DIR}/assets
 	${CMAKE_CURRENT_BINARY_DIR}/assets
 )