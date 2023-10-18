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
)