#! base
cmake_minimum_required(VERSION 3.10)
project(json_parser
			VERSION 1.2
    		LANGUAGES CXX
		)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)


#! compiling and linking
#include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB EXAMPLE_SOURCE_FILES ${CMAKE_SOURCE_DIR}/example/*.cpp)

add_library(libjson_parser SHARED ${SOURCE_FILES})
set_target_properties(libjson_parser PROPERTIES OUTPUT_NAME "json_parser")
target_include_directories(libjson_parser
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

foreach (EXAMPLE ${EXAMPLE_SOURCE_FILES})
	get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)
	add_executable(${EXAMPLE_NAME} ${EXAMPLE})
	target_link_libraries(${EXAMPLE_NAME} PRIVATE libjson_parser)
endforeach ()

install(TARGETS libjson_parser
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
)

install(DIRECTORY include/
		DESTINATION include
)