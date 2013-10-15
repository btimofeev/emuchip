cmake_minimum_required(VERSION 2.8)

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
	
	SET(HEADERS
		src/chip_emu.h
		src/version.h)
	
	SET(MOCHEAD
		src/qt/displaywidget.h
		src/qt/mainwindow.h)
	
	SET(SOURCES
		src/qt/main.cpp
		src/qt/displaywidget.cpp
		src/qt/mainwindow.cpp
		src/chip_emu.cpp)
	
	find_package (Qt4 REQUIRED QTCORE QTGUI)
	if(NOT Qt4_FOUND)
		message(SEND_ERROR "Failed to find Qt4")
		return()
	endif()
	
	QT4_WRAP_CPP(HEADERS_MOC ${MOCHEAD})
	
	include_directories(${Qt_INCLUDE_DIR})
	INCLUDE(${QT_USE_FILE})
	ADD_DEFINITIONS(${QT_DEFINITIONS})
	add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${HEADERS_MOC})
	target_link_libraries (${PROJECT} ${QT_LIBRARIES})
endif()

install(TARGETS ${PROJECT} DESTINATION ${INSTALL_PATH})