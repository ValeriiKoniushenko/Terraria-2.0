file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Base-World STATIC
	${Sources}
)

target_include_directories(Base-World PUBLIC include/)

target_link_libraries(Base-World PUBLIC
	Game
)