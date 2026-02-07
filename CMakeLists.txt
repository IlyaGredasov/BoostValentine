cmake_minimum_required(VERSION 3.10)

project(boost_valentine)
set(CMAKE_CXX_STANDARD 17)

if(POLICY CMP0167)
	cmake_policy(SET CMP0167 OLD)
endif()

find_package(Boost 1.89 REQUIRED COMPONENTS json)
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::headers Boost::json)
