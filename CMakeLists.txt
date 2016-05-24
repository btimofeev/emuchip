cmake_minimum_required(VERSION 2.8.11)

SET(APP_VERSION v0.3)

SET(INSTALL_PATH /usr/local/bin/)

option(BUILD_SDL "Build emuchip-sdl." ON)
option(BUILD_QT "Build emuchip-qt." ON)

if ((NOT BUILD_SDL) AND (NOT BUILD_QT))
	message(SEND_ERROR "Error: you have disabled all build options.")
	return()
endif()

include(FindGit OPTIONAL)
if(GIT_FOUND AND (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git))
	set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/module ${CMAKE_MODULE_PATH})
	include(GetGitRevisionDescription)
	git_describe(VERSION --tags)
else()
	set(VERSION ${APP_VERSION})
endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.cmake
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h)

if (BUILD_SDL)
	SET(PROJECT_SDL
		emuchip-sdl)
	project(${PROJECT_SDL})
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

	add_executable(${PROJECT_SDL} ${HEADERS} ${SOURCES})
	target_link_libraries (${PROJECT_SDL})
endif()

if (BUILD_QT)
	SET(PROJECT_QT
		emuchip-qt)
	project(${PROJECT_QT})
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

	add_executable(${PROJECT_QT} ${HEADERS} ${SOURCES} ${HEADERS_MOC})
	target_link_libraries (${PROJECT_QT} Qt5::Widgets)
endif()

install(TARGETS ${PROJECT_SDL} ${PROJECT_QT} DESTINATION ${INSTALL_PATH})
