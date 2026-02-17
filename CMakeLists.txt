#! base
cmake_minimum_required(VERSION "3.10")
project("my_json_parser"
			VERSION "1.1.4"
    		LANGUAGES "CXX"
		)
set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#! packages
find_package(Boost REQUIRED COMPONENTS spirit)


#! compiling and linking
include_directories("${CMAKE_SOURCE_DIR}/")
file(GLOB MY_SOURCE "${CMAKE_SOURCE_DIR}/*.cpp")

add_executable(main ${MY_SOURCE})

target_link_libraries(main Boost::spirit)