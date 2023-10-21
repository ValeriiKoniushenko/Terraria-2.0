file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Game STATIC
	${Sources}
)

target_include_directories(Game PUBLIC include/)

target_link_libraries(Game PUBLIC
	BaseGame
	Map
	Camera
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
	Texture-Manager
)