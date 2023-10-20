file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Texture-Manager STATIC
	${Sources}
)

target_include_directories(Texture-Manager PUBLIC include/)

target_link_libraries(Texture-Manager PUBLIC
	Utils
	Core-Wrappers
	Shapes
)