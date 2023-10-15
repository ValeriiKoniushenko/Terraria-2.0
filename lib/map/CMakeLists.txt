file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Map STATIC
	${Sources}
)

target_include_directories(Map PUBLIC include/)

target_link_libraries(Map PUBLIC
	Utils
	Core-Wrappers
	Texture-Manager
	Shapes
)