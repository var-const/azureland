cmake_minimum_required(VERSION 2.8)

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

project(Azureland)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -pedantic-errors -Wall -Wextra")
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color")
endif()

set(PROJECT_SOURCE_DIR ../src)
add_executable(azureland cri_aabb.cpp cri_aabb_int.cpp rand.cpp cri_timer.cpp cri_math.cpp weapons/cri_reload.cpp weapons/cri_weapon.cpp weapons/cri_weapon_helpers.cpp)
