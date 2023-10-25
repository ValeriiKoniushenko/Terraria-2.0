file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Entity STATIC
	${Sources}
)

target_include_directories(Entity PUBLIC include/)

target_link_libraries(Entity PUBLIC
	Utils
	Core-Wrappers
	Game
)