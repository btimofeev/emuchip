cmake_minimum_required(VERSION 2.8.11)

option(BUILDSDL "BUILDSDL" OFF)

SET(PROJVER
	v0.3)

SET(INSTALL_PATH /usr/local/bin/)

include(FindGit OPTIONAL)
if(GIT_FOUND)
	set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/module ${CMAKE_MODULE_PATH})
	include(GetGitRevisionDescription)
	git_describe(VERSION --tags)
else()
	set(VERSION %{PROJVER})
endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.cmake
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h)

if (BUILDSDL)
	SET(PROJECT
		emuchip-sdl)
	project(${PROJECT})
	message("Build SDL version")
	SET(HEADERS
		src/chip_emu.h
		src/version.h)

	SET(SOURCES
		src/sdl/main.cpp
		src/chip_emu.cpp)
	
	find_package(SDL REQUIRED)
	if(NOT SDL_FOUND)
		message(SEND_ERROR "Failed to find SDL")
		return()
	endif()
	include_directories(${SDL_INCLUDE_DIR})

	link_libraries(${SDL_LIBRARY})

	add_executable(${PROJECT} ${HEADERS} ${SOURCES})
	target_link_libraries (${PROJECT})
else()
	SET(PROJECT
		emuchip-qt)
	project(${PROJECT})
	message("Build Qt version")

	set(CMAKE_AUTOMOC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	
	set(HEADERS
		src/chip_emu.h
		src/version.h)

	set(MOCHEAD
		src/qt/displaywidget.h
		src/qt/mainwindow.h)

	set(SOURCES
		src/qt/main.cpp
		src/qt/displaywidget.cpp
		src/qt/mainwindow.cpp
		src/chip_emu.cpp)

	find_package (Qt5Widgets REQUIRED)
	if(NOT Qt5Widgets_FOUND)
		message(SEND_ERROR "Failed to find Qt5")
		return()
	endif()

	add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${HEADERS_MOC})
	target_link_libraries (${PROJECT} Qt5::Widgets)
endif()

install(TARGETS ${PROJECT} DESTINATION ${INSTALL_PATH})
