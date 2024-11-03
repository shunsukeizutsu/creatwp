# CMakeのバージョンを設定
cmake_minimum_required(VERSION 3.16)

# プロジェクト名と使用する言語を設定
project(test_viewer_cmake CXX)

if(UNIX)
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if(COMPILER_SUPPORTS_CXX11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
	elseif(COMPILER_SUPPORTS_CXX0X)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread")
	else()
		message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Use a different C++ compiler.")
	endif()
endif()



include_directories(/usr/local/include/ssmtype ~/TC2024/localizer/src ~/creatFig/src )

# set the build type
set(CMAKE_BUILD_TYPE Release)
# confirmation messages
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

add_executable(cwp ./src/creatwp.cpp ~/creatFig/src/creatfig.cpp)
target_link_libraries(cwp m ssm)

add_executable(log2wp ./src/log2wp.cpp ~/creatFig/src/creatfig.cpp)
target_link_libraries(log2wp m ssm)

add_executable(list_pra ./src/adrewp.cpp)

add_executable(v_2cwp ./src/v2_creatwp.cpp ~/creatFig/src/creatfig.cpp)
target_link_libraries(v_2cwp m ssm rppjs ypspur)


add_executable(v3cwp ./src/v3_creatwp.cpp ~/creatFig/src/creatfig.cpp)
target_link_libraries(v3cwp m ssm)

add_executable(com_wp ./src/comp_wp.cpp ~/creatFig/src/creatfig.cpp)
target_link_libraries(com_wp m ssm)