cmake_minimum_required(VERSION 3.16)

project(boost_valentine)
set(CMAKE_CXX_STANDARD 17)

if(POLICY CMP0167)
	cmake_policy(SET CMP0167 OLD)
endif()

find_package(Boost 1.89 REQUIRED COMPONENTS json)
add_executable(${PROJECT_NAME} main.cpp)

if (WIN32)
	target_link_libraries(boost_valentine PRIVATE ws2_32)
	target_compile_definitions(boost_valentine PRIVATE -D_WIN32_WINNT=0x0A00)
endif()

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /EHsc)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::headers Boost::json)
